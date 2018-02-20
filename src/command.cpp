#include"command.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sstream>
#include <netdb.h>
#include <sys/socket.h>
#include <pwd.h>
using namespace std;
//constructor simply has a string put into the classes string
cmd::cmd(string element){
	this -> str = element;
}
//executes the command for execute or ex class
bool ex::execute(bool& itr){
	//if its true it runs
	if(itr == true){	
		istringstream s2(this -> str);
		vector<string> order;
		string in;
		int status;
		//splits by spaces once again
		while(!s2.eof()){
			s2 >> in;
			//places strings in vector for execution
			order.push_back(in);
		}
		//then creates char* array to use for
		//execvp
		//set to 100 for werror	
		char* args [100];
		// assigns each character array to
		// character pointer array
		for(unsigned i = 0; i < order.size(); ++i){
			args[i] = (char*)order.at(i).c_str(); 
		}
		//lastly we put null if not command will
		//not execute
		args[order.size()] = NULL;
		//fork to run execvp on child
		pid_t pid = fork();
		//child pid	
		if(pid == 0){
			//runs execvp on child
			//if sucessful returns 1 else 0
			if(execvp(args[0],args) == -1){
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
		//error if pid < 0
		else if (pid < 0){
			perror("fork");
			exit(1);
		}
	}
		//else case
		//it returns true
		itr = true;
		return itr;
	

}
// connector execute
bool connector::execute(bool& itr){
	//flipts itr since if successful the next command
	//doesnt have to run and if not next command has to
	if(this -> str == "||"){
		itr = !itr;
	}
	//; commands always have to run
	if(this -> str == ";"){
		itr = true;
	}
	//and does not modify since if previous command
	//failed its aready false and other command
	//wont run
	return itr;
}

//string constructors
ex::ex(string element){
	this -> str = element;
}

//string constructors
connector::connector(string element){
	this -> str = element;
}

//simply shows string inside str
string ex::showstring(){
	return str;
}

//simply shows string inside str
string connector::showstring(){
	return str;
}
