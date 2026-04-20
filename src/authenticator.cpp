#include "authenticator.h"
Authenticator::Authenticator(Server* server)
	:server(server), fromUser(server->getFromUser()), toUser(server->getToUser()) {
	failCount = 0;
}

int Authenticator::startProcess() {
	if (failCount >= MAX_FAIL) {
		toUser << "You have made too many attempts!\nPermission denied.\n\n";
		return 1;
	}
	toUser << "Greetings!\n";

	std::string corPassword = "";

	if (identification(&corPassword)) {
		toUser << "Ошибка идентификации!\n";
		return 2;
	}
	if (authentication(corPassword)) {
		toUser << "Ошибка аутентификации!\n";
		return 3;
	}
	return authorization();
}
int Authenticator::identification(std::string* corPassword) {
	char* errmsg = 0;
	std::string sql = "";
	std::string login = "";

	while (*corPassword == "") {
		if (login != "")
			toUser << "Неверный логин!\nПожалуйста повторите попытку...\n\n";

		toUser << "login: ";
		fromUser >> login;
		fromUser.ignore(100, '\n');

		sql = "SELECT password FROM Users WHERE login='" + login + "'";

		int code = server->dbRequest(
			sql.c_str(),
			[](void* vres, int n, char** val, char** head) {
				std::string* res = (std::string*)vres;
				(*res) = val[0];
				return 0;
			},
			corPassword);

		if (code)
			return code;
	}
	return 0;
}

int Authenticator::authentication(const std::string& corPassword) {
	std::string password = "";

	while (password != corPassword) {
		if (password != "") {
			failCount++;
			toUser << "Неверный пароль!\n";
			if (failCount >= MAX_FAIL) {
				toUser << "У вас не осталось попыток!\n\n";
				return 1;
			}
			else {
				toUser << "У вас осталось " << MAX_FAIL - failCount << " попыток.\n\n";
			}
		}
		toUser << "password: ";
		fromUser >> password;
		fromUser.ignore(100, '\n');
	}
	return 0;
}


int Authenticator::authorization() {
	toUser << "Вход в систему выполнен!\n";
	failCount = 0;

	return server->startProcess();
}
