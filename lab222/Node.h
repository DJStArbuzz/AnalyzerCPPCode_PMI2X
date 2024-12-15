#pragma once
// ������������ ���� ��� �������� ����� ������ �������. 
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
    string name;   // ��� ���� 
    Token token;   // ��������� ����� 
    vector<shared_ptr<Node>> children; // �������� ���� 
    string type;   // ��� ����
    string trace;  // ����������� ���� ����, ��� ��� ���� ����� ����
    int countElem; // ���� ��������� � ����
public:
    Node(const string& name, const string& type, const string& trace, int countValue, const Token& token = Token());
    Node(const string& name, const Token& token = Token()); 

    void addChild(shared_ptr<Node> child); // �������� �������� ���� 
    const vector<shared_ptr<Node>>& getChildren() const; // �������� �������� ���� 

    const string& getName() const; // �������� ��� ���� 
    const Token& getToken() const; // �������� ����� 
    const string& getType() const; // �������� ��� ����
    const string& getTrace() const; // �������� ���� ����  
    const int getCountElem() const; // �������� ���-�� ��������� ����

    void setType(const string& value);  // �������� ��� ���� 
    void setTrace(const string& value); // �������� ���� ���� 
    void setCountElem(const int value); // �������� ���-�� ��������� ���� 
};

void printTreeToFile(const shared_ptr<Node>& node, int indent, ofstream &outputFileSintax);
void printTreeToConsole(const shared_ptr<Node>& node, int indent = 0);

#endif // NODE_H
