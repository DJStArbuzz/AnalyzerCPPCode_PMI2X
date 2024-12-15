#include "Node.h" 
#include <iostream> 
#include <fstream>

using namespace std;

Node::Node(const string& name, const string& type, const string& trace, int countElem, const Token& token)
    : name(name), type(type), trace(trace), countElem(countElem),  token(token) {}

Node::Node(const string& name, const Token& token)
    : name(name), token(token) {}

void Node::addChild(shared_ptr<Node> child) {
    children.push_back(child);
}

const string& Node::getName() const {
    return name;
}

const string& Node::getType() const {
    return type;
}

const string& Node::getTrace() const {
    return trace;
}

const int Node::getCountElem() const {
    return countElem;
}

void Node::setType(const string& value) {
    type = value;
}

void Node::setTrace(const string& value) {
    trace = value;
}

void Node::setCountElem(int value) {
    countElem = value;
}

const Token& Node::getToken() const {
    return token;
}

const vector<shared_ptr<Node>>& Node::getChildren() const {
    return children;
}

// Печать дерева в консоль с отступами. 
void printTreeToConsole(const shared_ptr<Node>& node, int indent) {
    for (int i = 0; i < indent; i++)
        cout << "    ";
    cout << node->getName();
    if (node->getToken().type != END_OF_FILE) {
        string tmp = node->getToken().value;
        if (tmp == "(" || tmp == ")") {
            tmp = "Scob";
        }
        if (tmp == "{" || tmp == "}") {
            tmp = "Brace";
        }
        if (tmp == "," || tmp == ";") {
            tmp = "Separ";
        }
        if (tmp == "+" || tmp == "-") {
            tmp = "ArOp";
        }
        if (tmp.size() != 0) { cout << "    [" << tmp << "] "; }
    }
    cout << '\n';
    for (const auto& child : node->getChildren()) {
        printTreeToConsole(child, indent + 1);
    }

}

void printTreeToFile(const shared_ptr<Node>& node, int indent, ofstream & outputFileSintax) {

    for (int i = 0; i < indent; i++)
        outputFileSintax << "    ";
    outputFileSintax << node->getName();
    if (node->getToken().type != END_OF_FILE) {
        string tmp = node->getToken().value;
        if (tmp == "(" || tmp == ")") {
            tmp = "Scob";
        }
        if (tmp == "{" || tmp == "}") {
            tmp = "Brace";
        }
        if (tmp == "," || tmp == ";") {
            tmp = "Separ";
        }
        if (tmp == "+" || tmp == "-") {
            tmp = "ArOp";
        }
        if (tmp.size() != 0) outputFileSintax << "    [" << tmp << "] ";
    }
    outputFileSintax << '\n';
    for (const auto& child : node->getChildren()) {
        printTreeToFile(child, indent + 1, outputFileSintax);
    }

}