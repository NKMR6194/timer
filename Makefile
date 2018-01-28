all: bin/timer bin/alarm

bin/timer: main.cpp
	g++ -std=c++11 main.cpp -o bin/timer

bin/alarm: alarm.cpp
	g++ -std=c++11 alarm.cpp -o bin/alarm