#include <iostream>
#include "command.h"
#include <vector>
#include <boost/tokenizer.hpp>
#include <string>
#include <sstream>
using namespace std;

void display(vector<cmd*> & v){
	for(unsigned i = 0; i < v.size(); i++){
		cout << v.at(i) -> showstring() << endl;	
	}
}
 
vector<cmd*> tokenized(string& in){
	vector<string> temp;
	istringstream s1(in);
	string through;
	while(s1 >> through){
		temp.push_back(through);
	}
	for(unsigned i = 0; i < temp.size(); i++){
		if(temp.at(i).find(";") != string::npos){
			string split = temp.at(i).substr(0,temp.at(i).size() - 1);
			string colon = temp.at(i).substr(temp.at(i).size() - 1, temp.at(i).size());
			temp.at(i) = colon;
			temp.insert(temp.begin() + i , split);
			++i;
		}
	}
	vector<cmd* > full;
	string buildstring = "";
	bool newstring = true;
	for(unsigned i = 0; i < temp.size(); ++i){
			if(temp.at(i) == ";" || temp.at(i) == "&&" || temp.at(i) == "||"|| i == temp.size() - 1){
				if(i == temp.size() - 1){
					buildstring = buildstring + " ";
					buildstring = buildstring + temp.at(i);
					full.push_back(new ex(buildstring));
				}
				else{
					full.push_back(new ex(buildstring));
					if(temp.at(i) == ";"){
						full.push_back(new connector(";"));
					}
					else if(temp.at(i) == "&&"){
						full.push_back(new connector("&&"));
					}
					else {
						full.push_back(new connector("||"));
					}
					buildstring = "";
					newstring = true;
				}
			}
			else{
				if(newstring){
					buildstring = buildstring + temp.at(i);	
					newstring = false;	
				}
				else {
					buildstring = buildstring + " ";
					buildstring = buildstring + temp.at(i);	
				}
			}	
	}
	return full;
}


int main(){
//while(true){	
	string input;
	cout << "$";
	getline(cin,input);
	vector<cmd*> command = tokenized(input);
	display(command);
//}
return 0;
}
