#ifndef COMPOSITES_H
#define COMPOSITES_H
#include <iostream>
#include <string>
#include <vector>
#include "Command.h"

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
using namespace std;



class Semi : public Command{
	Command* left;
	Command* right;
	 
	
	public:
		Semi(Command* l, Command* r){this->left = l; this->right = r;  this->in_file = ""; this->out_file = ""; this->redirection_type = -1;}
		bool execute(){
			int old_out_fd = -1;
			int new_out_fd = -1;
			int old_in_fd  = -1;
			int new_in_fd  = -1;
			bool rtn;
			
			if(redirection_type != -1){
				left->execute();
				this->begin_redirection(old_out_fd, new_out_fd, old_in_fd, new_in_fd);
		    	rtn = right->execute();
		    	this->end_redirection(old_out_fd, new_out_fd, old_in_fd, new_in_fd);
		    	return rtn;
			}
			else{
				left->execute(); 
		    	return (right->execute());	
			}
		}
		void setRightChild(Command* r){this->right = r;}
};

class And : public Command{
	Command* left;
	Command* right;
	 
	
	public:
		And(Command* l, Command* r){this->left = l; this->right = r;  this->in_file = ""; this->out_file = ""; this->redirection_type = -1;}
		bool execute(){
			int old_out_fd = -1;
			int new_out_fd = -1;
			int old_in_fd  = -1;
			int new_in_fd  = -1;
			bool rtn;

			if(redirection_type != -1){
				if(left->execute()){
					this->begin_redirection(old_out_fd, new_out_fd, old_in_fd, new_in_fd);
					rtn = right->execute();
					this->end_redirection(old_out_fd, new_out_fd, old_in_fd, new_in_fd);
					return rtn;
				} 
				return false;
			}
			else{
				if(left->execute()){
					return (right->execute());
				} 
				return false;
			}
		}
		void setRightChild(Command* r){this->right = r;}
};

class Or : public Command{
	Command* left;
	Command* right;
	public:
		Or(Command* l, Command* r){this->left = l; this->right = r;  this->in_file = ""; this->out_file = ""; this->redirection_type = -1;}
		bool execute(){
			int old_out_fd = -1;
			int new_out_fd = -1;
			int old_in_fd  = -1;
			int new_in_fd  = -1;
			bool rtn;
			
			if(!left->execute()){
				this->begin_redirection(old_out_fd, new_out_fd, old_in_fd, new_in_fd);
				rtn = right->execute();
				this->end_redirection(old_out_fd, new_out_fd, old_in_fd, new_in_fd);
				return rtn;
			}
		    //cout << "execute Or" << endl;
		    return true;
		 
		}
		void setRightChild(Command* r){this->right = r;}
};

class Pipe : public Command{
	Command* left;
	Command* right;
	public:
		Pipe(Command* l, Command* r){this->left = l; this->right = r; this->redirection_type = -1;}
		bool execute(){
			int old_out_fd1 = -1;
			int new_out_fd1 = -1;
			int old_in_fd1  = -1;
			int new_in_fd1  = -1;
			
			this->begin_redirection(old_out_fd1, new_out_fd1, old_in_fd1, new_in_fd1);
			
			
			int old_in_fd;
			int old_out_fd;
			int pfds[2];
			pipe(pfds);
			int status;
		  	pid_t pid = fork();
			//child pid	
			if(pid == 0){
				//runs execvp on child
				//if sucessful returns 1 else 0
				old_in_fd = dup(1);
				dup2(pfds[1], 1);
				close(pfds[0]);
				left->execute();
				close(pfds[1]);
				dup2(old_in_fd, 1);
				exit(0);
				
				// if(execvp(args_arr[0],args_arr) == -1){
				// 	perror("exec");
				// 	exit(1);
				// }	
				// else{
				// 	exit(0);
				// }
			}
			//adult pid
			else if (pid > 0){
				//waits for childs return
				wait(&status);
				// if not -1 error in wait
				if (wait(&status) != -1){
					perror("wait");
				}
				//WIFEXITED means child returned normally
				if(WIFEXITED(status)){
					// if 0 child run of execvp was successful
					// so itr is true
					if (WEXITSTATUS(status) == 0){
						old_in_fd = dup(0);
						dup2(pfds[0], 0);
						close(pfds[1]);
						right->execute();
						close(pfds[0]);
						dup2(old_in_fd, 0);
						this->end_redirection(old_out_fd1, new_out_fd1, old_in_fd1, new_in_fd1);
						return true;
					}
					//else child run of execvp was incorrect so
					//it ran uncessfully
					//itr is returned false
					else{
						this->end_redirection(old_out_fd1, new_out_fd1, old_in_fd1, new_in_fd1);
						return false;
					}
				}
			}

			
			
			// bool rtn;
			// int pfds[2];
			// pipe(pfds);
			// pid_t pid;
			// int status;
			
			// pid = fork();
			// int old_in_fd;
			// int old_out_fd;
			
			// if(pid == 0){
			// 	old_in_fd = dup(1);
			// 	dup2(pfds[1], 1);
			// 	close(pfds[0]);
			// 	left->execute();
			// 	close(pfds[1]);
			// 	dup2(old_in_fd, 1);
			// 	return true;
			// }
			// else{
				
			// 	if(WIFEXITED(status)){
			// 		// if 0 child run of execvp was successful
			// 		// so itr is true
			// 		wait(&status);
			// 		if (WEXITSTATUS(status) == 0){
			// 			old_in_fd = dup(0);
			// 			dup2(pfds[0], 0);
			// 			close(pfds[1]);
			// 			right->execute();
			// 			close(pfds[0]);
			// 			dup2(old_in_fd, 0);
			// 			return true;
			// 		}
			// 		//else child run of execvp was incorrect so
			// 		//it ran uncessfully
			// 		//itr is returned false
			// 		else{
			// 			return false;
			// 		}
			// 	}
			// }
            
		 //   //cout << "execute Or" << endl;
		 //   return true;
		 
		}
		void setRightChild(Command* r){this->right = r;}
};
   


#endif