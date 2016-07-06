parser:
	g++ -o parser -W -Wall -ansi -pedantic ly2level.cpp -std=c++11

smoother:
	g++ -o smoother -W -Wall -ansi -pedantic smoother.cpp -std=c++11

test_design:
	g++ -o test_design -W -Wall -ansi -pedantic test_design.cpp -std=c++11


generate:
	rm parser -f
	make parser
	./parser $(a) $(a).txt
	cp $(a).txt level_design.txt
	rm test_design -f
	make test_design
	./test_design > $(a)_normalized.txt
	rm smoother -f
	make smoother
	./smoother < $(a)_normalized.txt > $(a)_final.txt
