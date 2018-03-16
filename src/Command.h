#ifndef COMMAND_H
#define COMMAND_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Command{
    protected:
        int in_fd;
        int out_fd;
        string in_file;
        string out_file;
        int re_type;
    public:
        Command(){}
    	virtual bool execute(int in_fd, int out_fd) = 0;
    	virtual void setRightChild(Command* r) = 0;
    	// fix below
    	void set_in_fd(int in_fd){this->in_fd = in_fd;}
    	void set_out_fd(int out_fd){this->out_fd = out_fd;}
    	void set_in_file(string in_file){this->in_file = in_file;}
    	void set_out_file(string out_file){this->out_file = out_file;}
    	void set_re_type(int re_type){this->re_type = re_type;}
};


#endif