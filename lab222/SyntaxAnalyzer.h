#pragma once
// Заголовочный файл для парсера. Определяет интерфейс класса Parser. 
#ifndef PARSER_H 
#define PARSER_H 

#include "LexicalAnalyzer.h" 
#include "Node.h" 
#include "SemanticAnalyzer.h"
#include <vector> 
#include <memory> 

using namespace std;

// Класс Parser осуществляет синтаксический разбор на основе токенов из Lexer. 
class SyntaxAnalyzer {
public:
    SyntaxAnalyzer(LexicalAnalyzer& lexAn, SemanticAnalyzer& semAn); // Конструктор 
    
    shared_ptr<Node> parse(); // Основной метод разбора 
    const vector<string>& getErrorsSyntaxAn() const; // Получить список ошибок 
    int countA = 0;

private:
    Token tokenCurr; // Текущий токен 
    LexicalAnalyzer& lexicallAn; // Лексер 
    SemanticAnalyzer& semanticallAn;
    vector<string> errorsSyntax; // Список ошибок 

    // Вспомогательные методы 
    void getNextTokenSyn();
    void error(const string& message);

    // Разделение элементов
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
