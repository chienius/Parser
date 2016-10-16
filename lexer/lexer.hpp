#include <string>
#include <vector>
#include <map>
#include <exception>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
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

enum Symbol {
    $INT=1, $VOID=2, $IF=3, $ELSE=4, $WHILE=5, $RETURN=6,
    $ID=7, $NUM=8, $ASSIGN=9, $PLUS=10, $MIN=11, $STAR=12, $SLASH=13,
    $EQUAL=14, $GT=15, $GE=16, $LT=17, $LE=18, $NEQUAL=19, $SEMICOLON=20,
    $COMMA=21, $LCMT=22, $RCMT=23, $CMT=24, $LPAREN=25, $RPAREN=26,
    $LCURLY=27, $RCURLY=28, $EOF=29, $RET=30, $OTHER=31
};

typedef struct {
    Symbol symbol;
    string token;
    // TODO Implement Const and Id table cursors.
    //Lconst* lconst;
    //Identifier* identifier;
} Word;


typedef std::map<int, string> SymbolMap;


/**
 * Lexer
 */
class Lexer {
    protected:
        void initializeSymbolList();
        SymbolMap symbolList;

        vector<Identifier> identifiers;
        vector<Lconst> consts;
        vector<Word> wordList; // <- Results are here

    private:
        string token; // String Buffer
        char token_char; // Reading char

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
        void insertWord(Symbol symbol);

        int mainControl();
        int commentControl();
        int longCommentControl();

    public:
        void analyze(string input);
        boost::property_tree::ptree generateResult();
        void printResult();
};

