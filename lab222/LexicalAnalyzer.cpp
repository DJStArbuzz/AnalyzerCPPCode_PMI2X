#include "LexicalAnalyzer.h" 
#include <cctype> 
#include <stdexcept> 
#include <iostream>

using namespace std;

vector<string> words = { "int", "while", "return" };

LexicalAnalyzer::LexicalAnalyzer(const string& input)
    : text(input), position(0), line(1), column(1) {

}

bool LexicalAnalyzer::isDigit(const string& lexeme) {
    for (char c : lexeme)
        if (!isdigit(c))
            return false;
    return true;
}

bool LexicalAnalyzer::isValidIdentifier(const string& lexeme) const {
    for (char elem : lexeme)
        if (!isalpha(elem))
            return false;

    return true;
}

bool LexicalAnalyzer::isArithmeticOperator(const string & elem) const {
    return (elem ==  "+" || elem == "-");
}

bool LexicalAnalyzer::isArithmeticOperator(char& elem) const {
    return (elem == '+' || elem == '-');
}

bool LexicalAnalyzer::isRelationOperator(char elem) const {
    return (elem == '<' || elem == '>');
}

bool LexicalAnalyzer::isRelationOperator(const string& elem) const {
    return (elem == ">" || elem == "<" || elem == "<=" || elem == ">=" || elem == "==" || elem == "!=");
}

bool LexicalAnalyzer::isDelimiter(const string &elem) const {
    return (elem == ")" || elem == "(" || elem == "," 
        || elem == ";" || elem == "{" || elem == "}");
}


Token LexicalAnalyzer::isKeyWord(const string& lexeme2, int inde) {
    string lexeme;

    int index = 0;
    int indexKey = 0;
    int count = 0;
    bool flag = true;

    if (lexeme2[0] == 'i') {
        indexKey = 0;
    }
    else if (lexeme[1] == 'w') {
        indexKey = 1;
    }
    else if (lexeme[2] == 'r') {
        indexKey = 2;
    }
    else {
        return Token(TokenType::ID_NAME, lexeme);
    }

    for (char elem : lexeme2) {
        lexeme += elem;
        
        std::cout << lexeme << '\n';
        if (index >= words[indexKey].size()) {
            flag = false;
            break;
        }

        if (elem == words[indexKey][index]) {
            count++;
            index++;
        }

        cout << index << endl;
    }

    if (count == words[indexKey].size() && (!flag)) {
        if (indexKey == 0)
            return Token(TokenType::TYPE, lexeme);
        else if (indexKey == 1)
            return Token(TokenType::OP, lexeme);
        else
            return Token(TokenType::OP, lexeme);
    }
    else {
        if (!isValidIdentifier(lexeme))
            return Token(TokenType::ERROR, lexeme);
        else
            return Token(TokenType::ID_NAME, lexeme);
    }
}


char LexicalAnalyzer::peek() const {
    return text[position];
}

void LexicalAnalyzer::nextChar() {
    if (peek() == '\n') {
        line++;
        column = 1;
    }
    else {
        column++;
    }
    position++;
}

void LexicalAnalyzer::skipWhitespaceAndComments() {
    while (position < text.length()) {
        char current_char = peek();
        if (isspace(current_char)) {
            nextChar();
        }
        else
            break;
    }
}

Token LexicalAnalyzer::identifier() {
    int lineStart = line;
    int columnStart = column;
    string result;
    while (position < text.length() && (isalnum(peek()))) {
        result += peek();
        nextChar();
    }

    if (result == "int") {
        return Token(TYPE, result, lineStart, columnStart, 0, 0);
    }
    else if (result == "while") {
        return Token(OP, result, lineStart, columnStart, 0, 0);
    }
    else if (result == "return") {
        return Token(OP, result, lineStart, columnStart, 0, 0);
    }
    return Token(ID, result, lineStart, columnStart, 0, 0);
}


Token LexicalAnalyzer::number() {
    int lineStart = line;
    int columnStart = column;
    string result;
    while (position < text.length() && isdigit(peek())) {
        result += peek();
        nextChar();
    }
    return Token(CONST, result, lineStart, columnStart);
}


Token LexicalAnalyzer::symbol() {
    int lineStart = line;
    int columnStart = column;
    char currElemChar = peek();
    string value(1, currElemChar);
    nextChar();

    if ((currElemChar == '=' || currElemChar == '!' 
        || (isRelationOperator(currElemChar))) &&
        position < text.length() && peek() == '=') {
        value += peek();
        nextChar();
    }

    if (isDelimiter(value)) {
        return Token(DELIMITER, value, lineStart, columnStart);
    }
    if (isArithmeticOperator(value)) {
        return Token(OP, value, lineStart, columnStart);
    }

    if (value == "=") {
        return Token(OP, value, lineStart, columnStart);
    }
    else{
      return Token(ROP, value, lineStart, columnStart);
    }
}


Token LexicalAnalyzer::getNextToken() {
    while (position < text.length()) {
        skipWhitespaceAndComments();
        if (position >= text.length()) break;

        char current_char = peek();
        if (isalpha(current_char)) {
            Token tmp = identifier();
            return tmp;
        }
        else if (isdigit(current_char)) {
            Token tmp = number();
            return tmp;
        }
        else {
            Token tmp = symbol();
            return tmp;
        }
    }
}
