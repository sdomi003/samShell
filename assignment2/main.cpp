#include <stdio.h>
#include <string>
#include <iostream>
#include <cstddef>
#include "Command.h"
#include "Executable.h"
#include "Composites.h"

// where i left off:
// issue: echo hi && ( echo 1 && echo 2 ) || echo 3
// after the ), the next iteration will find || and set the right of link (which already has a right, ie the subtree) to the newly created executable.
// A good solution is to create a bool which will keep track if the new child should be left or right, and this will also eliminate the need for 
// the repitition of the if else statements.

using namespace std;

Command* make_tree(string str, size_t &start){
    bool single = false;
    Command* link, *ex = NULL;
    string args;
//  need to set the first left here
    //size_t start = 0; // NOTE:: being handled by the passed in variable now
    size_t found = 0;
    
    //CHANGE TO ";&|()"
    found = str.find_first_of(";&|()",start);
    args = str.substr(start, found - start);
    // args is fine here
    
    
    if(str[found] == '&'){
        ex = new Executable(args);
        link = new And(ex, 0);
        start = found + 2;
    }
    else if(str[found] == '|'){
        ex = new Executable(args);
        link = new Or(ex, 0);
        start = found + 2;
    }
    else if(found == string::npos){
        link = new Executable(args);
        single = true;
    }
    else if(str[found] == '('){
        // NOTE:: FIX ME :: when we have a single command in parenthesis like (echo hi) then it might output 2x since variable single cannot be set since we don't know if it is true
        ++found;
        link = make_tree(str, found); // double check that found here is the right param to use
        // cout << "special case execute: " << endl;
        // link->execute();
        // cout << endl;
        start = found + 1;
    
        found = str.find_first_of(";&|()",start);
        // set new link's left as old link
        if(str[found] == '&'){
            Command* temp = link;
            link = new And(temp, 0);
            start = found + 2;
            temp = 0;
        }
        else if(str[found] == '|'){
            Command* temp = link;
            link = new Or(temp, 0);
            start = found + 2;
            temp = 0;
        }
        else if(str[found] == ';'){
            Command* temp = link;
            link = new Semi(temp, 0);
            start = found + 1;
            temp = 0;
        }
        else if(found == string::npos){ // consider echo a && (echo b && ( echo c && echo d))
            return link;
        }
        else if(str[found] == ')'){ // idk, consider echo a && (echo b && ( echo c && echo d))
            return link;
        }
    }
    else if(str[found] == ')'){
        // test if they put spaces in between
        if(!args.empty()){
            link = new Executable(args);
        }
        start = found;
        return link;            // THIS COULD HAPPEN IF WE HAVE echo a && (echo b && ( echo c && echo d))
    }
    else{
        ex = new Executable(args);
        link = new Semi(ex, 0);
        start = found + 1;
    }
    
    found = str.find_first_of(";&|()",start);
    args = str.substr(start, found - start);
    
    while(found != string::npos){
        if(str[found] == '&'){
            ex = new Executable(args);
            link->setRightChild(ex);
            Command* temp = link;
            link = new And(temp, 0);
            start = found + 2;
            temp = 0;
        }
        else if(str[found] == '|'){
            ex = new Executable(args);
            link->setRightChild(ex);
            Command* temp = link;
            link = new Or(temp, 0);
            start = found + 2;
            temp = 0;
        }
        else if(str[found] == '('){
            ++found;
            link->setRightChild(make_tree(str, found));
            // cout << "evaluating link after subtree added as right child" << endl;
            // link->execute();
            // cout << endl;
            //TRIALLLL
            
            start = found + 1;
            found = str.find_first_of(";&|()",start);
            // set new link's left as old link
            if(str[found] == '&'){
                Command* temp = link;
                link = new And(temp, 0);
                start = found + 2;
                temp = 0;
            }
            else if(str[found] == '|'){
                Command* temp = link;
                link = new Or(temp, 0);
                start = found + 2;
                temp = 0;
            }
            else if(str[found] == ';'){
                Command* temp = link;
                link = new Semi(temp, 0);
                start = found + 1;
                temp = 0;
            }
            else if(found == string::npos){ // consider echo a && (echo b && ( echo c && echo d))
                return link;
            }
            else if(str[found] == ')'){ // idk, consider echo a && (echo b && ( echo c && echo d))
                return link;
            }
        }
        else if(str[found] == ')'){
            ex = new Executable(args);
            link->setRightChild(ex);
            start = found;
            // cout << ") entered" << endl;
            // cout << "evaluating link as it stands: " << endl;
            // link->execute();
            // cout << endl;
            return link;
        }
        else{
            ex = new Executable(args);
            link->setRightChild(ex);
            Command* temp = link;
            link = new Semi(temp, 0);
            start = found + 1;
            temp = 0;
        }
        found = str.find_first_of(";&|()",start);
        args = str.substr(start, found - start);
     
    }
    // set last right
    // I BELIEVE THAT THIS IS THE ISSUE RIGHT UNDERNEATH!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // argss will be emptyh if last was )
    if(!single && !args.empty()){
        ex = new Executable(args);
        link->setRightChild(ex); 
    }
    return link;
}

int main(){
    
    string str;
    cout << "$ ";
    size_t first_hash;
    getline(cin, str);
    first_hash = str.find_first_of("#",0);
    if(first_hash != string::npos){
        str.resize(first_hash); // maybe +1
    }
    // FIX ME: ADD A CHECKER TO ENSURE MATCHING ()
    // FIX ME: might as well check for (( here too
    // at this point, str should be ready to be parsed and has been checked for matching () 
    Command* base_node; 
    while(str != "exit" && str != "quit"){
        size_t start = 0;
        base_node = make_tree(str, start);
        base_node->execute();
        
        cout << "$ ";
        getline(cin, str);
        first_hash = str.find_first_of("#",0);
        if(first_hash != string::npos){
            str.resize(first_hash); // maybe +1
        }
    }


    return 0;
}