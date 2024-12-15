#include "SyntaxAnalyzer.h"
#include <stdexcept>
#include <iostream>

using namespace std;

bool SyntaxAnalyzer::isRelationOperator(const string& op) {
    return op == "==" || op == "!=" || op == "<" || op == ">" || op == "<=" || op == ">=";
}

SyntaxAnalyzer::SyntaxAnalyzer(LexicalAnalyzer& lexAn, SemanticAnalyzer& semAn)
    : lexicallAn(lexAn), semanticallAn(semAn) {
    getNextTokenSyn();
}

void SyntaxAnalyzer::getNextTokenSyn() {
    tokenCurr = lexicallAn.getNextToken();
}

void SyntaxAnalyzer::error(const string& message) {
    string errorMsg = "Ошибка на линии " + to_string(tokenCurr.line) +
        ", столбец " + to_string(tokenCurr.column) + ": " + message;
    errorsSyntax.push_back(errorMsg);
    throw runtime_error(errorMsg);
}

const vector<string>& SyntaxAnalyzer::getErrorsSyntaxAn() const {
    return errorsSyntax;
}

shared_ptr<Node> SyntaxAnalyzer::parse() {
    return baseOfTheTree();
}


shared_ptr<Node> SyntaxAnalyzer::baseOfTheTree() {
    shared_ptr<Node> beginNode = beginPart();
    shared_ptr<Node> descriptionsNode = descriptionsPart();
    shared_ptr<Node> operatorsNode = operatorsPart();
    shared_ptr<Node> endNode = endPart();

    if (tokenCurr.value != "}") {
        error("Ожидалось '}' в конце функции");
        return nullptr;
    }
    else {
        lexicallAn.tokenList.addToken(tokenCurr);
        endNode->addChild(make_shared<Node>(tokenCurr.value, "", "", 1, tokenCurr));

        shared_ptr<Node> functionNode = make_shared<Node>("Function");
        functionNode->addChild(beginNode);
        functionNode->addChild(descriptionsNode);
        functionNode->addChild(operatorsNode);

        bool flag = true;
        int tmp = 0;
        string res2Op = semanticallAn.createMainOperatorsTrace(operatorsNode, flag, tmp);
        semanticallAn.addSemen(res2Op);

        functionNode->addChild(endNode);

        string resEnd = semanticallAn.createEndTrace(endNode);
        semanticallAn.addSemen(resEnd);

        return functionNode;
    }
}

// Begin → Type FunctionName ( ) {
shared_ptr<Node> SyntaxAnalyzer::beginPart() {
    shared_ptr<Node> typeNode = typePart();
    shared_ptr<Node> functionNameNode = funcNamePart();
    if (tokenCurr.value == "(") {
        Token openParenToken = tokenCurr;
        getNextTokenSyn();
        if (tokenCurr.value == ")") {
            Token closeParenToken = tokenCurr;
            getNextTokenSyn();
            if (tokenCurr.value == "{") {
                Token openBraceToken = tokenCurr;
                getNextTokenSyn();
                shared_ptr<Node> beginNode = make_shared<Node>("Begin");
                beginNode->addChild(typeNode);
                beginNode->addChild(functionNameNode);
                beginNode->addChild(make_shared<Node>("{", "", "", 1, openBraceToken)); // Add the '{' node here

                lexicallAn.tokenList.addToken(openParenToken);
                lexicallAn.tokenList.addToken(closeParenToken);
                lexicallAn.tokenList.addToken(openBraceToken);

                string resEnd = semanticallAn.createBeginTrace(beginNode);
                semanticallAn.addSemen(resEnd);

                return beginNode;
            }
            else {
                error("Ожидалось '{' после ')'");
            }
        }
        else {
            error("Ожидалось ')' после '('");
        }
    }
    else {
        error("Ожидалось '(' после наименования функции");
    }
    return nullptr;
}


// FunctionName → Id
shared_ptr<Node> SyntaxAnalyzer::funcNamePart() {
    if (tokenCurr.type != ID) {
        error("Ожидалось наименование функции.");
        return nullptr;
    }
    else {
        Token idToken = tokenCurr;
        getNextTokenSyn();
        lexicallAn.tokenList.addToken(idToken);
        return make_shared<Node>("FunctionName", idToken);
    }
}


