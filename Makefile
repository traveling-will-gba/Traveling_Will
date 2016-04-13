all:
	g++ src/main.cpp src/game.cpp  -W -Wall -pedantic -ansi -std=c++11 -o obj/main

run:
	./obj/main

clean:
	rm obj/*
