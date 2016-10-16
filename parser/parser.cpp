#include <iostream>
#include "./parser.hpp"

// #define DEBUG_PRINT(x) cout << "ERROR syntax in " << x << endl;

#define DEBUG_PRINT(x) ;

void Parser::initializeNTSymbolList() {
    ntSymbolList[$Program] = "$Program";
    ntSymbolList[$DeclBlock] = "$DeclBlock";
    ntSymbolList[$Declaration] = "$Declaration";
    ntSymbolList[$DeclType] = "$DeclType";
    ntSymbolList[$DeclVar] = "$DeclVar";
    ntSymbolList[$DeclFunc] = "$DeclFunc";
    ntSymbolList[$FparaBlock] = "$FparaBlock";
    ntSymbolList[$FpapraList] = "$FpapraList";
    ntSymbolList[$Fparameter] = "$Fparameter";
    ntSymbolList[$StatBlock] = "$StatBlock";
    ntSymbolList[$InnerDeclar] = "$InnerDeclar";
    ntSymbolList[$InnerDeclVar] = "$InnerDeclVar";
    ntSymbolList[$StatString] = "$StatString";
    ntSymbolList[$Statement] = "$Statement";
    ntSymbolList[$StatIf] = "$StatIf";
    ntSymbolList[$StatWhile] = "$StatWhile";
    ntSymbolList[$StatReturn] = "$StatReturn";
    ntSymbolList[$StatAssign] = "$StatAssign";
    ntSymbolList[$Expression] = "$Expression";
    ntSymbolList[$ExprArith] = "$ExprArith";
    ntSymbolList[$Item] = "$Item";
    ntSymbolList[$Factor] = "$Factor";
    ntSymbolList[$Ftype] = "$Ftype";
    ntSymbolList[$Call] = "$Call";
    ntSymbolList[$Aparameter] = "$Aparameter";
    ntSymbolList[$AparaList] = "$AparaList";
    ntSymbolList[$TerminalSymbol] = "$TerminalSymbol";
}

void Parser::advance() {
    while (true) {
        ++word_it;
        if(word_it == wordList.end()) {
            throw runtime_error("End of word list in parsing.");
        }
        if(word_it->symbol == $LCMT || word_it->symbol == $RCMT || word_it->symbol == $CMT) {
            continue;
        } else {
            break;
        }
    }
    word = *word_it;
}

void Parser::retrack(vector<Word>::iterator it) {
    word_it = it;
    word = *word_it;
}

TreeNode Parser::createNode(NTSymbol nt_symbol, Symbol t_symbol, string token) {
    TreeNode new_node;
    new_node.nt_symbol = nt_symbol;
    new_node.t_symbol = t_symbol;
    new_node.token = token;
    return new_node;
}

void Parser::insertNode(TreeNode* parent, TreeNode child) {
    parent->children.push_back(child);
}

int Parser::_program(TreeNode* parent) {
    TreeNode child = createNode($DeclBlock);
    if(_declBlock(&child))  {
        insertNode(parent, child);
    } else {
        return 0;
        DEBUG_PRINT("_program");
    }
    return 1;
}

int Parser::_declBlock(TreeNode* parent) {
    TreeNode child = createNode($Declaration);
    if(_declaration(&child)) {
        insertNode(parent, child);
        while(1) {
            TreeNode child = createNode($Declaration);
            if(_declaration(&child)) {
                insertNode(parent, child);
            } else break;
        }
    } else {
        DEBUG_PRINT("_declBlock");
        return 0;
    }
    return 1;
}

