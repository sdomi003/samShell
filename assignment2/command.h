#include <iostream>
#include <string>
#include <vector>

using namespace std;

class cmd{
	protected:
	string str;
	bool AlreadyRun;
	public:
	cmd(){};
	cmd(string element);
	virtual bool execute(bool&) = 0;
	virtual string showstring() = 0;
};

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

