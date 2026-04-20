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

	int identification(std::string* corPassword);
	int authentication(const std::string& corPassword);
	int authorization();

public:
	Authenticator(Server* server);

	int startProcess();
};

#endif
