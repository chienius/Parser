#include "./parser.hpp"

void Parser::advance() {
    while (true) {
        ++word_it;
        if(word_it == wordList.end()) {
            throw runtime_error("End of word list in parsing.");
        }
        if(word.symbol == $LCMT || word.symbol == $RCMT || word.symbol == $CMT) {
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

TreeNode Parser::createNode(NTSymbol nt_symbol, Symbol t_symbol) {
    TreeNode new_node;
    new_node.parent = nullptr;
    new_node.nt_symbol = nt_symbol;
    new_node.t_symbol = t_symbol;
    return new_node;
}

void Parser::insertNode(TreeNode* parent, TreeNode child) {
    child.parent = parent;
    parent->children.push_back(child);
}

int Parser::_program(TreeNode* parent) {
    TreeNode child = createNode($DeclBlock);
    int r = _declBlock(&child);
    if(r) insertNode(parent, child);
    return 1;
}

int Parser::_declBlock(TreeNode* parent) {
    int emtpy = true;
    while(1) {
        TreeNode child = createNode($Declaration);
        int r = _declaration(&child);
        if(r) {
            insertNode(parent, child);
            emtpy = false;
        } else break;
    }
    if(emtpy) {
        return 0;
    }
    return 1;
}

int Parser::_declaration(TreeNode* parent) {
    if(word.symbol == $INT) {
        advance();
        if(word.symbol == $ID) {
            advance();
            TreeNode child = createNode($DeclType);
            int r = _declType(&child);
            if(r) {
                INSERT_TERM_SYMBOL($INT);
                INSERT_TERM_SYMBOL($ID);
                insertNode(parent, child);
            }
            else{
                return 0;
            }
        } else {
            return 0;
        }
    } else if(word.symbol == $VOID) {
        advance();
        if(word.symbol == $ID) {
            advance();
            TreeNode child = createNode($DeclFunc);
            int r = _declFunc(&child);
            if(r) {
                INSERT_TERM_SYMBOL($VOID);
                INSERT_TERM_SYMBOL($ID);
                insertNode(parent, child);
            }
            else {
                return 0;
            }
        } else {
            return 0;
        }
    } else {
        return 0;
    }
    return 1;
}

int Parser::_declType(TreeNode* parent) {
    TreeNode child = createNode($DeclVar);
    int r = _declVar(&child);
    if(r) insertNode(parent, child);
    else {
        TreeNode child = createNode($DeclFunc);
        int r = _declFunc(&child);
        if(r) insertNode(parent, child);
        else return 0;
    }
    return 1;
}

int Parser::_declVar(TreeNode* parent) {
    if(word.symbol == $SEMICOLON) {
        advance();
        INSERT_TERM_SYMBOL($SEMICOLON);
    } else {
        return 0;
    }
    return 1;
}

int Parser::_declFunc(TreeNode* parent) {
    if(word.symbol == $LPAREN) {
        advance();
        TreeNode child = createNode($FparaBlock);
        int r = _fparaBlock(&child);
        if(r) {
            if(word.symbol == $RPAREN) {
                advance();
                TreeNode child2 = createNode($StatBlock);
                int r = _statBlock(&child2);
                if(r) {
                    INSERT_TERM_SYMBOL($LPAREN);
                    insertNode(parent, child);
                    INSERT_TERM_SYMBOL($RPAREN);
                    insertNode(parent, child2);
                } else {
                    return 0;
                }
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    } else {
        return 0;
    }
    return 1;
}

int Parser::_fparaBlock(TreeNode *parent) {
    TreeNode child = createNode($FpapraList);
    vector<Word>::iterator t_it = word_it;
    int r = _fparaList(&child);
    if(r) {
        insertNode(parent, child);
    } else {
        retrack(t_it);
    }
    return 1;
}

int Parser::_fparaList(TreeNode *parent) {
    TreeNode child = createNode($Fparameter);
    int r = _fparameter(&child);
    if(r) {
        insertNode(parent, child);
    } else {
        return 0;
    }
    while(word.symbol == $COMMA) {
        advance();
        TreeNode child = createNode($Fparameter);
        int r = _fparameter(&child);
        if(r) {
            INSERT_TERM_SYMBOL($COMMA);
            insertNode(parent, child);
        } else {
            return 0;
        }
    }
    return 1;
}

int Parser::_fparameter(TreeNode *parent) {
    if(word.symbol == $INT) {
        advance();
        if(word.symbol == $ID) {
            advance();
            INSERT_TERM_SYMBOL($INT);
            INSERT_TERM_SYMBOL($ID);
        } else {
            return 0;
        }
    } else {
        return 0;
    }
    return 1;
}

int Parser::_statBlock(TreeNode *parent) {
    if(word.symbol == $LCURLY) {
        advance();
        TreeNode child = createNode($InnerDeclar);
        int r = _innerDeclar(&child);
        if(r) {
            TreeNode child2 = createNode($StatString);
            int r = _statString(&child2);
            if(r) {
                if(word.symbol == $RCURLY) {
                    advance();
                    INSERT_TERM_SYMBOL($LCURLY);
                    insertNode(parent, child);
                    insertNode(parent, child2);
                    INSERT_TERM_SYMBOL($RCURLY);
                    return 1;
                }
            }
        }
    }
    return 0;
}

int Parser::_innerDeclar(TreeNode *parent) {
    TreeNode child = createNode($InnerDeclVar);
    vector<Word>::iterator t_it = word_it;
    int r =_innerDeclVar(&child);
    if(r) {
        insertNode(parent, child);
        while(word.symbol == $SEMICOLON) {
            advance();
            TreeNode child = createNode($InnerDeclar);
            int r = _innerDeclVar(&child);
            if(r) {
                INSERT_TERM_SYMBOL($SEMICOLON);
                insertNode(parent, child);
            } else {
                return 0;
            }
        }
        if(word.symbol == $SEMICOLON) {
            advance();
            INSERT_TERM_SYMBOL($SEMICOLON);
        } else {
            return 0;
        }
    } else {
        retrack(t_it);
    }
    return 1;
}

int Parser::_innerDeclVar(TreeNode *parent) {
    if(word.symbol == $INT) {
        advance();
        if(word.symbol == $ID) {
            advance();
            INSERT_TERM_SYMBOL($INT);
            INSERT_TERM_SYMBOL($ID);
            return 1;
        }
    }
    return 0;
}

int Parser::_statString(TreeNode* parent) {
    int emtpy = true;
    while(1) {
        TreeNode child = createNode($Statement);
        int r = _statement(&child);
        if(r) {
            insertNode(parent, child);
            emtpy = false;
        }
        else break;
    }
    if(emtpy) {
        return 0;
    }
    return 1;
}

int Parser::_statement(TreeNode *parent){
    TreeNode child = createNode($StatIf);
    int r = _statIf(&child);
    if(r) {
        insertNode(parent, child);
    } else {
        TreeNode child = createNode($StatWhile);
        int r = _statWhile(&child);
        if(r) {
            insertNode(parent, child);
        } else {
            TreeNode child = createNode($StatReturn);
            int r = _statReturn(&child);
            if(r) {
                insertNode(parent, child);
            } else {
                TreeNode child = createNode($StatAssign);
                int r = _statAssign(&child);
                if(r) {
                    insertNode(parent, child);
                } else {
                    return 0;
                }
            }
        }
    }
    return 1;
}

int Parser::_statAssign(TreeNode *parent) {
    if(word.symbol == $ID){
        advance();
        if(word.symbol == $ASSIGN) {
            advance();
            TreeNode child = createNode($Expression);
            if(_expression(&child)) {
                INSERT_TERM_SYMBOL($ID);
                INSERT_TERM_SYMBOL($ASSIGN);
                insertNode(parent, child);
                return 1;
            }
        }
    }
    return 0;
}

int Parser::_statReturn(TreeNode *parent) {
    if(word.symbol == $RETURN) {
        advance();
        INSERT_TERM_SYMBOL($RETURN);
        TreeNode child = createNode($Expression);
        vector<Word>::iterator t_it = word_it;
        if(_expression(&child)) {
            insertNode(parent, child);
        }
        if(word.symbol == $SEMICOLON) {
            advance();
            INSERT_TERM_SYMBOL($SEMICOLON);
            return 1;
        }
    }
    return 0;
}

int Parser::_statWhile(TreeNode *parent) {
    if(word.symbol == $WHILE) {
        advance();
        INSERT_TERM_SYMBOL($WHILE);
        if(word.symbol == $LPAREN) {
            advance();
            INSERT_TERM_SYMBOL($LPAREN);
            TreeNode child = createNode($Expression);
            if(_expression(&child)) {
                insertNode(parent, child);
                if(word.symbol == $RPAREN) {
                    advance();
                    INSERT_TERM_SYMBOL($RPAREN);
                    TreeNode child = createNode($StatBlock);
                    if(_statBlock(&child)) {
                        insertNode(parent, child);
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

int Parser::_statIf(TreeNode *parent) {
    if(word.symbol == $IF) {
        advance();
        INSERT_TERM_SYMBOL($IF);
        if(word.symbol == $LPAREN) {
            advance();
            INSERT_TERM_SYMBOL($LPAREN);
            TreeNode child = createNode($Expression);
            if(_expression(&child)) {
                insertNode(parent, child);
                TreeNode child = createNode($StatBlock);
                if(_statBlock(&child)) {
                    insertNode(parent, child);
                    if(word.symbol == $ELSE) {
                        advance();
                        INSERT_TERM_SYMBOL($ELSE);
                        TreeNode child = createNode($StatBlock);
                        if(_statBlock(&child)) {
                            insertNode(parent, child);
                        } else {
                            return 0;
                        }
                    }
                    return 1;
                }
            }
        }
    }
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
                return 0;
            }
        }
    }
    return 1;
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
                return 0;
            }
        }
        return 1;
    }
    return 0;
}

int Parser::_item(TreeNode* parent) {
    TreeNode child = createNode($Factor);
    if(_factor(&child)) {
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
                return 0;
            }
        }
        return 1;
    }
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
        } else {
            return 0;
        }
    } else if(word.symbol == $ID) {
        INSERT_TERM_SYMBOL($ID);
        advance();
        TreeNode child = createNode($Ftype);
        if(_ftype(&child)) {
            insertNode(parent, child);
            return 1;
        } else {
            return 0;
        }
    }
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
    return 0;
}

int Parser::_call(TreeNode *parent) {
    if(word.symbol == $LPAREN) {
        INSERT_TERM_SYMBOL($LPAREN);
        advance();
        TreeNode child = createNode($AparaList);
        if(_aparaList(&child)) {
            insertNode(parent, child);
            if(word.symbol == $RPAREN) {
                INSERT_TERM_SYMBOL($RPAREN);
                advance();
                return 1;
            }
        }
    }
    return 0;
}

int Parser::_aparaList(TreeNode *parent) {
    TreeNode child = createNode($Aparameter);
    vector<Word>::iterator t_it = word_it;
    if(_aparameter(&child)) {
        insertNode(parent, child);
    } else {
        retrack(t_it);
    }
    return 1;
}

int Parser::_aparameter(TreeNode *parent) {
    TreeNode child = createNode($Expression);
    if(_expression(&child)) {
        insertNode(parent, child);
        while(word.symbol == $COMMA) {
            advance();
            INSERT_TERM_SYMBOL($COMMA);
            TreeNode child = createNode($Expression);
            if(_expression(&child)) {
                insertNode(parent, child);
            } else {
                return 0;
            }
        }
        return 1;
    }
    return 0;
}

void Parser::analyze(string input) {
    Lexer::analyze(input);

    TreeNode newTree;
    newTree.parent = nullptr;
    synTree = newTree;

    word_it = Lexer::wordList.begin();
    word = *word_it;
}
