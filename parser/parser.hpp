#include "../lexer/lexer.hpp"

enum NTSymbol {
    $Program=1, $Declaration, $DeclType, $DeclVar, $DeclFunc, $FparaBlock,
    $FpapraList, $Fparameter, $StatBlock, $Statement, $StatIf, $StatWhile,
    $StatReturn, $StatAssign, $Expression, $ExprArith, $Item, $Factor,
    $Ftype, $Call, $Aparameter, $AparaList, $TerminalSymbol
};

typedef struct TreeNode {
    TreeNode *parent;
    vector<TreeNode> children;
    NTSymbol nt_symbol; //Non-terminal symbol type
    Symbol t_symbol; //Terminal symbol type

    void appendChild(NTSymbol nt_symbol, Symbol symbol);
} TreeNode;


/**
 * Parser
 */
class Parser : protected Lexer {
    protected:
        TreeNode synTree; // Syntactical Tree
        TreeNode currentNode;

        /**
         * Automation Recursive Functions
         * @return 1 - Success; 0 - Error;
         */
        int _program(TreeNode* parent);
        int _declaration(TreeNode* parent);
        int _declType(TreeNode* parent);
        int _declVar(TreeNode* parent);
        int _declFunc(TreeNode* parent);
        int _fparaBlock(TreeNode* parent);
        int _fparaList(TreeNode* parent);
        int _fparameter(TreeNode* parent);
        int _statBlock(TreeNode* parent);
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
        void appendNode(NTSymbol nt_symbol, Symbol symbol=$OTHER);
        Symbol symbol; // Current definite symbol

    public:
        void analyze(string input);
        void printResult();
};

