#include <stdio.h>
#include <string>
#include <iostream>
#include <cstddef>
#include "Command.h"
#include "Executable.h"
#include "Composites.h"
using namespace std;


// takes in current start and finds next args
// puts args in args and returns found position
string clean_file(string str){
    int start = 0;
    int end = str.size() - 1;
    while(str[start] == ' '){
        ++ start;
    }
    while(str[end] == ' '){
        --end;
    }
    return str.substr(start, end-start + 1);
}

size_t get_next_arg(string str, string &args, size_t start){
    size_t found = str.find_first_of(";&|()<>",start);
    // size_t temp_start;
    // take care of process | process | process | process
    // while(str[found] == '|' && str[found + 1] != '|'){
    //     temp_start = found + 1;
    //     found = str.find_first_of(";&|()<>",temp_start);
    // }
    args = str.substr(start, found - start);
    return found;
}

void change_status(Command* top_node, string file_name, string redirect_type){
    if(redirect_type == ">"){
        // clean file_name
        file_name = clean_file(file_name);
        top_node->set_out_file(file_name);
        if(top_node->get_redirection_type() == 2){
            top_node->set_redirection_type(3);
        }
        else{
            top_node->set_redirection_type(0);
        }
    }
    else if(redirect_type == ">>"){
        file_name = clean_file(file_name);
        top_node->set_out_file(file_name);
        if(top_node->get_redirection_type() == 2){
            top_node->set_redirection_type(4);
        }
        else{
            top_node->set_redirection_type(1);
        }
    }
    else if(redirect_type == "<"){
        file_name = clean_file(file_name);
        top_node->set_in_file(file_name);
        top_node->set_redirection_type(2);
    }
}

string get_user_input(){
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
    return str;
}



