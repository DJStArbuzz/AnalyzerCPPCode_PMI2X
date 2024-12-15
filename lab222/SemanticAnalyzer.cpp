#include <stdexcept>
#include <iostream>
#include <fstream>
#include "SemanticAnalyzer.h"

using namespace std;

void SemanticAnalyzer::printTokensToFilesSemantic() {
	ofstream outputFileSemantic("outputSemantic.txt");
	for (int i = 0; i < resultList.size(); i++) {
		outputFileSemantic << resultList[i] << '\n';
	}
	outputFileSemantic.close();
}

void SemanticAnalyzer::printTokensToConsoleSemantic() {
	for (int i = 0; i < resultList.size(); i++) {
		cout << resultList[i] << '\n';
	}
}

const vector<string>& SemanticAnalyzer::getErrorsSemanticAn() const {
	return errorsSem;
}

void SemanticAnalyzer::errorSemExtra(const string& message) {
	string errorMsg = "Повторное объявление элемента: " + message + ".";;
	errorsSem.push_back(errorMsg);
	throw runtime_error(errorMsg);
}

void SemanticAnalyzer::errorSemLess(const string& message) {
	string errorMsg = "Не объявлен элемент: " + message + ".";
	errorsSem.push_back(errorMsg);
	throw runtime_error(errorMsg);
}

const pair<string, int> SemanticAnalyzer::createVarListTrace(shared_ptr<Node>& begin, string type) const {
	string varListNodeTrace = " ";
	int count = 0;

	while (begin->getChildren().size() != 0) {
		count++;
		varListNodeTrace += (" " + begin->getChildren()[0]->getToken().value);

		if (begin->getChildren().size() != 3)
			break;
		else 
			begin = begin->getChildren()[2];
	} 

	return make_pair(varListNodeTrace.erase(0, 2), count);
}

const string SemanticAnalyzer::createDecrTrace(shared_ptr<Node>& descrNode) const{
	string result = "";
	vector<shared_ptr<Node>> listKids = descrNode->getChildren();
	auto typeDescrNode = listKids[0];
	auto varListDescrNode = listKids[1];

	typeDescrNode->setType(typeDescrNode->getChildren()[0]->getType());

	string typeRes = typeDescrNode->getType();
	pair<string, int> varListTraceRes = createVarListTrace(varListDescrNode, typeRes);
	string varListTrace = varListTraceRes.first;
	int varListCount = varListTraceRes.second;

	result = typeRes + " " + varListTrace + " " + to_string(1 + varListCount) + " DECL";
	return result;
}

const string SemanticAnalyzer::createEquatingTrace(shared_ptr<Node>& equatingNode) const {
	string resEquatingTrace = "";
	vector<shared_ptr<Node>> listKids = equatingNode->getChildren();
	
	string partId = listKids[0]->getToken().value;
	string partEqual = listKids[1]->getToken().value;
	string partExpr = createExprTrace(listKids[2]);

	resEquatingTrace = partId + " " + partExpr + " " + partEqual;
	return resEquatingTrace;
}

const string SemanticAnalyzer::createMainOperatorsTrace(shared_ptr<Node>& operatorsNode, bool &flag, int &tmp) const {
	vector<shared_ptr<Node>> listKids = operatorsNode->getChildren();

	string res = "";
	for (int i = 0; i < listKids.size(); i++) {
		shared_ptr<Node> op = listKids[i];
		if (op->getChildren()[0]->getName() == "Id") 
			res += createExprTrace(op);

		else {
			flag = false;
			res += createWhileTrace(op, tmp, flag);
			flag = true;
		}
		res += '\n';
	}
	return res;
}

const string SemanticAnalyzer::createOperatorsTrace(shared_ptr<Node>& operatorsNode, bool& flag, int& tmp) const {
	vector<shared_ptr<Node>> listKids = operatorsNode->getChildren();

	string res = "";
	for (int i = 0; i < listKids.size(); i++) {
		shared_ptr<Node> op = listKids[i];
		if (op->getChildren()[0]->getName() == "Id") {
			res += createExprTrace(op);
		}
		else {
			flag = false;
			res += createWhileTrace(op, tmp, flag);
		}
	}
	return res;
}