// End → return Id ; }
shared_ptr<Node> SyntaxAnalyzer::endPart() {
    if (tokenCurr.value != "return") {
        error("Оператор возврата 'return' не найден");
        return nullptr;
    }
    Token returnToken = tokenCurr;
    getNextTokenSyn();
    if (tokenCurr.type == ID) {
        Token idToken = tokenCurr;
        getNextTokenSyn();
        if (tokenCurr.value == ";") {
            Token finToken = tokenCurr;
            getNextTokenSyn();
            shared_ptr<Node> endNode = make_shared<Node>("End");
            endNode->addChild(make_shared<Node>(returnToken.value, "", "", 1, returnToken));

            lexicallAn.tokenList.addToken(returnToken);
            if (!lexicallAn.tokenList.search(idToken.value)) {
                semanticallAn.errorSemLess(idToken.value);
            }
            lexicallAn.tokenList.addToken(idToken);
            lexicallAn.tokenList.addToken(finToken);

            endNode->addChild(make_shared<Node>("Id", "", "", 1, idToken));
            endNode->addChild(make_shared<Node>(";", "", "", 1, finToken));
            return endNode;
        }
        else {
            error("Ожидалось ';' перед оператором возврата 'return'.");
            return nullptr;
        }
    }
    else {
        error("Ожидалось полная конструкция с 'return.'");
        return nullptr;
    }
    return nullptr;
}

// Descriptions → Descr Descriptions | ε
shared_ptr<Node> SyntaxAnalyzer::descriptionsPart() {
    shared_ptr<Node> descriptionsNode = make_shared<Node>("Descriptions");
    while (tokenCurr.value == "int") {
        shared_ptr<Node> descrNode = descrPart();
        descriptionsNode->addChild(descrNode);
    }
    return descriptionsNode;
}

// Descr → Type VarList ;
shared_ptr<Node> SyntaxAnalyzer::descrPart() {
    shared_ptr<Node> typeNode = typePart();
    shared_ptr<Node>  varListNode = varListBegPart();
    if (tokenCurr.value == ";") {
        Token semicolonToken = tokenCurr;
        getNextTokenSyn();
        shared_ptr<Node> descrNode = make_shared<Node>("Descr");
        descrNode->addChild(typeNode);

        lexicallAn.tokenList.addToken(semicolonToken);
        descrNode->addChild(varListNode);
        descrNode->addChild(make_shared<Node>(";", "", "", 1, semicolonToken));

        string resDescr = semanticallAn.createDecrTrace(descrNode);
        semanticallAn.addSemen(resDescr);
        return descrNode;
    }
    else {
        error("Потерян ';' после объявления переменных.");
        return nullptr;
    }
}

// VarList → Id VarList'
shared_ptr<Node> SyntaxAnalyzer::varListBegPart() {
    shared_ptr<Node> varListNode = make_shared<Node>("VarList");
    if (tokenCurr.type == ID) {
        Token idToken = tokenCurr;

        if (lexicallAn.tokenList.search(idToken.value)) {
            semanticallAn.errorSemExtra(idToken.value);
        }
        lexicallAn.tokenList.addToken(idToken);
        getNextTokenSyn();
        varListNode->addChild(make_shared<Node>("Id", idToken));
        
        if (tokenCurr.value == ",") {
            Token delToken = tokenCurr;
            getNextTokenSyn();
            if (tokenCurr.type == ID) {
                varListNode->addChild(make_shared<Node>(",", "", "", 1, delToken));
                
                shared_ptr<Node> tmp = varListPart();
                varListNode->addChild(tmp);

                lexicallAn.tokenList.addToken(delToken);

            }
            else {
                error("Ожидалась переменная ','");
                return nullptr;
            }
        }
        return varListNode;
    }
    else {
        error("Потеряна переменная при объявлении");
        return nullptr;
    }
}

