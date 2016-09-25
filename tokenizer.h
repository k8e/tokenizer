//
//  Tokenizer.h
//  Tokenator
//
//  Created by Katie Schaffer on 9/4/16.
//

#ifndef tokenizer_h
#define tokenizer_h

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>

class Tokenizer {
private:
    int previous_state;
    int current_state;

    std::string token;
    std::string token_type;

    char new_char;
    char held_char;

    int get_char_type(char c);
    void process_char(char c);
    void print_token();
    std::string get_word_type(std::string value);
    std::string get_op_type(char op);

    // STATE TRANSITION MATRIX
    const int transitions[14][24] =
    {
        // CHAR  //     0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23
        ///////////        ws  let num  +   -   /   *   &   #   !   |   =   ;   "   (   )   <   >   ,   {   }  \r  etc
        // STATE //                                                                                                      // [MODE]
        /* 0  */     {  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1 }, // ERROR
        /* 1  */     {  0,  2,  3,  4, 12, 12, 12, 12, 12, 13, 10, 12, 11, 12,  5, 12, 12,  6,  7, 12, 12, 12,  1,  0 }, // FINISHED
        /* 2  */     {  0,  2,  3,  4, 12, 12, 12, 12, 12, 13, 10, 12, 11, 12,  1, 12, 12,  6,  7, 12, 12, 12,  1,  0 }, // WS
        /* 3  */     {  0,  1,  3,  3,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0 }, // WORD
        /* 4  */     {  0,  1,  0,  4,  1,  1,  1,  1,  1,  1,  1,  1, 11,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0 }, // NUM
        /* 5  */     {  0,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  1,  5,  5,  5,  5,  5,  5,  5,  5,  5 }, // STRING
        /* 6  */     {  0,  1,  1,  1,  1,  8,  1,  1,  1,  1,  1,  1,  9,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0 }, // LT
        /* 7  */     {  0,  1,  1,  1,  1,  7,  1,  1,  1,  1,  1,  1,  9,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0 }, // GT
        /* 8  */     {  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0 }, // ASSIGNOP
        /* 9  */     {  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0 }, // RELOP
        /* 10 */     {  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  9,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0 }, // EXCL
        /* 11 */     {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 }, // EQUALS
        /* 12 */     {  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0 }, // SINGLECHAR
        /* 13 */     { 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,  1, 13 }  // COMMENT
    };

public:
    Tokenizer();
    void analyze(std::ifstream & input_file);
};

#endif /* tokenizer_h */