int Parser::_declaration(TreeNode* parent) {
    if(word.symbol == $INT) {
        INSERT_TERM_SYMBOL($INT);
        advance();
        if(word.symbol == $ID) {
            INSERT_TERM_SYMBOL($ID);
            advance();
            TreeNode child = createNode($DeclType);
            if(_declType(&child)) {
                insertNode(parent, child);
            } else{
                DEBUG_PRINT("_declaration");
                return 0;
            }
        } else {
            DEBUG_PRINT("_declaration");
            return 0;
        }
    } else if(word.symbol == $VOID) {
        INSERT_TERM_SYMBOL($VOID);
        advance();
        if(word.symbol == $ID) {
            INSERT_TERM_SYMBOL($ID);
            advance();
            TreeNode child = createNode($DeclFunc);
            if(_declFunc(&child)) {
                insertNode(parent, child);
            } else {
                DEBUG_PRINT("_declaration");
                return 0;
            }
        } else {
            DEBUG_PRINT("_declaration");
            return 0;
        }
    } else {
        DEBUG_PRINT("_declaration");
        return 0;
    }
    return 1;
}

int Parser::_declType(TreeNode* parent) {
    TreeNode child = createNode($DeclVar);
    if(_declVar(&child)) insertNode(parent, child);
    else {
        TreeNode child = createNode($DeclFunc);
        if(_declFunc(&child)) insertNode(parent, child);
        else {
            DEBUG_PRINT("_declType");
            return 0;
        }
    }
    return 1;
}

int Parser::_declVar(TreeNode* parent) {
    if(word.symbol == $SEMICOLON) {
        advance();
        INSERT_TERM_SYMBOL($SEMICOLON);
    } else {
        DEBUG_PRINT("_declVar");
        return 0;
    }
    return 1;
}

int Parser::_declFunc(TreeNode* parent) {
    if(word.symbol == $LPAREN) {
        INSERT_TERM_SYMBOL($LPAREN);
        advance();
        TreeNode child = createNode($FparaBlock);
        if(_fparaBlock(&child)) {
            insertNode(parent, child);
            if(word.symbol == $RPAREN) {
                INSERT_TERM_SYMBOL($RPAREN);
                advance();
                TreeNode child = createNode($StatBlock);
                if(_statBlock(&child)) {
                    insertNode(parent, child);
                    return 1;
                }
            }
        }
    }
    DEBUG_PRINT("_declFunc");
    return 0;
}

int Parser::_fparaBlock(TreeNode *parent) {
    TreeNode child = createNode($FpapraList);
    vector<Word>::iterator t_it = word_it;
    if(word.symbol == $VOID) {
        INSERT_TERM_SYMBOL($VOID);
        advance();
    } else if(_fparaList(&child)) {
        insertNode(parent, child);
    } else {
        return 0;
    }
    return 1;
}

int Parser::_fparaList(TreeNode *parent) {
    TreeNode child = createNode($Fparameter);
    if(_fparameter(&child)) {
        insertNode(parent, child);
        while(word.symbol == $COMMA) {
            INSERT_TERM_SYMBOL($COMMA);
            advance();
            TreeNode child = createNode($Fparameter);
            if(_fparameter(&child)) {
                insertNode(parent, child);
            } else {
                DEBUG_PRINT("_fparaList");
                return 0;
            }
        }
        return 1;
    } else {
        DEBUG_PRINT("_fparaList");
        return 0;
    }
}

int Parser::_fparameter(TreeNode *parent) {
    if(word.symbol == $INT) {
        INSERT_TERM_SYMBOL($INT);
        advance();
        if(word.symbol == $ID) {
            INSERT_TERM_SYMBOL($ID);
            advance();
            return 1;
        }
    }
    DEBUG_PRINT("_fparameter");
    return 0;
}

int Parser::_statBlock(TreeNode *parent) {
    if(word.symbol == $LCURLY) {
        INSERT_TERM_SYMBOL($LCURLY);
        advance();
        TreeNode child = createNode($InnerDeclar);
        if(_innerDeclar(&child)) {
            insertNode(parent, child);
            TreeNode child = createNode($StatString);
            if(_statString(&child)) {
                insertNode(parent, child);
                if(word.symbol == $RCURLY) {
                    INSERT_TERM_SYMBOL($RCURLY);
                    advance();
                    return 1;
                }
            }
        }
    }
    DEBUG_PRINT("_statBlock");
    return 0;
}