Command* make_tree(string str, size_t &start){
    bool single = false;
    Command* link, *ex = NULL;
    string args;
//  need to set the first left here
    //size_t start = 0; // NOTE:: being handled by the passed in variable now
    size_t found = 0;
    
    //CHANGE TO ";&|()"
    // found = str.find_first_of(";&|()",start);
    // args = str.substr(start, found - start);
    // args is fine here
    
    found = get_next_arg(str, args, start);
    
    if(str[found] == '|' && str[found+1] != '|'){
        ex = new Executable(args);
        link = new Pipe(ex, 0);
        start = found + 1;
    }
    else if(str[found] == '&'){
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
        found = get_next_arg(str, args, start);
        // set new link's left as old link
        if(str[found] == '|' && str[found + 1] != '|'){
            Command* temp = link;
            link = new Pipe(temp, 0);
            start = found + 1;
            temp = 0;
        }
        else if(str[found] == '&'){
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
        // if > is found. maybe add a new composite that just executes single child but opens file first
        // essentially a decorator
        // also works for <
        
    }
    else if(str[found] == ')'){
        // test if they put spaces in between
        if(!args.empty()){
            link = new Executable(args);
        }
        start = found;
        return link;            // THIS COULD HAPPEN IF WE HAVE echo a && (echo b && ( echo c && echo d))
    }
    else if(str[found] == '>' || str[found] == '<'){
        // maybe make this subrotine called single_>
        // first pass, create new ex and pass that in as the command to have its redirection_status changed
        // echo a > yo.txt && echo b
        ex = new Executable(args);
        while(found != string::npos && (str[found] == '>' || str[found] == '<')){
            if(str[found + 1] == '>'){
                start = found + 2;
                found = get_next_arg(str, args, start);
                change_status(ex, args, ">>");
            }
            else if(str[found] == '>'){
                start = found + 1;
                found = get_next_arg(str, args, start);
                change_status(ex, args, ">");
            }
            else if(found == string::npos){
                break;
            }
            else{
                // string[found] == '<'
                start = found + 1;
                found = get_next_arg(str, args, start);
                change_status(ex, args, "<");
            }
        }
        
        if(str[found] == '|' && str[found + 1] != '|'){
            link = new Pipe(ex, 0);
            start = found + 1;
        }
        else if(found == string::npos){
            return ex;
        }
        else if(str[found] == '&'){
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
        else if(str[found] == ')'){
            start = found + 1;
            return ex;
        }
        else{
            cout << "ERROR: NOTHING RETURNED IN FIRST PASS > " << endl;
        }
        // take 1 away from found to make it as if > yo.txt nev er happened
        // so that the rest of the code stays the same
        
    }
    else{
        ex = new Executable(args);
        link = new Semi(ex, 0);
        start = found + 1;
    }
    
    // echo a > yo.txt && echo b > hi.txt       && echo hi orr && (echo hi)
    found = get_next_arg(str, args, start);
    
    while(found != string::npos){
        if(str[found] == '|' && str[found + 1] != '|'){
            ex = new Executable(args);
            link->setRightChild(ex);
            Command* temp = link;
            link = new Pipe(temp, 0);
            start = found + 1;
            temp = 0;
        }
        else if(str[found] == '>' || str[found] == '<'){
            ex = new Executable(args);
            link->setRightChild(ex);
            
            while(found != string::npos && (str[found] == '>' || str[found] == '<')){
                if(str[found + 1] == '>'){
                    start = found + 2;
                    found = get_next_arg(str, args, start);
                    change_status(link, args, ">>");
                }
                else if(str[found] == '>'){
                    start = found + 1;
                    found = get_next_arg(str, args, start);
                    change_status(link, args, ">");
                }
                else if(found == string::npos){
                    break;
                }
                else{
                    // string[found] == '<'
                    start = found + 1;
                    found = get_next_arg(str, args, start);
                    change_status(link, args, "<");
                }
            }
            
            if(str[found] == '&'){
                link = new And(link, 0);
                start = found + 2;
            }
            else if(str[found] == '|'){
                link = new Or(link, 0);
                start = found + 2;
            }
            else if(str[found] == ';'){
                link = new Semi(link, 0);
                start = found + 1;
            }
            else if(found == string::npos){
                return link;
            }
            else if(str[found] == ')'){
                start = found + 1;
                return link;
            }
        }
        else if(str[found] == '&'){
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
            
            // maybe this start = found + 1 is a probelm
            start = found + 1;
            found = get_next_arg(str, args, start);
            // set new link's left as old link
            if(str[found] == '|' && str[found + 1] != '|'){
                Command* temp = link;
                link = new Pipe(temp, 0);
                start = found + 1;
                temp = 0;
            }
            else if(str[found] == '&'){
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
            else if(str[found] == '>' || str[found] == '<'){
                // echo a && (echo b && echo c) > yo.txt    && echo d
                // for first pass, iterate down the left side until null and set file (terrible but it might work)
                while(found != string::npos && (str[found] == '>' || str[found] == '<')){
                    if(str[found + 1] == '>'){
                        start = found + 2;
                        found = get_next_arg(str, args, start);
                        change_status(link, args, ">>");
                    }
                    else if(str[found] == '>'){
                        start = found + 1;
                        found = get_next_arg(str, args, start);
                        change_status(link, args, ">");
                    }
                    else if(found == string::npos){
                        break;
                    }
                    else{
                        // string[found] == '<'
                        start = found + 1;
                        found = get_next_arg(str, args, start);
                        change_status(link, args, "<");
                    }
                }
                if(str[found] == '|' && str[found + 1] != '|'){
                    link = new Pipe(link, 0);
                    start = found + 1;
                }
                if(found == string::npos){
                    return link;
                }
                if(str[found] == '&'){
                    link = new And(link, 0);
                    start = found + 2;
                }
                else if(str[found] == '|'){
                    link = new Or(link, 0);
                    start = found + 2;
                }
                else if(str[found] == ';'){
                    link = new Semi(link, 0);
                    start = found + 1;
                }
                else if(found == string::npos){
                    return link;
                }
                else if(str[found] == ')'){
                    start = found + 1;
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
        found = get_next_arg(str, args, start);
     
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
    str = get_user_input();
    
    Command* base_node;
    while(str != "exit" && str != "quit"){
        size_t start = 0;
        base_node = make_tree(str, start);
        try{
            base_node->execute();
        }
        catch (const char* msg){
            cout << msg << endl;
            break;
        }
        
        // get user input again
        str = get_user_input();
    }

    
    return 0;
}