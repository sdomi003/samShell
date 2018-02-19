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
		Semi(Command* l, Command* r){this->left = l; this->right = r;}
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
		And(Command* l, Command* r){this->left = l; this->right = r;}
		bool execute(){
		    if(left->execute()){
		    	return (right->execute());
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
		Or(Command* l, Command* r){this->left = l; this->right = r;}
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