#pragma once
#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "Token.h"
#include <fstream>

using namespace std;

class HashTable {
public:
    HashTable() : tokenCount(0) {
        for (int i = 0; i < HASH_TABLE_SIZE; i++) {
            hashTable[i] = nullptr;
        }
        for (int i = 0; i < MAX_TOKENS; i++) {
            tokenSequence[i] = nullptr;
        }
    }

    ~HashTable() {
        for (int i = 0; i < HASH_TABLE_SIZE; i++) {
            delete hashTable[i];
        }
        for (int i = 0; i < MAX_TOKENS; i++) {
            delete tokenSequence[i];
        }
    }

    static const int MAX_TOKENS = 10000; 
    Token* tokenSequence[MAX_TOKENS]; 
    void addToken(const Token& token);
    int hashFunction(const string& lexeme) const;
    int countInCode = 0;

    // output arrays to the console and text files
    void printTokensToFiles();
    void printTokensToConsole();

    int getIndex(int i) {
        return tokenSequence[i]->getIndex();
    }

    bool search(string& token);


private:
    static const int HASH_TABLE_SIZE = 1000;
    static const int HASH_SIZE = 10000;  // size max
    Token* hashTable[HASH_TABLE_SIZE];

    int tokenCount;

};

#endif 