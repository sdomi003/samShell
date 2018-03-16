#include <stdio.h>
#include <string>
#include <iostream>
#include <cstddef>
#include "Command.h"
#include "Executable.h"
#include "Composites.h"

/*  IDEAS FOR TEST
        FOR THE CASE WHERE USER USES "test"
            SIMPLY PASRSE AS USUAL.
            WHEN AN EXECUTABLE IS GOING TP BE CREATED, FIRST CHECK TO SEE IF THE FIRST ARG IS "test"
            IF FIRST ARG IS TEST, THEN INSTEAD OF CREATING EXECUTABLE, CREATE TEST
        FOR THE CASE WHERE USER USES []
            ADD "[" TO THE SEARCH-FOR CHARACTERS.
            IF "[" IS FOUND, THEN SEARCH FOR ']' THEN SET THE ARGS TO THE STRING INSIDE "[]" AND CREATE A "test" OBJECT
            
    IDEA #2 - this is the one I'm implementing right now
        treat test and [] as normal executables, except when it comes time to execute, simply check to see if the first char is [ or if the first arg is "test"
        and then execute a different way (test function is already written by me in stat_practice.cpp)
                
*/      

// resolving issue with (echo A && echo B) && (echo C && echo D) && exit
// exit will walways be treated as a leaf
// create an if statement that returns NULL if exit is the argument
// then in the execute function 

using namespace std;

Command* make_tree(string str, size_t &start){
    bool single = false;
    Command* link, *ex = NULL;
    string args;
//  need to set the first left here
    //size_t start = 0; // NOTE:: being handled by the passed in variable now
    size_t found = 0;
    
    //CHANGE TO ";&|()"
    found = str.find_first_of(";&|()<>",start);
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
            //      NOTE NOTE NOTE NOTE 
            // FIRST THING AFTER ( COULD BE A [] OR TEST
        
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
    else if(str[found] == '>'){
        // first thing found was a redirect. Create executable out of args and set its output file to what comes after.
        ex = new Executable(args);
        if(str[found + 1] == '>'){
            // we have >>, need to update start and also the redirection_type of the executable since this is first pass
            
            // increase found not start bc I will set start to found+1 later
            ++ found;
            // 0 is >, 1 is >>, 2 is <
            ex->set_redirection_type(1);
        }
        else{
            // no need to ++start again, just set redirection_type to 0
            ex->set_redirection_type(0);
        }
        // in order to get output file, we need to run the find next composite again and start our tree up.
        // note: what do we do when we ONLY have echo a > yo.txt ?
        start = found + 1;
        found = str.find_first_of(";&|",start);
        args = str.substr(start, found - start);
        // the following shows that args has the correct value
        cout << "args: " << args << endl;
        // now we set the output file to the args found (which was really just a file name)
        ex->set_out_file(args);
        // check for >> instead of just >
        
        // also, after this step, we need to find the next composite which means we need to create a new composite here and set current executable as left.
        // only &, |, and ; are allowed after a file name
        if(str[found] == '&'){
            link = new And(ex, 0);
            start = found + 2;
        }
        else if(str[found] == '|'){
            link = new Or(ex, 0);
            start = found + 2;
        }
        else if(str[found] == ';'){
            link = new Semi(ex, 0);
            start = found + 1;
        }
        else{
            // npos
            // should I use string size -1 here ? I didn't but idk
            // output file already set so just set the bool is single to true and make link = the executable
            link = ex;
            single = true;
        }
        // now we should have a tree that consists of an executable with an output file in it, and its next composite set OR allow it to be a single if npos.
    }
    else{
        ex = new Executable(args);
        link = new Semi(ex, 0);
        start = found + 1;
    }
    
    
    found = str.find_first_of(";&|()>",start);
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
            // whenever we create a right subtree, the next step isn't to set right because we'll overwrite the right subtree we just made
            // to solve this, we make the next composite found and set its left child to the top of our tree
            
            // cout << "evaluating link after subtree added as right child" << endl;
            // link->execute();
            // cout << endl;
            //TRIALLLL
            
            start = found + 1;
            found = str.find_first_of(";&|()>",start);
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
            else if (str[found] == '>'){  // no executable to be made in the case of (echo hi && echo lo) > yo.txt
                // first step is to see if it's > or >> and update start & found accordingly
                cout << "OK FLAG" << endl;
                if(str[found + 1] == '>'){
                    ++found;
                    link->set_redirection_type(1);
                }
                else{
                    link->set_redirection_type(0);
                }
                // make sure to go to composites and use the files in and out and redirection types right on the right child
                // ISSUE ERROR: WHAT IF WE HAVE (echo a && echo b) > yo.txt, not echo a && (echo b && echo c) > yo.txt
                // they are different because in the second case, the () are a right subtree, but in the first case, the () are a single tree so 
                // only setting right to be printed to file would neglect the left side of the tree.
                // maybe give that case its own redirection_type number?
                
                // for now, solve case where we have echo a && (echo b && echo c) > yo.txt && echo b
                start = found + 1;
                found = str.find_first_of(";&|",start);
                args = str.substr(start, found - start);
                cout << "argsss : " << args << endl;
                link->set_out_file(args);
                
                // create a new command and set current link as left
                if(str[found] == '&'){
                    Command* temp = link;
                    link = new And(temp, 0);
                    start = start + 2;
                }
                else if(str[found] == '|'){
                    Command* temp = link;
                    link = new Or(temp, 0);
                    start = start + 2;
                }
                else if(str[found] == ';'){
                    Command* temp = link;
                    link = new Semi(temp, 0);
                    ++start;
                }
                else if(found == string::npos){
                    return link;
                }
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
    bool error = 0;
    do{
        int num_parenthesis = 0;
        int num_brackets = 0;
        error = 0;
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
        for(unsigned int i = 0; i < str.size(); ++i){
            if(str.at(i) == '(' || str.at(i) == ')'){
                ++num_parenthesis;
            }
            else if(str.at(i) == '[' || str.at(i) == ']'){
                ++num_brackets;
            }
        }
        
        if (num_brackets % 2 != 0){
            cout << "ERROR: Uneven number of brackets" << endl;
            error = 1;
        }
        if (num_parenthesis % 2 != 0){
            cout << "EROR: Unven number of parenthesis" << endl;
            error = 1; 
        }
    }while(error == 1);
    
    Command* base_node;
    while(str != "exit" && str != "quit"){
        size_t start = 0;
        base_node = make_tree(str, start);
        try{
            base_node->execute(0,0);
        }
        catch (const char* msg){
            cout << msg << endl;
            break;
        }
        
        // get user input again
        do{
            int num_parenthesis = 0;
            int num_brackets = 0;
            error = 0;
            cout << "$ ";
            size_t first_hash;
            getline(cin, str);
            first_hash = str.find_first_of("#",0);
            if(first_hash != string::npos){
                str.resize(first_hash); // maybe +1
            }
            for(unsigned int i = 0; i < str.size(); ++i){
                if(str.at(i) == '(' || str.at(i) == ')'){
                    ++num_parenthesis;
                }
                else if(str.at(i) == '[' || str.at(i) == ']'){
                    ++num_brackets;
                }
            }
            
            if (num_brackets % 2 != 0){
                cout << "ERROR: Uneven number of brackets" << endl;
                error = 1;
            }
            if (num_parenthesis % 2 != 0){
                cout << "ERROR: Unven number of parenthesis" << endl;
                error = 1; 
            }
        }while(error == 1);
    }

    
    return 0;
}