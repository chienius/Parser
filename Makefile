main: lexer.o main.cpp
	g++ lexer.o main.cpp -I. -o main

lexer.o: ./lexer/lexer.cpp ./lexer/lexer.hpp
	g++ -c ./lexer/lexer.cpp -o lexer.o

.PHONY: clean test

clean:
	rm -rf main lexer.o

test: main
	main <test.a