int Parser::_innerDeclar(TreeNode *parent) {
    TreeNode child = createNode($InnerDeclVar);
    vector<Word>::iterator t_it = word_it;
    if(_innerDeclVar(&child)) {
        insertNode(parent, child);
        while(word.symbol == $SEMICOLON) {
            INSERT_TERM_SYMBOL($SEMICOLON);
            advance();
            TreeNode child = createNode($InnerDeclar);
            vector<Word>::iterator t_it = word_it;
            if(_innerDeclVar(&child)) {
                insertNode(parent, child);
            } else {
                retrack(t_it);
                break;
            }
        }
        return 1;
    } else {
        retrack(t_it);
        return 1;
    }
    DEBUG_PRINT("_innerDeclar");
    return 0;
}

int Parser::_innerDeclVar(TreeNode *parent) {
    if(word.symbol == $INT) {
        INSERT_TERM_SYMBOL($INT);
        advance();
        if(word.symbol == $ID) {
            INSERT_TERM_SYMBOL($ID);
            advance();
            return 1;
        }
    }
    DEBUG_PRINT("_innerDeclVar");
    return 0;
}

int Parser::_statString(TreeNode* parent) {
    TreeNode child = createNode($Statement);
    if(_statement(&child)) {
        insertNode(parent, child);
        while(1) {
            TreeNode child = createNode($Statement);
            if(_statement(&child)) {
                insertNode(parent, child);
            } else {
                break;
            }
        }
    }
    return 1;
}

int Parser::_statement(TreeNode *parent){
    TreeNode child = createNode($StatIf);
    if(_statIf(&child)) {
        insertNode(parent, child);
    } else {
        TreeNode child = createNode($StatWhile);
        if(_statWhile(&child)) {
            insertNode(parent, child);
        } else {
            TreeNode child = createNode($StatReturn);
            if(_statReturn(&child)) {
                insertNode(parent, child);
            } else {
                TreeNode child = createNode($StatAssign);
                if(_statAssign(&child)) {
                    insertNode(parent, child);
                } else {
                    DEBUG_PRINT("_statement");
                    return 0;
                }
            }
        }
    }
    return 1;
}

int Parser::_statAssign(TreeNode *parent) {
    if(word.symbol == $ID){
        INSERT_TERM_SYMBOL($ID);
        advance();
        if(word.symbol == $ASSIGN) {
            INSERT_TERM_SYMBOL($ASSIGN);
            advance();
            TreeNode child = createNode($Expression);
            if(_expression(&child)) {
                insertNode(parent, child);
                if(word.symbol == $SEMICOLON) {
                    INSERT_TERM_SYMBOL($SEMICOLON);
                    advance();
                    return 1;
                }
            }
        }
    }
    DEBUG_PRINT("_statAssign");
    return 0;
}

int Parser::_statReturn(TreeNode *parent) {
    if(word.symbol == $RETURN) {
        INSERT_TERM_SYMBOL($RETURN);
        advance();
        TreeNode child = createNode($Expression);
        vector<Word>::iterator t_it = word_it;
        if(_expression(&child)) {
            insertNode(parent, child);
        }
        if(word.symbol == $SEMICOLON) {
            INSERT_TERM_SYMBOL($SEMICOLON);
            advance();
            return 1;
        }
    }
    DEBUG_PRINT("_statReturn");
    return 0;
}

int Parser::_statWhile(TreeNode *parent) {
    if(word.symbol == $WHILE) {
        INSERT_TERM_SYMBOL($WHILE);
        advance();
        if(word.symbol == $LPAREN) {
            INSERT_TERM_SYMBOL($LPAREN);
            advance();
            TreeNode child = createNode($Expression);
            if(_expression(&child)) {
                insertNode(parent, child);
                if(word.symbol == $RPAREN) {
                    INSERT_TERM_SYMBOL($RPAREN);
                    advance();
                    TreeNode child = createNode($StatBlock);
                    if(_statBlock(&child)) {
                        insertNode(parent, child);
                        return 1;
                    }
                }
            }
        }
    }
    DEBUG_PRINT("_statWhile");
    return 0;
}

