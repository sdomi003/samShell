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
		Semi(Command* l, Command* r){this->in_fd = 0; this->out_fd = 0; this->left = l; this->right = r;  this->in_file = ""; this->out_file = ""; this->redirection_type = -1;}
		bool execute(int in_fd, int out_fd){
			int old_out;
		    left->execute(0,0); 
		    //cout << "execute semi" << endl;
		    if(redirection_type != -1){
				// for now, redirect is 0 or 1
				// test case
				// clean .txt
				if(redirection_type == 0 || redirection_type == 1){
					cout << "outfile before clean:" << out_file <<":" << endl; 
					int first_char = 0;
					int last_char = out_file.size() - 1;
					while(out_file.at(first_char) == ' '){
					  ++first_char;
					}
					while(out_file.at(last_char) == ' '){
					  --last_char;
					}
					out_file = out_file.substr(first_char, last_char); // +1?
					// done cleaning .txt
					cout << "cleaned:" << out_file << ":" << endl;
					
					old_out = dup(1);
					if(redirection_type == 0){
						out_fd = open(out_file.c_str(), O_WRONLY | O_TRUNC);
					}
					else{
						out_fd = open(out_file.c_str(), O_WRONLY | O_APPEND);
					}
					cout << "special out_fd: " << out_fd << endl;
					dup2(out_fd, 1);	
				}
			}
			bool rtn = right->execute(0,0);
			if(redirection_type != -1){
	    		close(out_fd);
	    		dup2(old_out, 1);
	    		close(old_out);
	    	}
			
		    return rtn;
		}
		void setRightChild(Command* r){this->right = r;}
};



class And : public Command{
	Command* left;
	Command* right;
	public:
		And(Command* l, Command* r){this->in_fd = 0; this->out_fd = 0; this->left = l; this->right = r;  this->in_file = ""; this->out_file = ""; this->redirection_type = -1;}
		bool execute(int in_fd, int out_fd){
			// get dups done
			int old_out;
			
		    if(left->execute(0,0)){
		    	if(redirection_type != -1){
					// for now, redirect is 0 or 1
					// test case
					// clean .txt
					if(redirection_type == 0 || redirection_type == 1){
						cout << "outfile before clean:" << out_file <<":" << endl; 
						int first_char = 0;
						int last_char = out_file.size() - 1;
						while(out_file.at(first_char) == ' '){
						  ++first_char;
						}
						while(out_file.at(last_char) == ' '){
						  --last_char;
						}
						out_file = out_file.substr(first_char, last_char); // +1?
						// done cleaning .txt
						cout << "cleaned:" << out_file << ":" << endl;
						
						old_out = dup(1);
						if(redirection_type == 0){
							out_fd = open(out_file.c_str(), O_WRONLY | O_TRUNC);
						}
						else{
							out_fd = open(out_file.c_str(), O_WRONLY | O_APPEND);
						}
						cout << "special out_fd: " << out_fd << endl;
						dup2(out_fd, 1);	
					}
				}
		    	bool itr = right->execute(0,0);
		    	if(redirection_type != -1){
		    		close(out_fd);
		    		dup2(old_out, 1);
		    		close(old_out);
		    	}
		    	return itr;
		    } 
		    // must do the same for false case
		    return false;
		    //cout << "execute And" << endl;
		    
		}
		void setRightChild(Command* r){this->right = r;}
};



class Or : public Command{
	Command* left;
	Command* right;
	public:
		Or(Command* l, Command* r){this->in_fd = 0; this->out_fd = 0; this->left = l; this->right = r; this->in_file = ""; this->out_file = ""; this->redirection_type = -1;}
		bool execute(int in_fd, int out_fd){
			int old_out;
			bool left_success = left->execute(0,0);
			if(!left_success){
				if(redirection_type != -1){
				// for now, redirect is 0 or 1
				// test case
				// clean .txt
					if(redirection_type == 0 || redirection_type == 1){
						cout << "outfile before clean:" << out_file <<":" << endl; 
						int first_char = 0;
						int last_char = out_file.size() - 1;
						while(out_file.at(first_char) == ' '){
						  ++first_char;
						}
						while(out_file.at(last_char) == ' '){
						  --last_char;
						}
						out_file = out_file.substr(first_char, last_char); // +1?
						// done cleaning .txt
						cout << "cleaned:" << out_file << ":" << endl;
						
						old_out = dup(1);
						if(redirection_type == 0){
							out_fd = open(out_file.c_str(), O_WRONLY | O_TRUNC);
						}
						else{
							out_fd = open(out_file.c_str(), O_WRONLY | O_APPEND);
						}
						cout << "special out_fd: " << out_fd << endl;
						dup2(out_fd, 1);	
					}
				}
				bool rtn = right->execute(0,0);
				
				if(redirection_type != -1){
		    		close(out_fd);
		    		dup2(old_out, 1);
		    		close(old_out);
		    	}
				
				return rtn;
			}
			
			
		    return true;
		 
		}
		void setRightChild(Command* r){this->right = r;}
};
   


#endif