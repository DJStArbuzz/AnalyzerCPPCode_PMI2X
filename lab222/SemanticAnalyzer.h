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
    vector<string> resultList;          // Сохранение результата семантического  анализа   э
    bool flag = false;                  // '\n' для элементов внутри while
    vector<string> errorsSem;           // Список ошибок 
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

    void printTokensToFilesSemantic();                  // Вывод результата семантического анализа в текстовый файл
    void printTokensToConsoleSemantic();                // Вывод результата семантического анализа в консоль

    // Пути
    const string createBeginTrace(shared_ptr<Node>& beginNode) const;          // Сохрание пути части Begin
    const string createMainOperatorsTrace(shared_ptr<Node>& opearorsNode, bool& flag, int& tmp) const;  // 
    const string createOperatorsTrace(shared_ptr<Node>& opearorsNode, bool& flag, int& tmp) const;      // Сохранение пути Operatots
    const string createEndTrace(shared_ptr<Node>& beginNode) const;            // Сохранение пути части End

    const string createDecrTrace(shared_ptr<Node>& descrNode) const;                                 // Сохранение пути Desrc
    const string createExprTrace(shared_ptr<Node>& opNode) const;                                    // Сохранение пути Expr
    const pair<string, int> createVarListTrace(shared_ptr<Node>&varListNode, string typeStr) const;  // Сохранение пути VarList
    const string createEquatingTrace(shared_ptr<Node>& equatingNode) const;                          // Сохранение пути Id = Expr
    const string createConditionTrace(shared_ptr<Node>& conditionNode) const;                        // Сохранение пути Condition
    const string createWhileTrace(shared_ptr<Node>& whileNode, int & tmp, bool& flag) const;         // Сохранение пути while
    const string createSimpExprTrace(shared_ptr<Node>& simExpNode) const;                            // Сохранение пути SimplExpr

    const vector<string>& getErrorsSemanticAn() const;
    void addSemen(const string& trace);

};

#endif 