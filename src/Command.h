#ifndef COMMAND_H
#define COMMAND_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Command{
    protected:
        string in_file;
        string out_file;
        int redirection_type;
    public:
        Command(){}
    	virtual bool execute() = 0;
    	virtual void setRightChild(Command* r) = 0;
    	void set_in_file(string in_file){this->in_file = in_file;}
    	void set_out_file(string out_file){this->out_file = out_file;}
    	void set_redirection_type(int redirection_type){this->redirection_type = redirection_type;}
};


#endif