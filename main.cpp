//
//  main.cpp
//  Tokenator
//
//  Created by Katie Schaffer on 9/4/16.
//

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>

#include "tokenizer.h"

using namespace std;

int main(int argc, const char * argv[]) {
    string filename;

    // Catch argument errors
    if (argc != 2) {
        cout << "Usage: " << endl;
        cout << "Tokenator [filename]\n\n" << endl;
        return 0;
    }

    // Get filename from arguments
    filename = argv[1];
    ifstream inputFile;
    inputFile.open(filename.c_str());

    // Create tokenizer object and token list
    Tokenizer tokenizer;
    vector<Token> tokenList;

    // Use tokenizer to get list of tokens from input file
    tokenList = tokenizer.getTokens(inputFile);

    // Iterate through token list, printing out token info
    std::vector<Token>::iterator token;
    for (token = tokenList.begin(); token != tokenList.end(); ++token) {
        std::cout << "TOKEN:" << std::left << std::setw(15) << token->type << "\t" << token->value << std::endl;
    }

    return 0;
}