// Доп-проверка для varList (в случае если объявлено много переменных)
shared_ptr<Node> SyntaxAnalyzer::varListPart() {
    shared_ptr<Node> varListNode = make_shared<Node>("VarList");
    if (tokenCurr.type == ID) {
        Token idToken = tokenCurr;

        if (lexicallAn.tokenList.search(idToken.value)) {
            semanticallAn.errorSemExtra(idToken.value);
        }
        lexicallAn.tokenList.addToken(idToken);

        getNextTokenSyn();
        varListNode->addChild(make_shared<Node>("Id", idToken));

        if (tokenCurr.value == ",") {
            Token delToken = tokenCurr;
            getNextTokenSyn();
            if (tokenCurr.type == ID) {
                varListNode->addChild(make_shared<Node>(",", "", "", 1, delToken));

                shared_ptr<Node> tmp = varListPart();
                varListNode->addChild(tmp);
                lexicallAn.tokenList.addToken(delToken);
            }
            else {
                error("Expected identifier after ','");
                return nullptr;
            }
        }
        return varListNode;
    }
    else {
        error("Ожидалась переменная.");
        return nullptr;
    }
}

// Type → int
shared_ptr<Node> SyntaxAnalyzer::typePart() {
    if (tokenCurr.value == "int") {
        Token intToken = tokenCurr;
        
        lexicallAn.tokenList.addToken(intToken);
        getNextTokenSyn();
        shared_ptr<Node> typeNode = make_shared<Node>("Type");

        typeNode->addChild(make_shared<Node>(intToken.value, "int", "int", 1, intToken));
        return typeNode;
    }
    else {
        error("Ожидался единственно-доступный тип 'int'");
        return nullptr;
    }
}

// Operators → Op Operators | ε
shared_ptr<Node> SyntaxAnalyzer::operatorsPart() {
    string trace = "";
    shared_ptr<Node> operatorsNode = make_shared<Node>("Operators");

    while (tokenCurr.type != END_OF_FILE && tokenCurr.value != "}") {
        shared_ptr<Node> opNode = opPart();
        if (opNode == nullptr) 
        { 
            break; 
        }
        operatorsNode->addChild(opNode);
    }

    return operatorsNode;
}

// Op → Id = Expr ; | for ( Init; Condition ; Expr ) { Operators }
shared_ptr<Node> SyntaxAnalyzer::opPart() {
    if (tokenCurr.type == ID) {
        // Id = Expr ;
        Token idToken = tokenCurr;

        if (!lexicallAn.tokenList.search(idToken.value)) {
            semanticallAn.errorSemLess(idToken.value);
        }

        lexicallAn.tokenList.addToken(idToken);

        getNextTokenSyn();
        if (tokenCurr.value == "=") {
            Token equalToken = tokenCurr;
            getNextTokenSyn();
            shared_ptr<Node> exprNode = exprPart();
            if (tokenCurr.value == ";") {
                Token semicolonToken = tokenCurr;
                getNextTokenSyn();
                shared_ptr<Node> opNode = make_shared<Node>("Op");
                opNode->addChild(make_shared<Node>("Id", "", "", 1, idToken));

                lexicallAn.tokenList.addToken(equalToken);

                opNode->addChild(make_shared<Node>("=", "", "", 1, equalToken));

                opNode->addChild(exprNode);

                lexicallAn.tokenList.addToken(semicolonToken);
                opNode->addChild(make_shared<Node>(";", "", "", 1, semicolonToken));
                return opNode;
            }
            else {
                error("Ожидалось завершающее ';'.");
                return nullptr;
            }
        }
        else {
            error("Ожидалось '=' перед выражением.");
            return nullptr;
        }
    }
    else if (tokenCurr.value == "while") {
        Token whileToken = tokenCurr;
        getNextTokenSyn();
        if (tokenCurr.value == "(") {
            Token openParenToken = tokenCurr;
            getNextTokenSyn();  
            shared_ptr<Node> conditionNode = conditionPart();
            if (tokenCurr.value == ")") {
                Token closeParenToken = tokenCurr;
                getNextTokenSyn();
                if (tokenCurr.value == "{") {
                    Token openBraceToken = tokenCurr;
                    getNextTokenSyn();
                    shared_ptr<Node> operatorsNode = operatorsPart();
                    if (tokenCurr.value == "}") {
                        Token closeBraceToken = tokenCurr;
                        getNextTokenSyn();
                        shared_ptr<Node> opNode = make_shared<Node>("Op");
                        opNode->addChild(make_shared<Node>(whileToken.value, "", "", 1, whileToken));
                        opNode->addChild(make_shared<Node>("(", "", "", 1, openParenToken));

                        opNode->addChild(conditionNode);
                        opNode->addChild(make_shared<Node>(")", "", "", 1, closeParenToken));
                        opNode->addChild(make_shared<Node>("{", "", "", 1, openBraceToken));
                        opNode->addChild(operatorsNode);

                        lexicallAn.tokenList.addToken(whileToken);
                        lexicallAn.tokenList.addToken(openParenToken);
                        lexicallAn.tokenList.addToken(closeParenToken);
                        lexicallAn.tokenList.addToken(openBraceToken);

                        shared_ptr<Node> op = operatorsNode->getChildren()[0];
                        opNode->addChild(make_shared<Node>("}", "", "", 1, closeBraceToken));
                        return opNode;
                    }
                    else {
                        error("Ожидалось завершающее '}'."); 
                        return nullptr;
                    }
                }
                else {
                    error("Ожидалось '{' после ')'."); 
                    return nullptr;
                }
            }
            else {
                error("Ожидалось ')' после условия."); 
                return nullptr;
            }
        }
        else {
            error("Ожидалось '(' после 'while'."); 
            return nullptr;
        }
    }
    else if (tokenCurr.value == "return") {
        return nullptr;
    }
    else {
        error("Неизвестная операция."); 
        return nullptr;
    }
}

