#ifndef AUTHENTIFICATOR
#define AUTHENTIFICATOR
#include <iostream>
#include "server.h"
class Authenticator{
	const int MAX_FAIL = 4;
	Server* server;
	
	std::istream& fromUser;
	std::ostream& toUser;

	int failCount;

	void identification(std::string* corPassword);
	void authentication(std::string& corPassword, bool* success);
	void authorization();


public:
	Authenticator(Server* server);


	void startProcess();
};

#endif
