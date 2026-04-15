#define _CRT_SECURE_NO_WARNINGS
#include "../include/server.h"
void stringToWords(const std::string& string, std::vector<std::string>& words) {
	int pos1 = 0;
	while ((pos1 = string.find_first_not_of(" ", pos1)) != -1) {
		int pos2 = string.find_first_of(" ", pos1);

		words.push_back(string.substr(pos1, pos2 - pos1));

		pos1 = pos2;
	}
} 

int Server::dbSelect(const char* sql, int(*callback)(void* par, int n, char** vals, char** names)){
	char* errmsg = 0;

	CallbackParam par(toUser);

	 int code = dbRequest(sql, callback, &par);

	if (par.responseCount == 0) {
		toUser << "No Data!\n";
	}
	return code;
}

int Server::dbRequest(const char* sql, int (*callback)(void* par, int n, char** vals, char** names), void* par) {
	char* errmsg = 0;

	if (int code = sqlite3_exec(db, sql, callback, par, &errmsg)) {
		toUser << "Error! " << errmsg<<'\n';
		return code;
	}
	return 0;
}

Server::Server(std::ostream& toUser, std::istream& fromUser): toUser(toUser), fromUser(fromUser){
	sqlite3_open(dbPath, &db);
}

std::ostream& Server::getToUser() const {
	return toUser;
}
std::istream& Server::getFromUser() const {
	return fromUser;
}
int callbackGeneral(void* smth, int argc, char** val, char** header) {
	std::ostream& toUser = *(std::ostream*)smth;
	for (int i = 0;i < argc;i++) {
		toUser << header[i] << " - " << val[i] << '\n';
	}
	toUser << '\n';
	return 0;
}

int callbackPrintout(void* smth, int argc, char** val, char** header){
	CallbackParam* par = (CallbackParam*)smth;
	std::ostream& toUser = par->toUser;
	for (int i = 0;i < argc;i++) {
		toUser << header[i] << " - " << val[i] << '\n';
	}
	toUser << '\n';

	par->responseCount++;
	return 0;
}
int callbackList(void* smth, int argc, char** val, char** header) {
	CallbackParam* par = (CallbackParam*)smth;
	std::ostream& toUser = par->toUser;
	for (int i = 0;i < argc;i++) {
		toUser << val[i] << '\t';
	}
	toUser << '\n';

	par->responseCount++;
	return 0;
}
void Server::showHelp() {
	toUser <<
		"Available commands:\n"
		"\t-help or -?\t - list commands\n"
		"\t-sql [request]\t - custom SQL requst\n"
		"\t-show [table]\t - show table from database\n"
		"\t-tables	\t - list all tables from db\n"
		"\t-task1 or -1\t - perform task1\n"
		"\t-task2 or -2\t - perform task2\n"
		"\t-task3 or -3\t - perform task3\n"
		"\t-task4 or -4\t - perform task4\n"
		"\t-task5 or -5\t - perform task5\n"
		"\t-problem5 or -p5\t - perform problem5\n"
		"\t-problem6 or -p6\t - perform problem6\n"
		;
}
void Server::startProcess(){
	
	while (true) {
		toUser << "Waiting for request...\n";
		std::string req;
		std::vector<std::string> args;
		
		int argc = 0;
		
		do {
			std::getline(fromUser, req);

			stringToWords(req, args);
			argc = args.size();

		} while (argc == 0);


		std::string command = args[0];

		if (command == "-help" || command == "-?") {
			showHelp();
		}
		else if (command == "-sql" && argc > 1) { // Problem 3
			dbRequest(req.substr(command.size()).c_str(), callbackGeneral, &toUser);
			masterStatUpdate(); // Problem 4
		}
		else if (command == "-show" && argc > 1) {
			showTable(args[1]);
		}
		else if (command == "-tables") {
			listTables();
		}
		else if (command == "-task1" || command == "-1") {
			task1();
		}
		else if (command == "-task2" || command == "-2") {
			task2();
		}
		else if (command == "-task3" || command == "-3") {
			task3();
		}
		else if (command == "-task4" || command == "-4") {
			task4();
		}
		else if (command == "-task5" || command == "-5") {
			task5();
		}
		else if (command == "-problem5" || command == "-p5") {
			problem5();
		}
		else if (command == "-problem6" || command == "-p6") {
			problem6();
		}
		else {
			unknownCommand();
		}
	}
}
void Server::listTables() {
	const char *sql =	"SELECT name FROM sqlite_master "
						"WHERE type = 'table' AND name NOT LIKE 'sqlite%'";
	dbSelect(sql, callbackList);
	toUser << '\n';
}

