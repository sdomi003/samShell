#ifndef EXECUTABLE_H
#define EXECUTABLE_H

#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include "Command.h"



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
	  // args now parsed


	  delete[] cstr;
		//test
	  return true;
	}
	void setRightChild(Command* h){cout << "error, tried to set right child of test executable" << endl;}
};

#endif