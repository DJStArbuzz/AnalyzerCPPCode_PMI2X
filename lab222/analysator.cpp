#include <iostream> 
#include <fstream> 
#include <sstream> 
#include "LexicalAnalyzer.h" 
#include "SyntaxAnalyzer.h" 
#include "Node.h" 
#include "SemanticAnalyzer.h"

using namespace std;

int main() {
    setlocale(LC_ALL, "RUS");
    string finalMsg = "\nНажмите, чтобы завершить сценарий...\n";
    ifstream inputFile("input.txt");

    if (!inputFile) {
        cout << "Ошибка открытия файла: input.txt\n";
        cout << finalMsg;
        cin.get();
        return 1;
    }

    string inputText((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());

    LexicalAnalyzer lexicalAnalyzer(inputText);
    SemanticAnalyzer sematicAnalyzer = SemanticAnalyzer();
    SyntaxAnalyzer syntaxAnalyzer(lexicalAnalyzer, sematicAnalyzer);

    try {
        auto parseTree = syntaxAnalyzer.parse();
        const vector<string>& errorsSyn = syntaxAnalyzer.getErrorsSyntaxAn();
        const vector<string>& errorsSem = sematicAnalyzer.getErrorsSemanticAn();

        if (!errorsSyn.empty()) {
            for (const auto& error : errorsSyn) {
                cout << error << '\n';
            }
            cout << finalMsg;
            cin.get();
            return 1;
        }
        

        if (!errorsSem.empty()) {
            for (const auto& error : errorsSem) {
                cout << error << '\n';
            }
            cout << finalMsg;
            cin.get();
            return 1;
        }

        cout << "Лексический анализ\n";
        lexicalAnalyzer.tokenList.printTokensToConsole();
        lexicalAnalyzer.tokenList.printTokensToFiles();
        cout << "\nЛексический анализ успешно завершен. Hash - таблица сохранена в outputLexic.txt\n";

        ofstream outputFileSintax("outputSintax.txt");
        cout << "\nСинтаксический анализ\n";
        printTreeToConsole(parseTree);
        printTreeToFile(parseTree, 0, outputFileSintax);
        cout << "\nСинтаксический анализ успешно завершен. Дерево синтаксического анализа сохранено в outputSintax.txt\n";
        outputFileSintax.close();

        cout << "\nСемантический анализ\n";
        sematicAnalyzer.printTokensToConsoleSemantic();
        sematicAnalyzer.printTokensToFilesSemantic();
        cout << "\nСемантический анализ успешно завершен. Результат сохранен в outputSemantix.txt\n";

    }
    catch (const exception& ex) {
        ofstream ofs("outputSintaxSemanticError.txt");
        ofs.clear();
        cout << "Ошибка. " << ex.what() << '\n';
        cout << finalMsg;
        ofs << "Ошибка. " << ex.what();
        ofs.close();
        cin.get();
        return 1;
    }

    cout << finalMsg;
    cin.get();

    return 0;
}
