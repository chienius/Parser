main: lexer.o parser.o main.cpp
	g++ lexer.o parser.o main.cpp -I. -o main

lexer.o: ./lexer/lexer.cpp ./lexer/lexer.hpp
	g++ -c ./lexer/lexer.cpp -o lexer.o

parser.o: ./parser/parser.cpp ./parser/parser.hpp
	g++ -c ./parser/parser.cpp -o parser.o

.PHONY: clean test

clean:
	rm -rf main lexer.o parser.o

test: main
	main <test.a
