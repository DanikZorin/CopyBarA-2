#include <iostream>
#include "authenticator.h"
using namespace std;

int main(int argc, char** args){
	
	Server server;
	Authenticator auth(&server);

	if (auth.startProcess()) {
		std::cout << "Не удалось войти в систему.\n";
	}
	std::cout << "Завершение работы...\n";
}
