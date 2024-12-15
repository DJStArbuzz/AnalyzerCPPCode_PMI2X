#pragma once
// ������������ ���� ��� �������. ���������� ��������� ������ Parser. 
#ifndef PARSER_H 
#define PARSER_H 

#include "LexicalAnalyzer.h" 
#include "Node.h" 
#include "SemanticAnalyzer.h"
#include <vector> 
#include <memory> 

using namespace std;

// ����� Parser ������������ �������������� ������ �� ������ ������� �� Lexer. 
class SyntaxAnalyzer {
public:
    SyntaxAnalyzer(LexicalAnalyzer& lexAn, SemanticAnalyzer& semAn); // ����������� 
    
    shared_ptr<Node> parse(); // �������� ����� ������� 
    const vector<string>& getErrorsSyntaxAn() const; // �������� ������ ������ 
    int countA = 0;

private:
    Token tokenCurr; // ������� ����� 
    LexicalAnalyzer& lexicallAn; // ������ 
    SemanticAnalyzer& semanticallAn;
    vector<string> errorsSyntax; // ������ ������ 

    // ��������������� ������ 
    void getNextTokenSyn();
    void error(const string& message);

    // ���������� ���������
    shared_ptr<Node> baseOfTheTree();

    // Begin
    shared_ptr<Node> beginPart();
    shared_ptr<Node> funcNamePart();

    // Descriptions
    shared_ptr<Node> descriptionsPart();
    shared_ptr<Node> typePart();
    shared_ptr<Node> descrPart();
    shared_ptr<Node> varListBegPart();
    shared_ptr<Node> varListPart();

    // Operators
    shared_ptr<Node> operatorsPart();
    shared_ptr<Node> opPart();
    shared_ptr<Node> simpleExpPart();
    shared_ptr<Node> exprPart();
    shared_ptr<Node> conditionPart();

    // End
    shared_ptr<Node> endPart();
    bool isRelationOperator(const string& op);  // ROP
};

#endif // PARSER_H
