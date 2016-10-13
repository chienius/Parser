#include "parser/parser.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    Parser parser;
    string input_string;
    string line;
    while(getline(cin, line)) {
        input_string += line + '\n';
    }
    cout << input_string << endl;
    parser.analyze(input_string);
    parser.printResult();
    return 0;
}
