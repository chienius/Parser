#include "lexer/lexer.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    Lexer lexer;
    string input_string;
    string line;
    while(getline(cin, line)) {
        input_string += line + '\n';
    }
    cout << input_string << endl;
    lexer.analyze(input_string);
    lexer.printResult();
    return 0;
}