// Expr → SimpleExpr Expr'
shared_ptr<Node> SyntaxAnalyzer::exprPart() {
    shared_ptr<Node> exprLeftPartNode = simpleExpPart();

    while (tokenCurr.value == "+" || tokenCurr.value == "-") {
        Token opToken = tokenCurr;
        lexicallAn.tokenList.addToken(opToken);
        getNextTokenSyn();
        shared_ptr<Node> exprNode = make_shared<Node>("Expr");
        exprNode->addChild(exprLeftPartNode);
        exprNode->addChild(make_shared<Node>(opToken.value, "", "", 1, opToken));
        shared_ptr<Node> exprRightPartNode = simpleExpPart();
        exprNode->addChild(exprRightPartNode);

        exprLeftPartNode = exprNode;
    }
    return exprLeftPartNode;
}

// SimpleExpr → Id | Const | ( Expr )
shared_ptr<Node> SyntaxAnalyzer::simpleExpPart() {
    if (tokenCurr.type == ID) {
        Token idToken = tokenCurr;

        if (!lexicallAn.tokenList.search(idToken.value)) {
            semanticallAn.errorSemLess(idToken.value);
        }
        lexicallAn.tokenList.addToken(idToken);

        getNextTokenSyn();
        return make_shared<Node>("SimpleExpr", idToken);
    }
    else if (tokenCurr.type == CONST) {
        Token constToken = tokenCurr;

        lexicallAn.tokenList.addToken(tokenCurr);
        getNextTokenSyn();
        return make_shared<Node>("SimpleExpr", constToken);
    }
    else if (tokenCurr.value == "(") {
        Token openParenToken = tokenCurr;
        getNextTokenSyn();
        shared_ptr<Node> exprNode = exprPart();
        if (tokenCurr.value == ")") {
            Token closeParenToken = tokenCurr;
            getNextTokenSyn();
            shared_ptr<Node> simpleExprNode = make_shared<Node>("SimpleExpr");

            lexicallAn.tokenList.addToken(openParenToken);
            simpleExprNode->addChild(make_shared<Node>("(", "", "", 1, openParenToken));
            lexicallAn.tokenList.addToken(closeParenToken);
            simpleExprNode->addChild(exprNode);
            simpleExprNode->addChild(make_shared<Node>(")", "", "", 1, closeParenToken));

            return simpleExprNode;
        }
        else {
            error("Ожидалось ')' после '('.");
            return nullptr;
        }
    }
    else {
        error("Ожидалось выражение.");
        return nullptr;
    }
}

// Condition → Expr RelationOperators Expr
shared_ptr<Node> SyntaxAnalyzer::conditionPart() {
    shared_ptr<Node> leftExpr = exprPart();
    if (isRelationOperator(tokenCurr.value)) {
        Token relOpToken = tokenCurr;
        getNextTokenSyn();
        shared_ptr<Node> rightExpr = exprPart();
        shared_ptr<Node> condNode = make_shared<Node>("Condition");
        condNode->addChild(leftExpr);
        lexicallAn.tokenList.addToken(relOpToken);
        condNode->addChild(make_shared<Node>("RelationsOperators", "", "", 1, relOpToken));
        condNode->addChild(rightExpr);
        return condNode;
    }
    else {
        error("Потерян оператор сравнения.");
        return nullptr;
    }
}