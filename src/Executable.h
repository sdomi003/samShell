#ifndef EXECUTABLE_H
#define EXECUTABLE_H
// FIX INCLUDES
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

class Executable : public Command{
string args; 

public:
	Executable(string a){this->args = a; this->in_file = ""; this->out_file = ""; this->redirection_type = -1;}
	//idea, make execute of type bool to return whether or not it worked
	bool execute(){
		// split string into tokenS
		int old_out_fd = -1;
		int new_out_fd = -1;
		
		if(redirection_type == 0){ // or if both in and out are to be set?
			old_out_fd = dup(1);
			new_out_fd = open(out_file.c_str(), O_WRONLY | O_TRUNC);
			dup2(new_out_fd, 1);
		}
		else if(redirection_type == 1){
			old_out_fd = dup(1);
			new_out_fd = open(out_file.c_str(), O_WRONLY | O_APPEND);
			dup2(new_out_fd, 1);
		}
		
		
		
	  bool is_test = false;
	  char flag = 'e';
	  // cout << "args: " << args << endl; 
	  // before we tokenize, let's check if the first word is "test" or if the first character is '['
      //	if true, we set is_test to true
      // case where we have "  [ hi/wer ]" 
      // we have to account for spaces in the beginning. also, we should only tokenize until the character before the last, 
      
      // make args string have no extra spaces before or after
      
      
      //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // finish parser
      // need to finish what happens if [ is found
      // going to make another substr where [ and ] are not included
      // then check for flag
      // the goal is for the tokenizeer to ONLY tokenize the filepath
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      
      int first_char = 0;
      int last_char = args.size() - 1;
      while(args.at(first_char) == ' '){
      	++first_char;
      }
      while(args.at(last_char) == ' '){
      	--last_char;
      }
      args = args.substr(first_char, last_char + 1);
      if(args == "exit" || args == "quit"){
      	throw "exiting";
      }
      
      
      // test to see if first char is '['
      // also to see if first word is "test"
      size_t found;
      if(args.at(0) == '['){
      	is_test = true;
      	args = args.substr(2, args.size() - 3);
      	
      	found = args.find_first_of("-");
      	if(found != string::npos){
      		flag = args.at(found+1);
      		args = args.substr(found + 2, args.size() - found - 2);
      	}
      	else{
      		flag = 'e';
      	}
      }
      else if(args.compare(0, 5, "test ") == 0){			// this means that the strings are equal
        is_test = true;
      	args = args.substr(5, args.size() - 5);
      	found = args.find_first_of("-");
      	if(found != string::npos){
      		flag = args.at(found+1);
      		args = args.substr(found + 2, args.size() - found - 2);
      	}
      	else{
      		flag = 'e';
      	}
 
      }
      // begin tokenize
	  char* args_arr[100];	
	  char * cstr = new char [args.length()+1];
	  std::strcpy (cstr, args.c_str());
		
		// cstr now contains a c-string copy of str
		
	  	
	  char * p = std::strtok (cstr," ");
	  int i = 0;
	  while (p!=0)
	  {
	  	args_arr[i] = p;
		p = std::strtok(NULL," ");
		++i;
	  }
	  args_arr[i] = NULL;
	  // args_arr now holds what we want it to hold
	  
	  if(is_test){
	  	bool test_bool;
	  	struct stat sb;
	  	if(stat(args_arr[0], &sb) == -1){
	        cout << "(false)" << endl;
	        test_bool = false;
	    }
	    else if(flag == 'd'){
	    	if(S_ISDIR(sb.st_mode)){
	    		cout << "(true)" << endl;
	    		test_bool = true;
	    	}
	    	else{
	    		cout << "(false)" << endl;
	    		test_bool = false;
	    	}
	    } 
	    else if(flag == 'f'){
	        if(S_ISREG(sb.st_mode)){
	        	cout << "(true)" << endl;
	        	test_bool = true;
	        }
	        else{
	        	cout << "(false)" << endl;
	        	test_bool = false;
	        }
	    }
	    else if(flag == 'n' || flag == 'e'){
	    	cout << "(true)" << endl;
	    	test_bool = true;
	    }
		if(redirection_type == 0 || redirection_type == 1){
			close(new_out_fd);
			dup2(old_out_fd, 1);
		}
		return test_bool;
	  }
	  else{
	  
	  //COPY
		  int status;
		    bool itr;
		  	pid_t pid = fork();
			//child pid	
			if(pid == 0){
				//runs execvp on child
				//if sucessful returns 1 else 0
				if(execvp(args_arr[0],args_arr) == -1){
					perror("exec");
					exit(1);
				}	
				else{
					exit(0);
				}
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
						itr = true;
						if(redirection_type == 0 || redirection_type == 1){
							close(new_out_fd);
							dup2(old_out_fd, 1);
						}
						return itr;
					}
					//else child run of execvp was incorrect so
					//it ran uncessfully
					//itr is returned false
					else{
						itr = false;
						if(redirection_type == 0 || redirection_type == 1){
							close(new_out_fd);
							dup2(old_out_fd, 1);
						}
						return itr;
					}
				}
			}
	  }
	
	  delete[] cstr;
		//test
		if(redirection_type == 0){
			close(new_out_fd);
			dup2(old_out_fd, 1);
		}
	  return true;
	}
	
	void setRightChild(Command* h){cout << "error, tried to set right child of executable" << endl;}
};

#endif  