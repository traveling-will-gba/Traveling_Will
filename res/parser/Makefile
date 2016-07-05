parser:
	g++ -o parser -W -Wall -ansi -pedantic ly2level.cpp -std=c++11

smoother:
	g++ -o smoother -W -Wall -ansi -pedantic smoother.cpp -std=c++11

generate:
	rm smoother -f
	make parser
	rm smoother -f
	make smoother
	./parser $(a) $(a).txt
	./smoother < $(a).txt > $(a)_smoother.txt
