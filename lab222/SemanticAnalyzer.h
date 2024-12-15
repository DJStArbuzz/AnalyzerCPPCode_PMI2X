#pragma once

#ifndef SEMATNIK_H
#define SEMANTIK_H

#include "Token.h"
#include "Node.h"
#include <vector>
#include <fstream>

using namespace std;

class SemanticAnalyzer {
private:
    vector<string> resultList;          // ���������� ���������� ��������������  �������   �
    bool flag = false;                  // '\n' ��� ��������� ������ while
    vector<string> errorsSem;           // ������ ������ 
public:
    SemanticAnalyzer() {
        countInCode = 0;
    }
    ~SemanticAnalyzer() {
        for (int i = 0; i < resultList.size(); i++) {
            resultList[i].clear();
        }
    }
    int countInCode = 0;
    void errorSemExtra(const string& message);
    void errorSemLess(const string& message);

    void printTokensToFilesSemantic();                  // ����� ���������� �������������� ������� � ��������� ����
    void printTokensToConsoleSemantic();                // ����� ���������� �������������� ������� � �������

    // ����
    const string createBeginTrace(shared_ptr<Node>& beginNode) const;          // �������� ���� ����� Begin
    const string createMainOperatorsTrace(shared_ptr<Node>& opearorsNode, bool& flag, int& tmp) const;  // 
    const string createOperatorsTrace(shared_ptr<Node>& opearorsNode, bool& flag, int& tmp) const;      // ���������� ���� Operatots
    const string createEndTrace(shared_ptr<Node>& beginNode) const;            // ���������� ���� ����� End

    const string createDecrTrace(shared_ptr<Node>& descrNode) const;                                 // ���������� ���� Desrc
    const string createExprTrace(shared_ptr<Node>& opNode) const;                                    // ���������� ���� Expr
    const pair<string, int> createVarListTrace(shared_ptr<Node>&varListNode, string typeStr) const;  // ���������� ���� VarList
    const string createEquatingTrace(shared_ptr<Node>& equatingNode) const;                          // ���������� ���� Id = Expr
    const string createConditionTrace(shared_ptr<Node>& conditionNode) const;                        // ���������� ���� Condition
    const string createWhileTrace(shared_ptr<Node>& whileNode, int & tmp, bool& flag) const;         // ���������� ���� while
    const string createSimpExprTrace(shared_ptr<Node>& simExpNode) const;                            // ���������� ���� SimplExpr

    const vector<string>& getErrorsSemanticAn() const;
    void addSemen(const string& trace);

};

#endif 