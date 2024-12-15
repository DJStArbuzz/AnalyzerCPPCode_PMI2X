#pragma once
// Заголовочный файл описывает структуру токенов, которые генерирует лексер. 
#ifndef TOKEN_H 
#define TOKEN_H 

#include <string> 

using namespace std;

// Типы токенов, соответствующие различным элементам языка. 
enum TokenType {
    ID,             // Идентификаторы 
    PROGRAM,        // Program -> Begin Descriptions Operators End
    BEGIN,          // Type FunctionName() {
    END,            // return Id ; }
    DESCRIPTIONS,   // Descriptions -> Descr | Descr Descriptions
    DESCR,          // Descr -> Type VarList
    TYPE,           // Type -> INTEGER
    VARLIST,        // VarList -> Id | Id , VarList
    OP,             // Id = Expr ; | while ( Condition ) { Operators }
    OPERATORS,      // Op | Op Operators
    ID_NAME,        // a-z,A-Z
    CONST,          // 0-9
    OPERATOR,       // Op | Op Operators
    DELIMITER,      // ( ) [ ] { } 
    ERROR,          // Ошибки
    THEN,           // Then statement       
    SIMPLEEXPR,     // Id | Const | ( Expr )
    EXPR,           // Expr → SimpleExpr | SimpleExpr + Expr | SimpleExpr - Expr
    ROP,            // == | != | < | > | <= | >=
    END_OF_FILE,    // Конец файла 
    UNKNOWN
};

struct Token {
    TokenType type; // Тип токена 
    string value;   // Текстовое значение токена 
    int line;       // Номер строки 
    int column;     // Номер столбца 
    int index;      // Индекс токена для Hash - таблицы
    int hash;       // Hash токена

    Token(TokenType type = UNKNOWN, const string& value = "", int line = 0, int column = 0, int index=0, int hash = 0)
        : type(type), value(value), line(line), column(column), index(index), hash(hash) {}

    int getIndex();   // Возврат индекса токена
    string getType(); // Возврат типа токена
};

#endif // TOKEN_H
