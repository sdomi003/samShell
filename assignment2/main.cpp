#include <stdio.h>
#include <string>
#include <iostream>
#include <cstddef>
#include "Command.h"
#include "Executable.h"
#include "Composites.h"

using namespace std;

int main(){
    
    string str = "echo 1 && echo 2 || echo 3 && ech 4; echo 5"; //; echo 5";
    
    Command* link, *ex = NULL;
    string args;
//  need to set the first left here
    size_t start = 0;
    size_t found = 0;
    
    found = str.find_first_of(";&|",start);
    args = str.substr(start, found - start);
    
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
    else{
        ex = new Executable(args);
        link = new Semi(ex, 0);
        start = found + 1;
    }
    
    found = str.find_first_of(";&|",start);
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
        else{
            ex = new Executable(args);
            link->setRightChild(ex);
            Command* temp = link;
            link = new Semi(temp, 0);
            start = found + 1;
            temp = 0;
        }
        found = str.find_first_of(";&|",start);
        args = str.substr(start, found - start);
    }
    // set last right
    ex = new Executable(args);
    link->setRightChild(ex);
    
    link->execute();

    
    return 0;
}