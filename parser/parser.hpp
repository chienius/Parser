#include "../lexer/lexer.hpp"

enum NTSymbol {
    $Program=1, $DeclBlock, $Declaration, $DeclType, $DeclVar, $DeclFunc, $FparaBlock,
    $FpapraList, $Fparameter, $StatBlock, $InnerDeclar, $InnerDeclVar, $StatString, $Statement, $StatIf, $StatWhile,
    $StatReturn, $StatAssign, $Expression, $ExprArith, $Item, $Factor,
    $Ftype, $Call, $Aparameter, $AparaList, $TerminalSymbol
};

typedef map<NTSymbol, string> NTSymbolMap;

typedef struct TreeNode {
    vector<TreeNode> children;
    NTSymbol nt_symbol; //Non-terminal symbol type
    Symbol t_symbol; //Terminal symbol type
    string token;
} TreeNode;

typedef struct NodeElem {
    TreeNode n;
    int d;  //depth
} NodeElem;

/**
 * Parser
 */
class Parser : protected Lexer {
    protected:
        TreeNode synTree; // Syntactical Tree
        NTSymbolMap ntSymbolList;
        void initializeNTSymbolList();

    private:
        /**
         * Automation Recursive Functions
         * @return 1 - Success; 0 - Error;
         */
        int _program(TreeNode* parent);
        int _declBlock(TreeNode* parent);
        int _declaration(TreeNode* parent);
        int _declType(TreeNode* parent);
        int _declVar(TreeNode* parent);
        int _declFunc(TreeNode* parent);
        int _fparaBlock(TreeNode* parent);
        int _fparaList(TreeNode* parent);
        int _fparameter(TreeNode* parent);
        int _statBlock(TreeNode* parent);
        int _innerDeclar(TreeNode *parent);
        int _innerDeclVar(TreeNode *parent);
        int _statString(TreeNode* parent);
        int _statement(TreeNode* parent);
        int _statIf(TreeNode* parent);
        int _statWhile(TreeNode* parent);
        int _statReturn(TreeNode* parent);
        int _statAssign(TreeNode* parent);
        int _expression(TreeNode* parent);
        int _exprArith(TreeNode* parent);
        int _item(TreeNode* parent);
        int _factor(TreeNode* parent);
        int _ftype(TreeNode* parent);
        int _call(TreeNode* parent);
        int _aparameter(TreeNode* parent);
        int _aparaList(TreeNode* parent);

        void advance();
        void retrack(vector<Word>::iterator it);
        Word word; // Current definite symbol
        vector<Word>::iterator word_it;
        TreeNode createNode(NTSymbol nt_symbol, Symbol t_symbol = (Symbol)0, string token="");
        void insertNode(TreeNode* parent, TreeNode child);
    public:
        void analyze(string input);
        void printResult();
};

#define INSERT_TERM_SYMBOL(x)  insertNode(parent, createNode($TerminalSymbol, x, word.token))
