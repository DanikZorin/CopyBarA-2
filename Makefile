bin/main: obj/main.o obj/date.o obj/server.o obj/authenticator.o
	g++ obj/main.o obj/date.o obj/server.o obj/authenticator.o -o bin/main -lsqlite3

obj/main.o: src/main.cpp
	g++ -c src/main.cpp -o obj/main.o

obj/date.o: src/date.cpp
	g++ -c src/date.cpp -o obj/date.o

obj/server.o: src/server.cpp
	g++ -c src/server.cpp -o obj/server.o -lsqlite3

obj/authenticator.o: src/authenticator.cpp
	g++ -std=c++17  -c src/authenticator.cpp -o obj/authenticator.o

