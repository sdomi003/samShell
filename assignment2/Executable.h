#ifndef EXECUTABLE_H
#define EXECUTABLE_H

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


using namespace std;  

class Executable : public Command{
string args; 

public:
	Executable(string a){this->args = a;}
	//idea, make execute of type bool to return whether or not it worked
	bool execute(){
		// split string into tokenS
	  
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
	  //cout << args_arr[0] << endl;
	  
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
					return itr;
				}
				//else child run of execvp was incorrect so
				//it ran uncessfully
				//itr is returned false
				else{
					itr = false;
					return itr;
				}
			}
		}

	  delete[] cstr;
		//test
	  return true;
	}
	void setRightChild(Command* h){cout << "error, tried to set right child of executable" << endl;}
};

#endif