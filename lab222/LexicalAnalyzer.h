#pragma once
// Lexer.h 
#ifndef LEXER_H 
#define LEXER_H 

#include <string> 
#include <vector> 
#include "Token.h" 
#include "HashTable.h"


using namespace std;

class LexicalAnalyzer {
public:
    LexicalAnalyzer(const string& input); // Конструктор, принимает строку с исходным текстом. 
    Token getNextToken();                 // Возвращает следующий токен из потока. 

    HashTable tokenList;                  // Хэш - таблица
    string text;                          // Искомый текст
    size_t position;                      // Текущая позиция в тексте 
    int column;                           // Текущий столбец 
    int line;                             // Текущая строка 
    int currIndex;                        // Текущий индекс токена

private:
    bool isValidIdentifier(const string& lexeme) const;    // проверка на правильность id
    Token isKeyWord(const string& lexeme, int ind); // проверка на for int, while, return
    char peek() const;                // Возвращает текущий символ
    void nextChar();                  // Переходит к следующему символу
    void skipWhitespaceAndComments(); // Пропускает пробелы и комментарии
    Token identifier();               // Выделяет идентификатор (слова, переменные)
    Token number();                   // Выделяет числовую константу
    Token symbol();                   // Выделяет символ (оператор или разделитель)

    bool isArithmeticOperator(const string& elem) const;   // + -                     
    bool isArithmeticOperator(char& elem) const;           // + -       
    bool isRelationOperator(char elem) const;              // < > <= >= != ==
    bool isRelationOperator(const string &elem) const;     // < > <= >= != ==
    bool isDelimiter(const string& elem) const;            // , ;
    bool isDigit(const string& lexeme);                    // 0-9
};

#endif // LEXER_H
