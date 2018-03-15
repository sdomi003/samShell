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
		Semi(Command* l, Command* r){this->in_fd = 0; this->out_fd = 0; this->left = l; this->right = r;  this->in_file = ""; this->out_file = "";}
		bool execute(int in_fd, int out_fd){
		    left->execute(0,0); 
		    //cout << "execute semi" << endl;
		    return (right->execute(0,0));
		}
		void setRightChild(Command* r){this->right = r;}
};

class And : public Command{
	Command* left;
	Command* right;
	 
	
	public:
		And(Command* l, Command* r){this->in_fd = 0; this->out_fd = 0; this->left = l; this->right = r;  this->in_file = ""; this->out_file = "";}
		bool execute(int in_fd, int out_fd){
		    if(left->execute(0,0)){
		    	return (right->execute(0,0));
		    } 
		    return false;
		    //cout << "execute And" << endl;
		    
		}
		void setRightChild(Command* r){this->right = r;}
};

class Or : public Command{
	Command* left;
	Command* right;
	public:
		Or(Command* l, Command* r){this->in_fd = 0; this->out_fd = 0; this->left = l; this->right = r; this->in_file = ""; this->out_file = "";}
		bool execute(int in_fd, int out_fd){
			if(!left->execute(0,0)){
				return (right->execute(0,0));
			}
		    //cout << "execute Or" << endl;
		    return true;
		 
		}
		void setRightChild(Command* r){this->right = r;}
};
   


#endif