void Server::showTable(const std::string& table) {
	std::string sql = "SELECT * FROM " + table+";";
	dbSelect(sql.c_str(), callbackPrintout);
}
void Server::unknownCommand() {
	toUser <<
		"Error: Unknown syntax...\n"
		"Please enter -? or -help to see command list\n";

}
void Server::task1() {
	toUser << "Введите номер автомастерской:\n";
	int workshopNum;
	fromUser >> workshopNum;
	fromUser.ignore();

	Date dateStart;
	Date dateEnd;
	
	try {
		toUser << "Введите дату начала (образ. 2020-01-24):\n";
		fromUser >> dateStart;

		toUser << "Введите дату окончания (образ. 2020-01-24):\n";
		fromUser >> dateEnd;
	}
	catch (int) {
		toUser << "Invalid syntax!\naborting...\n";
		return;
	}

	char sql[] =
		"SELECT Masters.name, Repairs.name, Orders.start_date, Orders.end_date  FROM Orders "
		"JOIN Masters ON Orders.master_id = Masters.id "
		"JOIN Repairs ON Orders.repair_id = Repairs.id "
		"WHERE Orders.workshop_id = ? " 
		"AND Orders.start_date > ? "
		"AND Orders.end_date < ? "
		"ORDER BY Masters.name "
		;


	sqlite3_stmt* stmt;

	if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0)) {
		toUser << "sqlite3_prepare_v2 ERROR!\n\n";
		return;
	}

	sqlite3_bind_int(stmt, 1, workshopNum);
	sqlite3_bind_text(stmt, 2, dateStart.toString().c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 3, dateEnd.toString().c_str(), -1, SQLITE_TRANSIENT);

	toUser
		<< "Мастер" << sep
		<< "Услуга" << sep 
		<< "Начало" << sep
		<< "Конец" << sep
		<< '\n';



	while (sqlite3_step(stmt) == SQLITE_ROW) {
		for (int i = 0;i < sqlite3_column_count(stmt);i++) {
			const unsigned char* val_ = sqlite3_column_text(stmt, i);
			std::string val = val_?(const char*) val_:"NULL";
			toUser << val << sep;
		}
		toUser << '\n';
	}
	sqlite3_finalize(stmt);
}
void Server::task2(){
	toUser << "Введите имя (фамилию/отчество) мастера:\n";
	std::string masterName;
	fromUser >> masterName;
	masterName = "%" + masterName + "%";

	char sql[] =
		"SELECT Masters.name, Repairs.name, Cars.state_number "
		"FROM Orders "
		"JOIN Masters ON Orders.master_id = Masters.id "
		"JOIN Repairs ON Orders.repair_id = Repairs.id "
		"JOIN Cars ON Orders.state_number = Cars.state_number "
		"WHERE Masters.name LIKE ? "
		;


	sqlite3_stmt* stmt;

	if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0)) {
		toUser << "sqlite3_prepare_v2 ERROR! "<<sqlite3_errmsg(db)<<"\n\n";
		return;
	}

	sqlite3_bind_text(stmt, 1, masterName.c_str(), -1, SQLITE_TRANSIENT);
	
	toUser
		<< "Мастер" << sep
		<< "Услуга" << sep
		<< "Номер машины" << sep
		<<'\n';

	while (sqlite3_step(stmt) == SQLITE_ROW) {
		for (int i = 0;i < sqlite3_column_count(stmt);i++) {
			const unsigned char* val_ = sqlite3_column_text(stmt, i);
			std::string val = val_ ? (const char*)val_ : "NULL";
			toUser << val<<sep;
		}
		toUser << '\n';
	}
	sqlite3_finalize(stmt);
}
void Server::task3(){
	
	char sql[] =
		"SELECT Brands.brand, Workshops.id, Orders.start_date, Orders.end_date, Repairs.name, Masters.name "
		"FROM Brands "
		"JOIN Cars ON Brands.brand = Cars.brand "
		"JOIN Orders ON Cars.state_number = Orders.state_number "
		"JOIN Workshops ON Orders.workshop_id = Workshops.id "
		"JOIN Repairs ON Orders.repair_id = Repairs.id "
		"JOIN Masters ON Orders.master_id = Masters.id "
		"ORDER BY Brands.brand "
		;


	sqlite3_stmt* stmt;

	if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0)) {
		toUser << "sqlite3_prepare_v2 ERROR! " << sqlite3_errmsg(db) << "\n\n";
		return;
	}

	toUser
		<< "Марка" << sep
		<< "Мастерская" << sep
		<< "Начало" << sep
		<< "Конец" << sep
		<< "Услуга" << sep
		<< "Мастер" << sep
		<< '\n';

	while (sqlite3_step(stmt) == SQLITE_ROW) {
		for (int i = 0;i < sqlite3_column_count(stmt);i++) {

			const unsigned char* val_ = sqlite3_column_text(stmt, i);
			std::string val = val_ ? (const char*)val_ : "NULL";
			if (i == 5) {
				toUser << val.substr(0, val.find_first_of(" "));
			}
			else
				toUser << val;
			toUser << sep;
		}
		toUser << '\n';
	}
	sqlite3_finalize(stmt);
}
void Server::task4(){
	char sql[] =
		"SELECT Workshops.id, count(*), sum(Orders.final_cost) "
		"FROM Workshops "
		"JOIN Orders ON Workshops.id = Orders.workshop_id "
		"GROUP BY Workshops.id "
		;


	sqlite3_stmt* stmt;

	if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0)) {
		toUser << "sqlite3_prepare_v2 ERROR! " << sqlite3_errmsg(db) << "\n\n";
		return;
	}

	int ws[] = {20,18,8};
	toUser
		<< "Номер мастерской" << sep
		<< "Кол-во заказов" << sep
		<< "Прибыль" << sep
		<< '\n';
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		for (int i = 0;i < sqlite3_column_count(stmt);i++) {
			if (i)
				toUser << " | ";
			const unsigned char* val_ = sqlite3_column_text(stmt, i);
			std::string val = val_ ? (const char*)val_ : "NULL";
			toUser << val<<sep;
		}
		toUser << '\n';
	}
	sqlite3_finalize(stmt);
}
void Server::task5() {
char sql[] =
		"SELECT Orders.workshop_id, Repairs.name, Orders.final_cost, Masters.name, Cars.state_number, Cars.brand, Cars.tech_passport, Cars.release_year "
		"FROM Workshops "
		"JOIN Orders ON Orders.workshop_id = Workshops.id "
		"JOIN Repairs ON Repairs.id = Orders.repair_id "
		"JOIN Masters ON Masters.id = Orders.master_id "
		"JOIN Cars ON Cars.state_number = Orders.state_number "
		"WHERE Workshops.id = ( "
		"	SELECT Workshops.id "
		"	FROM Workshops "
		"	JOIN Orders ON Orders.workshop_id = Workshops.id "
		"	GROUP BY Workshops.id "
		"	ORDER BY count(*) DESC "
		"	LIMIT 1 "
		"	) "
		"ORDER BY Repairs.name "
		;


	sqlite3_stmt* stmt;

	if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0)) {
		toUser << "sqlite3_prepare_v2 ERROR! " << sqlite3_errmsg(db) << "\n\n";
		return;
	}
	toUser
		<< "Номер мастерской" << sep
		<< "Услуга" << sep
		<< "Стоимость" << sep
		<< "Мастер" << sep
		<< "Номер машины" << sep
		<< "Марка" << sep
		<< "Тех. пасспорт" << sep
		<< "Выпуск" << sep
		<< '\n';
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		for (int i = 0;i < sqlite3_column_count(stmt);i++) {

			const unsigned char* val_ = sqlite3_column_text(stmt, i);
			std::string val = val_ ? (const char*)val_ : "NULL";
			toUser << val<<sep;
		}
		toUser << '\n';
	}
	sqlite3_finalize(stmt);
}