const string SemanticAnalyzer::createExprTrace(shared_ptr<Node>& exprNode) const {
	vector<shared_ptr<Node>> listKids = exprNode->getChildren();	
	string resultExpr = "";
	string firstSimple;
	string oper;
	string secondSimple;

	if (listKids[0]->getName() == "Expr")
		firstSimple += createExprTrace(listKids[0]);
	else {
		if (listKids[0]->getChildren().size() == 0) {

			firstSimple = listKids[0]->getToken().value;
		}
		else {
			shared_ptr<Node> tmp = listKids[0];
			shared_ptr<Node> tmp2 = tmp->getChildren()[1];
			firstSimple += createExprTrace(tmp2);
		}
	}

	oper = listKids[1]->getToken().value;

	if (listKids[2]->getName() == "Expr")
		secondSimple += createExprTrace(listKids[2]);
	else {
		if (listKids[2]->getChildren().size() == 0)
			secondSimple = listKids[2]->getToken().value;
		else {
			shared_ptr<Node> tmp = listKids[2];
			shared_ptr<Node> tmp2 = tmp->getChildren()[1];
			secondSimple += createExprTrace(tmp2);
		}
	}

	resultExpr = firstSimple + " " + secondSimple + " " + oper + " ";
	return resultExpr;
}

const string SemanticAnalyzer::createSimpExprTrace(  shared_ptr<Node>& exprNode) const {
	vector<shared_ptr<Node>> listKids = exprNode->getChildren();
	string resultExpr = "";

	if (listKids.size() == 0)
		resultExpr += exprNode->getToken().value;
	else 
		resultExpr += createSimpExprTrace(listKids[1]) + " ";

	return resultExpr;

}

const string SemanticAnalyzer::createConditionTrace(shared_ptr<Node>& conditionNode) const {
	string resultCond = "";
	vector<shared_ptr<Node>> listKids = conditionNode->getChildren();
	string relation = listKids[1]->getToken().value;
	string firstOperand, secondOperand;
	
	if (listKids[0]->getName() == "Expr")
		firstOperand = createExprTrace(listKids[0]);
	else
		firstOperand = createSimpExprTrace(listKids[0]);

	if (listKids[2]->getName() == "Expr")
		firstOperand = createExprTrace(listKids[2]);
	else 
		secondOperand = createSimpExprTrace(listKids[2]);

	resultCond = firstOperand + " " + secondOperand + " " + relation;
	return resultCond;

}

const string SemanticAnalyzer::createWhileTrace(shared_ptr<Node>& whileNode, int &tmp, bool& flag) const {
	vector<shared_ptr<Node>> listKids = whileNode->getChildren();
	shared_ptr<Node> condition = listKids[2];
	shared_ptr<Node> operators = listKids[5];

	string resultCondition = createConditionTrace(condition);
	shared_ptr<Node> op = operators->getChildren()[0];

	string oper = createOperatorsTrace(operators, flag, tmp);

	string firstCond = "m" + to_string(tmp);
	string secondCond = "m" + to_string(tmp + 1);

	string resultWhile = firstCond + " DEFL " + resultCondition + " " + secondCond + " " + oper + " " + firstCond
		+ " BRL " + secondCond + " DEFL ";
	tmp += 2;
	return resultWhile;
}


const string SemanticAnalyzer::createBeginTrace(shared_ptr<Node>& beginNode) const {
	vector<shared_ptr<Node>> listKids = beginNode->getChildren();
	string type = listKids[0]->getChildren()[0]->getToken().value;
	string id = listKids[1]->getToken().value;

	string beginResult = type  + " " + id + " functionBegin";
	return beginResult;
}

const string SemanticAnalyzer::createEndTrace(shared_ptr<Node>& endNode) const {
	vector<shared_ptr<Node>> listKids = endNode->getChildren();
	string oper = listKids[0]->getName();
	string id = listKids[1]->getToken().value;

	string endResult = oper + " " + id + " functionEnd";
	return endResult;
}

void SemanticAnalyzer::addSemen(const string& trace) {
	resultList.push_back(trace);
}
