#include <iostream>
#include "lexer.hpp"

/**
 * Initialize Symbol List in Lexer.
 * @return void
 */
void Lexer::initializeSymbolList() {
    symbolList[$INT] = "$INT";
    symbolList[$VOID] = "$VOID";
    symbolList[$IF] = "$IF";
    symbolList[$ELSE] = "$ELSE";
    symbolList[$WHILE] = "$WHILE";
    symbolList[$RETURN] = "$RETURN";
    symbolList[$ID] = "$ID";
    symbolList[$NUM] = "$NUM";
    symbolList[$ASSIGN] = "$ASSIGN";
    symbolList[$PLUS] = "$PLUS";
    symbolList[$MIN] = "$MIN";
    symbolList[$STAR] = "$STAR";
    symbolList[$SLASH] = "$SLASH";
    symbolList[$EQUAL] = "$EQUAL";
    symbolList[$GT] = "$GT";
    symbolList[$GE] = "$GE";
    symbolList[$LT] = "$LT";
    symbolList[$LE] = "$LE";
    symbolList[$NEQUAL] = "$NEQUAL";
    symbolList[$SEMICOLON] = "$SEMICOLON";
    symbolList[$COMMA] = "$COMMA";
    symbolList[$LCMT] = "$LCMT";
    symbolList[$RCMT] = "$RCMT";
    symbolList[$CMT] = "$CMT";
    symbolList[$LPAREN] = "$LPAREN";
    symbolList[$RPAREN] = "$RPAREN";
    symbolList[$LCURLY] = "$LCURLY";
    symbolList[$RCURLY] = "$RCURLY";
    symbolList[$EOF] = "$EOF";
    symbolList[$RET] = "$RET";
    symbolList[$OTHER] = "$OTHER";
}


void Lexer::skipSpaces() {
    while(isspace(token_char)) {
        readCh();
    }
}

bool Lexer::isDigit() {
    return std::isdigit(token_char);
}

bool Lexer::isAlpha() {
    return std::isalpha(token_char);
}

int Lexer::findReserved(string token) {
    if(token == "int") {
        return $INT;
    } else if(token == "void") {
        return $VOID;
    } else if(token == "if") {
        return $IF;
    } else if(token == "else") {
        return $ELSE;
    } else if(token == "while") {
        return $WHILE;
    } else if(token == "return") {
        return $RETURN;
    } else {
        return 0;
    }
}

void Lexer::insertId(string token) {
    for(vector<Identifier>::iterator it=identifiers.begin(); it!=identifiers.end(); ++it) {
        if(it->token == token) {
            return;
        }
    }
    Identifier id = {token};
    identifiers.push_back(id);
}

void Lexer::insertConst(string token) {
    for(vector<Lconst>::iterator it=consts.begin(); it!=consts.end(); ++it) {
        if(it->token == token) {
            return;
        }
    }
    Lconst lconst = {token, std::atoi(token.c_str())};
    consts.push_back(lconst);
}

void Lexer::insertWord(SYMBOL symbol) {
    Word word = {symbol, token};
    wordList.push_back(word);
    token = "";
}

bool Lexer::readCh() {
    if(input_it == input.end()) {
        printResult();
        throw runtime_error("No more char can be read.");
        return false;
    }
    string::iterator it = input_it;
    ++input_it;
    token_char = *it;
    return true;
}

void Lexer::concatCh() {
    token += token_char;
}

int Lexer::mainControl() {
    skipSpaces();
    if( isAlpha() ) {
        concatCh();
        readCh();
        while( isAlpha() || isDigit() ) {
            concatCh();
            readCh();
        }
        // $ID
        int symbol = findReserved(token);
        if( symbol == 0 ) {
            insertId(token);
            insertWord($ID);
        }else {
            insertWord((SYMBOL)symbol);
        }
    } else if(isDigit()) {
        concatCh();
        readCh();
        while(isDigit()) {
            concatCh();
            readCh();
        }
        // $NUM
        insertConst(token);
        insertWord($NUM);
    } else if(token_char == '=') {
        concatCh();
        readCh();
        if(token_char == '=') {
            // $EQUAL
            concatCh();
            insertWord($EQUAL);
            readCh();
        } else {
            // $ASSIGN
            insertWord($ASSIGN);
        }
    } else if(token_char == '+') {
        concatCh();
        // $PLUS
        insertWord($PLUS);
        readCh();
    } else if(token_char == '-') {
        concatCh();
        // $MIN
        insertWord($MIN);
        readCh();
    } else if(token_char == '*') {
        concatCh();
        // $STAR
        insertWord($STAR);
        readCh();
    } else if(token_char == '>') {
        concatCh();
        readCh();
        if(token_char == '=') {
            concatCh();
            insertWord($GE);
            readCh();
        } else {
            insertWord($GT);
        }
    } else if(token_char == '<') {
        concatCh();
        readCh();
        if(token_char == '=') {
            concatCh();
            insertWord($LE);
            readCh();
        } else {
            insertWord($LT);
        }
    } else if(token_char == '!') {
        concatCh();
        readCh();
        if(token_char == '=') {
            concatCh();
            insertWord($NEQUAL);
            readCh();
        } else {
            throw runtime_error("No match in NEQUAL");
        }
    } else if(token_char == ';') {
        concatCh();
        insertWord($SEMICOLON);
        readCh();
    } else if(token_char == ',') {
        concatCh();
        insertWord($COMMA);
        readCh();
    } else if(token_char == '/') {
        concatCh();
        readCh();
        if(token_char == '/') {
            concatCh();
            insertWord($CMT);
            readCh();
            return 1;
        } else if(token_char == '*')  {
            concatCh();
            insertWord($LCMT);
            readCh();
            return 2;
        } else {
            insertWord($SLASH);
        }
    } else if(token_char == '(') {
        concatCh();
        insertWord($LPAREN);
        readCh();
    } else if(token_char == ')') {
        concatCh();
        insertWord($RPAREN);
        readCh();
    } else if(token_char == '{') {
        concatCh();
        insertWord($LCURLY);
        readCh();
    } else if(token_char == '}') {
        concatCh();
        insertWord($RCURLY);
        readCh();
    } else if(token_char == '#') {
        concatCh();
        insertWord($EOF);
        return -1;
    } else {
        throw runtime_error("No match globally");
    }
    return 0;
}

int Lexer::longCommentControl() {
    if(token_char == '*') {
        concatCh();
        readCh();
        if(token_char == '/') {
            concatCh();
            insertWord($RCMT);
            readCh();
            return 0;
        } else {
            token="";
            return 2;
        }
    } else {
        readCh();
    }
    return 2;
}

int Lexer::commentControl() {
    if(token_char == '\n') {
        readCh();
        return 0;
    } else {
        readCh();
    }
    return 1;
}

void Lexer::analyze(string instr) {
    Lexer::initializeSymbolList();
    token = "";
    input = instr;
    input_it = input.begin();

    int control_entry = 0;
    int control_finished = false;

    readCh();
    while(input_it!=input.end()) {
        switch(control_entry) {
            case -1:
                control_finished = true;
                break;
            case 1:
                control_entry = commentControl();
                break;
            case 2:
                control_entry = longCommentControl();
                break;
            default:
                control_entry = mainControl();
        }
        if(control_finished) break;
    }
}

void Lexer::printResult() {
    for(vector<Word>::iterator it=wordList.begin(); it!=wordList.end(); ++it) {
        cout << "(" << it->symbol << ", " << symbolList[it->symbol] << ", " << it->token << ")" << endl;
    }
}
