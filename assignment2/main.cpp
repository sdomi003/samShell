#include <iostream>
//#include "command.h"
#include <vector>
#include <boost/tokenizer.hpp>
#include <string>
#include <sstream>
using namespace std;

// This class converts the inputted string into a vector of commands
// Namespace boost will only apply here since this will be the only area where
// boost will be used. To prevent possible inability to be translated
 
//vector<cmd> tokenized(string& in){
void tokenized(string& in){
	using namespace boost;
	vector<string> temp;
	istringstream s1(in);
	string through;
	while(s1 >> through){
		temp.push_back(through);
	}
	for(unsigned i = 0; i < temp.size(); i++){
		cout << temp.at(i) << endl;
	}
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