void Server::masterStatUpdate(){
	
	const char* sql = 
		"DROP TABLE IF EXISTS Master_stat; "
		"CREATE TABLE Master_stat AS "
		"	SELECT "
		"	Masters.id AS Master, "
		"	Repairs.id AS Repair, "
		"	count(orders.id) AS count "
		"	FROM Orders "
		"	JOIN Masters ON Masters.id = Orders.master_id "
		"	JOIN Repairs ON Repairs.id = Orders.repair_id "
		"	GROUP BY Masters.id, repairs.id "
		"	;";

	dbRequest(sql, 0, 0);

}

void Server::problem5(){

	toUser << "Введите номер автомастерской:\n";
	int workshopNum;
	fromUser >> workshopNum;
	fromUser.ignore();

	Date dateStart;
	Date dateEnd;

	try {
		toUser << "Введите дату начала (образ. 2020-01-24):\n";
		fromUser >> dateStart;

		toUser << "Введите дату окончания (образ. 2020-01-24):\n";
		fromUser >> dateEnd;
	}
	catch (int) {
		toUser << "Invalid syntax!\naborting...\n";
		return;
	}

	dbRequest(
		"CREATE TABLE IF NOT EXISTS Workshop_stat( "
		"workshop_id integer NOT NULL, "
		"repair_id integer NOT NULL, "
		"repair_count NOT NULL "
		")", 
		0,
		0);

	const char* sql;
	sqlite3_stmt* stmt;
	
	sql = 
		"DELETE FROM Workshop_stat"
		"WHERE workshop_id = ?";


	sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
	sqlite3_bind_int(stmt, 1, workshopNum);

	sqlite3_step(stmt);
	sqlite3_finalize(stmt);

	sql =
		"INSERT INTO Workshop_stat(workshop_id, repair_id, repair_count)"
		"	SELECT Orders.workshop_id, Orders.repair_id, count(Orders.id) FROM Orders"
		"	WHERE Orders.workshop_id = ? AND Orders.start_date > ? AND Orders.end_date < ?"
		"	GROUP BY Orders.workshop_id, Orders.repair_id";
	sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
	sqlite3_bind_int(stmt, 1, workshopNum);
	sqlite3_bind_text(stmt, 2, dateStart.toString().c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(stmt, 3, dateEnd.toString().c_str(), -1, SQLITE_TRANSIENT);

	sqlite3_step(stmt);
	sqlite3_finalize(stmt);

	dbSelect("SELECT * FROM Workshop_stat", callbackPrintout);
}
void Server::problem6() {
	toUser << "Введите номер автомастерской:\n";
	int workshopNum;
	fromUser >> workshopNum;
	fromUser.ignore();

	Date dateEnd;

	try {
		
		toUser << "Введите дату окончания (образ. 2020-01-24):\n";
		fromUser >> dateEnd;
	}
	catch (int) {
		toUser << "Invalid syntax!\naborting...\n";
		return;
	}

	const char* sql;
	sqlite3_stmt* stmt;


	sql =
		"SELECT count(*) FROM Orders "
		"WHERE workshop_id = ? AND end_date < ?";


	sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
	sqlite3_bind_int(stmt, 1, workshopNum);
	sqlite3_bind_text(stmt, 2, dateEnd.toString().c_str(), -1, SQLITE_TRANSIENT);

	sqlite3_step(stmt);
	const unsigned char* val_ = sqlite3_column_text(stmt, 0);
	std::string val = val_ ? (const char*)val_ : "NULL";
	toUser <<"Работ выполнено: " << val << '\n';

	sqlite3_finalize(stmt);


}
