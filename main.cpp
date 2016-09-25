//
//  main.cpp
//  Tokenator
//
//  Created by Katie Schaffer on 9/4/16.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "tokenizer.h"

using namespace std;

int main(int argc, const char * argv[]) {

    // Default filename for debugging
    string filename = "input_old.txt";

    // Catch argument errors
    if (argc != 2) {
        cout << "Usage: " << endl;
        cout << "Tokenator [filename]\n\n" << endl;
        return 0;
    }

    // Get filename from arguments
    filename = argv[1];
    cout << "FILENAME: " << filename << "\n" << endl;
    ifstream inputFile;
    inputFile.open(filename.c_str());     // Input file

    // Create tokenizer object and analyze file
    Tokenizer tokenizer;
    tokenizer.analyze(inputFile);

    return 0;
}