int Parser::_statIf(TreeNode *parent) {
    if(word.symbol == $IF) {
        INSERT_TERM_SYMBOL($IF);
        advance();
        if(word.symbol == $LPAREN) {
            INSERT_TERM_SYMBOL($LPAREN);
            advance();
            TreeNode child = createNode($Expression);
            if(_expression(&child)) {
                insertNode(parent, child);
                if(word.symbol == $RPAREN) {
                    INSERT_TERM_SYMBOL($RPAREN);
                    advance();
                    TreeNode child = createNode($StatBlock);
                    if(_statBlock(&child)) {
                        insertNode(parent, child);
                        if(word.symbol == $ELSE) {
                            INSERT_TERM_SYMBOL($ELSE);
                            advance();
                            TreeNode child = createNode($StatBlock);
                            if(_statBlock(&child)) {
                                insertNode(parent, child);
                            } else {
                                DEBUG_PRINT("_statIf");
                                return 0;
                            }
                        }
                        return 1;
                    }
                }
            }
        }
    }
    DEBUG_PRINT("_statIf");
    return 0;
}

int Parser::_expression(TreeNode *parent) {
    TreeNode child = createNode($ExprArith);
    if(_exprArith(&child)) {
        insertNode(parent, child);
        while(true) {
            if(word.symbol == $LE) {
                INSERT_TERM_SYMBOL($LE);
            } else if(word.symbol == $LT) {
                INSERT_TERM_SYMBOL($LT);
            } else if(word.symbol == $GE) {
                INSERT_TERM_SYMBOL($GE);
            } else if(word.symbol == $GT) {
                INSERT_TERM_SYMBOL($GT);
            } else if(word.symbol == $EQUAL) {
                INSERT_TERM_SYMBOL($EQUAL);
            } else if(word.symbol == $NEQUAL) {
                INSERT_TERM_SYMBOL($NEQUAL);
            } else {
                break;
            }
            advance();
            TreeNode child = createNode($ExprArith);
            if(_exprArith(&child)) {
                insertNode(parent, child);
            } else {
                DEBUG_PRINT("_expression");
                return 0;
            }
        }
        return 1;
    }
    DEBUG_PRINT("_expression");
    return 0;
}

int Parser::_exprArith(TreeNode* parent) {
    TreeNode child = createNode($Item);
    if(_item(&child)) {
        insertNode(parent, child);
        while(true) {
            if(word.symbol == $PLUS) {
                INSERT_TERM_SYMBOL($PLUS);
            } else if(word.symbol == $MIN) {
                INSERT_TERM_SYMBOL($MIN);
            } else {
                break;
            }
            advance();
            TreeNode child = createNode($Item);
            if(_item(&child)) {
                insertNode(parent, child);
            } else {
                DEBUG_PRINT("_exprArith");
                return 0;
            }
        }
        return 1;
    }
    DEBUG_PRINT("_exprArith");
    return 0;
}

int Parser::_item(TreeNode* parent) {
    TreeNode child = createNode($Factor);
    if(_factor(&child)) {
        insertNode(parent, child);
        while(true) {
            if(word.symbol == $STAR) {
                INSERT_TERM_SYMBOL($STAR);
            } else if(word.symbol == $SLASH) {
                INSERT_TERM_SYMBOL($SLASH);
            } else {
                break;
            }
            advance();
            TreeNode child = createNode($Factor);
            if(_factor(&child)) {
                insertNode(parent, child);
            } else {
                DEBUG_PRINT("_item");
                return 0;
            }
        }
        return 1;
    }
    DEBUG_PRINT("_item");
    return 0;
}

int Parser::_factor(TreeNode* parent) {
    if(word.symbol == $NUM) {
        INSERT_TERM_SYMBOL($NUM);
        advance();
        return 1;
    } else if(word.symbol == $LPAREN){
        INSERT_TERM_SYMBOL($LPAREN);
        advance();
        TreeNode child = createNode($Expression);
        if(_expression(&child)) {
            insertNode(parent, child);
            if(word.symbol == $RPAREN) {
                INSERT_TERM_SYMBOL($RPAREN);
                advance();
                return 1;
            }
        }
    } else if(word.symbol == $ID) {
        INSERT_TERM_SYMBOL($ID);
        advance();
        TreeNode child = createNode($Ftype);
        if(_ftype(&child)) {
            insertNode(parent, child);
            return 1;
        }
    }
    DEBUG_PRINT("_factor");
    return 0;
}

