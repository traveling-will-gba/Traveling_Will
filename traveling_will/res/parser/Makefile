parser:
	g++ -o parser -W -Wall -ansi -pedantic ly2level.cpp -std=c++11

smoother:
	g++ -o smoother -W -Wall -ansi -pedantic smoother.cpp -std=c++11

normalizer:
	g++ -o normalizer -W -Wall -ansi -pedantic normalizer.cpp -std=c++11

generate:
	rm parser -f
	make parser
	./parser $(lilypond) $(lilypond).txt
	cp $(lilypond).txt level_design.txt
	rm normalizer -f
	make normalizer
	./normalizer > $(lilypond)_normalized.txt
	rm smoother -f
	make smoother
	./smoother < $(lilypond)_normalized.txt > level_design.txt
	rm $(lilypond).txt $(lilypond)_normalized.txt 

clean:
	rm parser -f
	rm normalizer -f
	rm smoother -f
