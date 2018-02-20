#include <iostream>
#include "command.h"
#include <vector>
#include <boost/tokenizer.hpp>
#include <string>
#include <sstream>
using namespace std;

//dispay string in each command*
//used for testing
void display(vector<cmd*> & v){
	for(unsigned i = 0; i < v.size(); i++){
		cout << v.at(i) -> showstring() << endl;	
	}
}

//displays each string
//also used for testing
void display(vector<string> v){
	cout << "vector: ";
	for(unsigned i = 0; i < v.size(); ++i){
		cout << v.at(i) << " ";
	}
	cout << endl;
}

//tokenizer didn't work as intended so i made my own parsing system
//Not nearly as efficient, will still seek to make tokenizer work in future 
//returns cmd* vector to the first cmd vector for evaluation
vector<cmd*> tokenized(string& in){
	vector<string> temp;
	istringstream s1(in);
	string through;
	//used string stream to remove spaces
	//along with creation of vector
	while(s1 >> through){
		temp.push_back(through);
	}
	//first iteration seperates ";","#" and '"' from the strings in the vector and makes them their own individual string in the vector
	//vector representation: [ls] [-la] [;] [echo] ["] [hello] [&&] [goodbye] ["]. each [] denotes vector element
	for(unsigned i = 0; i < temp.size(); i++){
		if(temp.at(i).find(";") != string::npos || temp.at(i).find("#") != string::npos || temp.at(i).find("\"") != string::npos){
			//splits if at the end of a string [ls] [-la;] becomes [ls] [-la] [;]	
			if(temp.at(i) != ";" && temp.at(i).find("#") == string::npos && temp.at(i).find(";") == temp.at(i).size() - 1
			 || temp.at(i).find("\"") == temp.at(i).size() - 1 ){
				string split = temp.at(i).substr(0,temp.at(i).size() - 1);
				string colon = temp.at(i).substr(temp.at(i).size() - 1, temp.at(i).size());
				temp.at(i) = colon;
				temp.insert(temp.begin() + i , split);
				++i;
			}
			//this splits if at the end of a string [ls] [#-la] becomes [ls] [#] [-la] 
			//semicolon not checked since it would be at the beginning of a string
			else if ((temp.at(i).find("#") == 0 && temp.at(i) != "#") || (temp.at(i).find("\"") == 0 && temp.at(i) != "\'")){
				string hash = temp.at(i).substr(0,1);
				string sep = temp.at(i).substr(1,temp.at(i).size());
				temp.at(i) = sep;
				temp.insert(temp.begin() + i, hash);
				++i;
			}
		}
	}
	//creat cmd* vector
	vector<cmd* > full;
	//buildstring will now build the full commands once again
	string buildstring = "";
	//newstring determines if the string is newly created or blank
	bool newstring = true;
	//iterates through vector to determine if connector or executible
	for(unsigned i = 0; i < temp.size(); ++i){
			//for ["] i just have it ignore other rules and continously input to string until other " is found
			//even if && || or ;
			if(temp.at(i) == "\""){
				buildstring = buildstring + " ";
				++i;
				while(temp.at(i) != "\""){
					buildstring = buildstring + temp.at(i);
					if (temp.at(i + 1) != "\""){
						buildstring = buildstring + " ";
					}
					++i;
				}
				if(i + 1 < temp.size()){
					++i;
				}
			}
			//detects ; && || or # or end of vector (its like a soft connector)
			if(temp.at(i) == ";" || temp.at(i) == "&&" || temp.at(i) == "||"|| i == temp.size() - 1 || temp.at(i) == "#"){
				// creates executible without connector since it is the last element	
				if(i == temp.size() - 1){
					if(temp.at(i) != "\""){
						if(!newstring){
							buildstring = buildstring + " ";
						}
							buildstring = buildstring + temp.at(i);
					}
							full.push_back(new ex(buildstring));
					
				}
				// other wise if creates the executible then detects what connector it is and creates a connector of that string
				else{
					full.push_back(new ex(buildstring));
					if(temp.at(i) == ";"){
						full.push_back(new connector(";"));
					}
					else if(temp.at(i) == "&&"){
						full.push_back(new connector("&&"));
					}
					else if(temp.at(i) == "||") {
						full.push_back(new connector("||"));
					}
					else if (temp.at(i) == "#"){
						full.push_back(new connector("#"));
					}
					buildstring = "";
					newstring = true;
				}
			}
			//if it doesnt detect a connector it will just keep on building on a string
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
	while(true){	
		string input;
		//output prompt and take in input
		cout << "Rshell # ";
		getline(cin,input);
		//uses tokenizer function to create a list of commands
		vector<cmd*> command = tokenized(input);
		bool run = true;
		//lastly it iterates through the vector and executes the command
		for(unsigned i = 0; i < command.size(); ++i){
			// # means the other strings in the vector are comments so it stops
			// the current iteration but continues the program
			if(command.at(i) -> showstring()  == "#"){
				break;
			}
			// exit denotes it is done with the whole program so it
			// exits
			else if(command.at(i) -> showstring() == "exit"){
				cout << "exiting rshell" << endl;
				exit(0);
			}
			// it runts the execute command on the command or connector class
			else{
				command.at(i) -> execute(run);
			}
		}
	}
return 0;
}
