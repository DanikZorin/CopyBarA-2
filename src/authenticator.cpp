#include "authenticator.h"
Authenticator::Authenticator(Server* server)
	:server(server), fromUser(server->getFromUser()), toUser(server->getToUser()) {
	failCount = 0;
}

void Authenticator::startProcess(){
	if (failCount >= MAX_FAIL) {
		toUser << "You have made too many attempts!\nPermission denied.\n\n";
		return;
	}
	toUser << "Greetings!\n";
	
	std::string corPassword = "";
	std::string login = "";
	identification(&corPassword);

	while (corPassword == "") {
		toUser << "Invalid login!\nPlease try again...\n\n";
		identification(&corPassword);
	}

	bool success;
	authentication(corPassword, &success);
	
	
	while (!success) {
		failCount++;
		if (failCount >= MAX_FAIL) {
			toUser << "You have made too many attempts!\nPermission denied.\n\n";
			return;
		}
		toUser << "Incorrect Password!\nYou have " << MAX_FAIL - failCount << " attempt(s) left.\n\n";
		authentication(corPassword, &success);
	}

	authorization();
}

void Authenticator::identification(std::string* corPassword){
	char* errmsg = 0;
	char sql[100];


	std::string login;
	toUser << "login: ";
	fromUser >> login;
	fromUser.ignore(100, '\n');
	sprintf_s(sql, "SELECT password FROM Users WHERE login='%s';", login.c_str());
	server->dbRequest(
		sql, 
		[](void* vres, int n, char** val, char** head) {
			std::string* res = (std::string*)vres;
			(*res) = val[0];
			return 0;
		},
		corPassword);
	
}

void Authenticator::authentication(std::string& corPassword, bool* success){
	std::string password;
	toUser << "password: ";
	fromUser >> password;
	fromUser.ignore(100, '\n');
	if (password != corPassword)
		(*success) = false;
	else
		(*success) = true;
}

void Authenticator::authorization(){
	toUser << "Success!\n";
	failCount = 0;

	server->startProcess();
}
