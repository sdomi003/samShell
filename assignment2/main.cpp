#include <iostream>
//#include "command.h"
#include <vector>
#include <boost/tokenizer.hpp>
#include <string>
#include <sstream>
using namespace std;

void display(vector<string> & v){
	for(unsigned i = 0; i < v.size(); i++){
		cout << v.at(i) << endl;	
	}
}
 
//vector<cmd> tokenized(string& in){
void tokenized(string& in){
	vector<string> temp;
	istringstream s1(in);
	string through;
	while(s1 >> through){
		temp.push_back(through);
	}
	vector<string> full;
	string complete = "";
	for(unsigned i = 0; i < temp.size(); i++){
		if(temp.at(i).find(";") != string::npos){
			string split = temp.at(i).substr(0,temp.at(i).size() - 1);
			string colon = temp.at(i).substr(temp.at(i).size() - 1, temp.at(i).size());
			temp.at(i) = colon;
			temp.insert(temp.begin() + i , split);
			++i;
		}
	}
	display(temp);
}


int main(){
//while(true){
	vector<string> command;
	string input;
	cout << "$";
	getline(cin,input);
	//vector<cmd> command = tokenized(Input);
	tokenized(input);
//}
return 0;
}
