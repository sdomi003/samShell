#ifndef COMMAND_H
#define COMMAND_H
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// cmd is base class with virtual void showstring and execute
class cmd{
	protected:
	string str;
	bool AlreadyRun;
	public:
	~cmd(){delete this;};
	cmd(){};
	cmd(string element);
	virtual bool execute(bool&) = 0;
	virtual string showstring() = 0;
};
// both these classes implement them
class ex: public cmd {
	public:
	ex(string element);
	bool execute(bool&);	
	string showstring();
};

class connector: public cmd {
	public:
	connector(string element);
	bool execute(bool&);
	string showstring();
};

#endif //COMMAND_H
