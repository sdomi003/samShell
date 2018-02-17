#include"command.h"
using namespace std;

cmd::cmd(string element){
	this -> str = element;
}

bool ex::execute(bool& itr){
	cout << "this is an executible" << endl;
	itr = true;
	return itr;
}

bool connector::execute(bool& itr){
	cout << "This is a connector" << endl;
	itr = true;
	return itr;
}

ex::ex(string element){
	this -> str = element;
}

connector::connector(string element){
	this -> str = element;
}

string ex::showstring(){
	return str;
}

string connector::showstring(){
	return str;
}
