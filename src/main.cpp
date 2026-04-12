#include <iostream>
#include "authenticator.h"
using namespace std;

int callback(void* smth, int argc, char** val, char** header) {
	for (int i = 0;i < argc;i++) {
		printf("%s - %s\n", header[i], val[i]);
	}
	printf("\n");
	return 0;
}
/// TODO remove windows and test on macOS
#include <windows.h>
int main(int argc, char** args){
	
	SetConsoleOutputCP(65001);
	SetConsoleCP(65001);
\
	Server* server = new Server();
	Authenticator* auth = new Authenticator(server);

	auth->startProcess();
	
	delete auth;
	delete server;
}
