#ifndef COMPOSITES_H
#define COMPOSITES_H
#include <iostream>
#include <string>
#include <vector>
#include "Command.h"
using namespace std;

class Semi : public Command{
	Command* left;
	Command* right;
	 
	
	public:
		Semi(Command* l, Command* r){this->left = l; this->right = r;  this->in_file = ""; this->out_file = ""; this->redirection_type = -1;}
		bool execute(){
		    left->execute(); 
		    //cout << "execute semi" << endl;
		    return (right->execute());
		}
		void setRightChild(Command* r){this->right = r;}
};

class And : public Command{
	Command* left;
	Command* right;
	 
	
	public:
		And(Command* l, Command* r){this->left = l; this->right = r;  this->in_file = ""; this->out_file = ""; this->redirection_type = -1;}
		bool execute(){
			// case where we have ">"
			
			
			if(redirection_type == 0){
				if(left->execute()){
					int old_out_fd = dup(1);
					int new_out_fd = open(out_file.c_str(), O_WRONLY | O_TRUNC);
					dup2(new_out_fd, 1);
					bool rtn = right->execute();
					close(new_out_fd);
					dup2(old_out_fd, 1);
					return rtn;
				}
			}
			else if(redirection_type == 1){
				if(left->execute()){
					int old_out_fd = dup(1);
					int new_out_fd = open(out_file.c_str(), O_WRONLY | O_APPEND);
					dup2(new_out_fd, 1);
					bool rtn = right->execute();
					close(new_out_fd);
					dup2(old_out_fd, 1);
					return rtn;
				}
			}
			//case where we don't change output or input. Could still be
			// output to different souce but that's on the parent to 
			// implement in this tree
			else{
				if(left->execute()){
		    		return (right->execute());
		    	} 
		    	return false;
			}

		    //cout << "execute And" << endl;
		    
		}
		void setRightChild(Command* r){this->right = r;}
};

class Or : public Command{
	Command* left;
	Command* right;
	public:
		Or(Command* l, Command* r){this->left = l; this->right = r;  this->in_file = ""; this->out_file = ""; this->redirection_type = -1;}
		bool execute(){
			if(!left->execute()){
				return (right->execute());
			}
		    //cout << "execute Or" << endl;
		    return true;
		 
		}
		void setRightChild(Command* r){this->right = r;}
};
   


#endif