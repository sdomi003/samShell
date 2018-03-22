#ifndef COMMAND_H
#define COMMAND_H

#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include "Command.h"
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <cstddef>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>
#include <fcntl.h>
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
        int  get_redirection_type(){return redirection_type;}
        void begin_redirection( int& old_out_fd, int& new_out_fd, int& old_in_fd, int& new_in_fd){
            if(redirection_type == 0){
                old_out_fd = dup(1);
                new_out_fd = open(out_file.c_str(), O_WRONLY | O_TRUNC | O_CREAT);
                dup2(new_out_fd, 1);
            }
            else if(redirection_type == 1){
                old_out_fd = dup(1);
                new_out_fd = open(out_file.c_str(), O_WRONLY | O_APPEND | O_CREAT);
                dup2(new_out_fd, 1);
            }
            else if(redirection_type == 2){
                old_in_fd = dup(0);
                new_in_fd = open(in_file.c_str(), O_RDONLY);
                dup2(new_in_fd, 0);
            }
            else if(redirection_type == 3){
                old_out_fd = dup(1);
                new_out_fd = open(out_file.c_str(), O_WRONLY | O_TRUNC | O_CREAT);
                dup2(new_out_fd, 1);
                old_in_fd = dup(0);
                new_in_fd = open(in_file.c_str(), O_RDONLY);
                dup2(new_in_fd, 0);
            }
            else if(redirection_type == 4){
                old_out_fd = dup(1);
                new_out_fd = open(out_file.c_str(), O_WRONLY | O_APPEND | O_CREAT);
                dup2(new_out_fd, 1);
                old_in_fd = dup(0);
                new_in_fd = open(in_file.c_str(), O_RDONLY);
                dup2(new_in_fd, 0);
            }
        }
    void end_redirection(int old_out_fd, int new_out_fd, int old_in_fd, int new_in_fd){
        if(redirection_type == 0 || redirection_type == 1 || redirection_type == 3 || redirection_type == 4){
			close(new_out_fd);
			dup2(old_out_fd, 1);
		}
		if(redirection_type == 2 || redirection_type == 3 || redirection_type == 4){
			close(new_in_fd);
			dup2(old_in_fd, 0);
		}
    }
        
};


#endif