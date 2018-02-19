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
		
	  char* args_arr[args.length()+1];	
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
	  cout << args_arr[0] << endl;
	  
	  //start forking
	  pid_t pid = fork();
	  int status;
	  if(pid < 0){
	  	cout << "ERROR: RSHELL FORKING FAILED" << endl;
	  	exit(1);
		//return false;
	  }
	  else if(pid == 0){
	  	if(execvp(args_arr[0], args_arr) == -1){
	  		cout << "ERROR: NO argument found" << endl;
	  		//exit(1);
	  		return false;
	  	}
	  }
	  
	  	else{
	  		while (wait(&status) != pid);
	  		//pid_t waitpid(pid_t pid, int *status, int options);

	  	}
	  	//return true for execute function here
	  
	  
	  //DON'T DELETE CSTR UNTIL I'M DONE USING TOKENS
	  delete[] cstr;
		//test
	  return true;
	}
	void setRightChild(Command* h){cout << "error, tried to set right child of executable" << endl;}
};

#endif