int Parser::_ftype(TreeNode *parent) {
    TreeNode child = createNode($Call);
    vector<Word>::iterator t_it = word_it;
    if(_call(&child)) {
        insertNode(parent, child);
    } else {
        retrack(t_it);
    }
    return 1;
}

int Parser::_call(TreeNode *parent) {
    if(word.symbol == $LPAREN) {
        INSERT_TERM_SYMBOL($LPAREN);
        advance();
        TreeNode child = createNode($Aparameter);
        if(_aparameter(&child)) {
            insertNode(parent, child);
            if(word.symbol == $RPAREN) {
                INSERT_TERM_SYMBOL($RPAREN);
                advance();
                return 1;
            }
        }
    }
    DEBUG_PRINT("_call");
    return 0;
}

int Parser::_aparameter(TreeNode *parent) {
    TreeNode child = createNode($AparaList);
    vector<Word>::iterator t_it = word_it;
    if(_aparaList(&child)) {
        insertNode(parent, child);
    } else {
        retrack(t_it);
    }
    return 1;
}

int Parser::_aparaList(TreeNode *parent) {
    TreeNode child = createNode($Expression);
    if(_expression(&child)) {
        insertNode(parent, child);
        while(word.symbol == $COMMA) {
            INSERT_TERM_SYMBOL($COMMA);
            advance();
            TreeNode child = createNode($Expression);
            if(_expression(&child)) {
                insertNode(parent, child);
            } else {
                DEBUG_PRINT("_aparaList");
                return 0;
            }
        }
        return 1;
    }
    DEBUG_PRINT("_aparaList");
    return 0;
}

void Parser::analyze(string input) {
    Lexer::analyze(input);

    initializeNTSymbolList();
    TreeNode newTree;
    newTree.nt_symbol = $Program;
    synTree = newTree;

    word_it = Lexer::wordList.begin();
    word = *word_it;
    while(word.symbol == $LCMT || word.symbol == $RCMT || word.symbol == $CMT) {
        word_it ++;
        word = *word_it;
    }

    if(!Parser::_program(&synTree)){
        cout << word.token << endl;
        //throw runtime_error("Syntax error detected.");
    };
}

void Parser::dfsResult(boost::property_tree::ptree* pt, TreeNode n) {
    using namespace boost::property_tree;
    ptree children;
    if(n.children.size()) {
        for(vector<TreeNode>::reverse_iterator it = n.children.rbegin(); it!=n.children.rend(); ++it) {
            ptree ptNode;
            if(n.nt_symbol != $TerminalSymbol) {
                ptNode.put("is_terminal", 0);
                ptNode.put("symbol", it->nt_symbol);
                ptNode.put("symbol_title", ntSymbolList[it->nt_symbol]);
            } else {
                ptNode.put("is_terminal", 1);
                ptNode.put("symbol", it->t_symbol);
                ptNode.put("symbol_title", Lexer::symbolList[it->t_symbol]);
                ptNode.put("token", it->token);
            }
            dfsResult(&ptNode, *it);
            children.push_back(std::make_pair("", ptNode));
        }
        pt->add_child("children", children);
    } else {
        return;
    }

}

boost::property_tree::ptree Parser::generateResult() {
    using namespace boost::property_tree;

    ptree pt;
    ptree children;

    children.put("is_terminal", 0);
    children.put("symbol", synTree.nt_symbol);
    children.put("symbol_title", ntSymbolList[synTree.nt_symbol]);
    dfsResult(&children, synTree);
    pt.add_child("results", children);

    return pt;
}

void Parser::printResult() {
    using namespace boost::property_tree;
    ptree pt = generateResult();
    std::stringstream ss_out;
    write_json(ss_out, pt);
    std::string str_out =  ss_out.str();
    std::cout << str_out << endl;
}
