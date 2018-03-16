
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
#include <sys/wait.h>
#include <cstddef>

#include <sys/types.h>
#include <sys/stat.h>

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <fcntl.h>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <cstddef>

#include <sys/types.h>
#include <sys/stat.h>
using namespace std;  

class Executable : public Command{
string args; 

public:
	Executable(string a){this->args = a; this->in_fd = 0; this->out_fd = 0; this->in_file = ""; this->out_file = "";}
	//idea, make execute of type bool to return whether or not it worked
	bool execute(int in_fd, int out_fd){
		// set standard input and output right now
		// if file is not empty, open it and set it using dup2
		// if file is empty, check fd passed in as params
		// if they are not 0, then set them as standard
		// if they are 0, do nothing
		
		// first handle input
		// STOP HERE FOR NOW, 12:35 pm
		int old_out_fd;
		// need to save old out file descriptor and put it back in at the end of execute
		old_out_fd = dup(1);
		if(!out_file.empty()){
			// clean infile 
			cout << "entered!" << endl;
			int first_char = 0;
      int last_char = out_file.size() - 1;
      while(out_file.at(first_char) == ' '){
      	++first_char;
      }
      while(out_file.at(last_char) == ' '){
      	--last_char;
      }
      out_file = out_file.substr(first_char, last_char + 1);
			// outfile is now cleaned
			out_fd = open(out_file.c_str(), O_WRONLY | O_TRUNC);
			cout << "out fd: " << out_fd << endl;
			dup2(out_fd,1);
			// out works when I DON"T close out_fd
			//close(out_fd);
		}
		// STOP
		// finished setting up standard in and out
		
		// split string into tokenS
		
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
      
      // begin cleaning args
      int first_char = 0;
      int last_char = args.size() - 1;
      while(args.at(first_char) == ' '){
      	++first_char;
      }
      while(args.at(last_char) == ' '){
      	--last_char;
      }
      args = args.substr(first_char, last_char + 1);
      
      // args now contains cleaned string
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
	  	struct stat sb;
	  	if(stat(args_arr[0], &sb) == -1){
	        cout << "(false)" << endl;
	        close(out_fd);
	        dup2(old_out_fd, 1);
	        close(old_out_fd);
	        return false;
	    }
	    else if(flag == 'd'){
	    	if(S_ISDIR(sb.st_mode)){
	    		cout << "(true)" << endl;
	    		close(out_fd);
	    		dup2(old_out_fd, 1);
	    		close(old_out_fd);
	    		return true;
	    	}
	    	else{
	    		cout << "(false)" << endl;
	    		close(out_fd);
	    		dup2(old_out_fd, 1);
	    		close(old_out_fd);
	    		return false;
	    	}
	    } 
	    else if(flag == 'f'){
	        if(S_ISREG(sb.st_mode)){
	        	cout << "(true)" << endl;
	        	close(out_fd);
	        	dup2(old_out_fd, 1);
	        	close(old_out_fd);
	        	return true;
	        }
	        else{
	        	cout << "(false)" << endl;
	        	close(out_fd);
	        	dup2(old_out_fd, 1);
	        	close(old_out_fd);
	        	return false;
	        }
	    }
	    else if(flag == 'n' || flag == 'e'){
	    	//file simply exists
	    	cout << "(true)" << endl;
	    	close(out_fd);
	    	dup2(old_out_fd, 1);
	    	close(old_out_fd);
	    	return true;
	    }
	  }
	  else{
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
						close(out_fd);
						dup2(old_out_fd, 1);
						close(old_out_fd);
						return itr;
					}
					//else child run of execvp was incorrect so
					//it ran uncessfully
					//itr is returned false
					else{
						itr = false;
						close(out_fd);
						dup2(old_out_fd, 1);
						close(old_out_fd);
						return itr;
					}
				}
			}
	  }
	
	  delete[] cstr;
		//test
		close(out_fd);
		dup2(old_out_fd, 1);
		close(old_out_fd);
	  return true;
	}
	
	void setRightChild(Command* h){cout << "error, tried to set right child of executable" << endl;}
};

#endif  