#pragma once
// Заголовочный файл для описания узлов дерева разбора. 
#ifndef NODE_H 
#define NODE_H 

#include <string> 
#include <vector> 
#include <memory> 
#include "Token.h" 
#include <fstream>

using namespace std;

class Node {
private:
    string name;   // Имя узла 
    Token token;   // Связанный токен 
    vector<shared_ptr<Node>> children; // Дочерние узлы 
    string type;   // тип узла
    string trace;  // составление пути узла, все что идет после него
    int countElem; // счет элементов в узле
public:
    Node(const string& name, const string& type, const string& trace, int countValue, const Token& token = Token());
    Node(const string& name, const Token& token = Token()); 

    void addChild(shared_ptr<Node> child); // Добавить дочерний узел 
    const vector<shared_ptr<Node>>& getChildren() const; // Получить дочерние узлы 

    const string& getName() const; // Получить имя узла 
    const Token& getToken() const; // Получить токен 
    const string& getType() const; // Получить тип узла
    const string& getTrace() const; // Получить путь узла  
    const int getCountElem() const; // Получить кол-во элементов узла

    void setType(const string& value);  // Получить тип узла 
    void setTrace(const string& value); // Получить путь узла 
    void setCountElem(const int value); // Получить кол-во элементов узла 
};

void printTreeToFile(const shared_ptr<Node>& node, int indent, ofstream &outputFileSintax);
void printTreeToConsole(const shared_ptr<Node>& node, int indent = 0);

#endif // NODE_H
