#ifndef SERVER
#define SERVER
#include <iostream>
#include <winsqlite/winsqlite3.h>
#include <vector>
#include <string>
#include "date.h"
struct CallbackParam { 
	int responseCount; 
	std::ostream& toUser;

	CallbackParam(std::ostream& toUser) :toUser(toUser), responseCount(0) {};
};

int callbackPrintout(void* smth, int argc, char** val, char** header);
int callbackList(void* smth, int argc, char** val, char** header);
class Server{
	const char* dbPath = "Workshops_system.db";
	sqlite3* db;

	std::istream& fromUser;
	std::ostream& toUser;


	void showHelp();
	void listTables();
	void showTable(const std::string& table);
	void unknownCommand();

	void task1();
	void task2();
	void task3();
	void task4();
	void task5();
public:
	Server(std::ostream& toUser=std::cout, std::istream& fromUser=std::cin);
	
	int dbSelect(const char* sql, int (*callback)(void* par, int n, char** vals, char** names));
	int dbRequest(const char* sql, int (*callback)(void* par, int n, char** vals, char** names), void* par);

	std::ostream& getToUser() const;
	std::istream& getFromUser() const;

	void startProcess();
};

#endif