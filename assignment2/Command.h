#ifndef COMMAND_H
#define COMMAND_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Command{
    public:
        Command(){}
    	virtual bool execute() = 0;
    	virtual void setRightChild(Command* r) = 0;
};


#endif