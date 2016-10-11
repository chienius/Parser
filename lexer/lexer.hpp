#include <string>
#include <vector>
#include <map>
#include <exception>
using namespace std;

typedef struct {
    string token;
    // TODO Complete the definition.
} Identifier;

typedef struct {
    string token;
    int integer;
    // TODO Complete the definition.
} Lconst;

enum SYMBOL {
    $INT=1, $VOID=2, $IF=3, $ELSE=4, $WHILE=5, $RETURN=6,
    $ID=7, $NUM=8, $ASSIGN=9, $PLUS=10, $MIN=11, $STAR=12, $SLASH=13,
    $EQUAL=14, $GT=15, $GE=16, $LT=17, $LE=18, $NEQUAL=19, $SEMICOLON=20,
    $COMMA=21, $LCMT=22, $RCMT=23, $CMT=24, $LPAREN=25, $RPAREN=26,
    $LCURLY=27, $RCURLY=28, $EOF=29, $RET=30, $OTHER=31
};

typedef struct {
    SYMBOL symbol;
    string token;
} Word;


typedef std::map<int, string> SymbolMap;


/**
 * Lexer
 */
class Lexer {
    private:
        //static const string reservedWords[6];
        void initializeSymbolList();
        SymbolMap symbolList;

        vector<Identifier> identifiers;
        vector<Lconst> consts;
        string token; // String Buffer
        char token_char; // Reading char
        vector<Word> wordList; // <- Results are here

        void skipSpaces();
        bool isDigit();
        bool isAlpha();
        int findReserved(string token);

        void insertId(string token);
        void insertConst(string token);

        string input;
        string::iterator input_it;
        bool readCh();
        void concatCh();
        void insertWord(SYMBOL symbol);

        int mainControl();
        int commentControl();
        int longCommentControl();

    public:
        void analyze(string input);
        void printResult();
};

//const string Lexer::reservedWords[6] = {"int", "void", "if", "else", "while", "return"};

