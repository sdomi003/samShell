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
   


#endif