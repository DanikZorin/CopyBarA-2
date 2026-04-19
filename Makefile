CXX = g++
CXXFLAGS = -std=c++17 -Iinclude
LIBS = -lsqlite3
TARGET = bin/main

$(shell mkdir -p obj bin)

SRC = src/main.cpp src/authenticator.cpp src/date.cpp src/server.cpp
OBJ = obj/main.o obj/authenticator.o obj/date.o obj/server.o

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LIBS)

obj/main.o: src/main.cpp include/authenticator.h
	$(CXX) $(CXXFLAGS) -c src/main.cpp -o obj/main.o

obj/authenticator.o: src/authenticator.cpp include/authenticator.h include/server.h
	$(CXX) $(CXXFLAGS) -c src/authenticator.cpp -o obj/authenticator.o

obj/date.o: src/date.cpp include/date.h
	$(CXX) $(CXXFLAGS) -c src/date.cpp -o obj/date.o

obj/server.o: src/server.cpp include/server.h include/date.h
	$(CXX) $(CXXFLAGS) -c src/server.cpp -o obj/server.o

clean:
	rm -f obj/*.o bin/*

run: $(TARGET)
	./$(TARGET)
