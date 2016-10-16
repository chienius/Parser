#include "parser/parser.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    string input_string;
    string line;
    while(getline(cin, line)) {
        input_string += line + '\n';
    }

    string s = string(argv[1]);
    if(s == "lexer") {
        Lexer lexer;
        try {
            lexer.analyze(input_string);
            lexer.printResult();
        } catch(exception& e) {
            cout << "0" << endl;
        }
    }else if(s == "parser") {
        Parser parser;
        try {
            parser.analyze(input_string);
            parser.printResult();
        } catch(exception& e) {
            cout << "0" << endl;
        }
    }

    return 0;
}
