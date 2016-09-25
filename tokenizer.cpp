//
//  Tokenizer.cpp
//  Tokenator
//
//  Created by Katie Schaffer on 9/4/16.
//

#include "tokenizer.h"

Tokenizer::Tokenizer() {
    // Initialize tokenizer
    this->previous_state = 1;
    this->current_state = 1;
    this->new_char = ' ';
    this->held_char = ' ';
}

void Tokenizer::analyze(std::ifstream & input) {
    char c;
    this->token = "";

    // Read input file while processing characters
    while (input.get(c)) {
        this->new_char = c;
        process_char(this->new_char);
    }
}

void Tokenizer::process_char(char c) {

    //
    // Determine state
    //
    int char_type = get_char_type(c);
    this->previous_state = current_state;
    this->current_state = transitions[this->current_state][char_type];

    //
    // Handle completed token
    //
    if (current_state == 1) {   // FINISHED

        // Set token type
        if (this->token_type == "WORD") {
            this->token_type = get_word_type(this->token);
        }
        else if (this->token_type == "SINGLE") {
            this->token_type = get_op_type(this->token[0]);
        }
        else if (this->token_type == "EQUALS") {
            this->token_type = "RELOP";
        }
        else if (this->token_type == "ERROR") {
            this->token_type = "ERROR";
        }

        // If it's a string, append final character
        if (previous_state == 5) {
            this->token = (this->token) + c;
        }

        // Print token
        if (this->token_type != "COMMENT") // (Don't print comments)
            print_token();

        // Update state
        if (previous_state != 5 ) { // (Don't do this for strings)
            this->previous_state = current_state;
            this->current_state = transitions[this->current_state][char_type];
        }
    }

    //
    // Handle incoming character
    //
    if (current_state == 0) {               // ERROR
        this->token_type = "ERROR";
        this->token = (this->token);

        // Print error token
        print_token();

        // Reset state twice
        this->previous_state = current_state;
        this->current_state = transitions[this->current_state][char_type];
        this->previous_state = current_state;
        this->current_state = transitions[this->current_state][char_type];
    }

    if (current_state == 3) {               // WORD
        this->token_type = "WORD";
        this->token = (this->token) + c;
    }
    else if (current_state == 4) {          // NUMBER
        this->token_type = "NUM";
        this->token = (this->token) + c;
    }
    else if (current_state == 5) {          // STRING
        this->token_type = "STRING";
        this->token = (this->token) + c;
    }
    else if (current_state == 6) {          // LT
        this->token = (this->token) + c;
    }
    else if (current_state == 7) {          // GT
        this->token = (this->token) + c;
    }
    else if (current_state == 8) {          // ASSIGNOP
        this->token_type = "ASSIGNOP";
        this->token = (this->token) + c;
    }
    else if (current_state == 9) {          // RELOP
        this->token_type = "RELOP";
        this->token = (this->token) + c;
    }
    else if (current_state == 13 ) {        // COMMENT
        this->token_type = "COMMENT";
        this->token = (this->token) + c;
    }
    else if (current_state == 12 && char_type != 1 && char_type != 22 ) {          // SINGLE CHAR
        this->token_type = "SINGLE";
        this->token = c;
    }
    else if (current_state == 11) {         // EQUALS
        this->token_type = "EQUALS";
        this->token = (this->token) + c;
    }
    else if (current_state == 2) {         // WS
        // Do nothing
    }
}

void Tokenizer::print_token() {
    // Print out token info and reset token

    std::cout << "TOKEN:" << std::left << std::setw(15) << this->token_type << "\t" << this->token << std::endl;
    this->token = "";
    this->token_type = "";
}

int Tokenizer::get_char_type(char c) {
    // Get type of char for use in transition matrix

    if ( (isspace(c) || c=='\0') && (c != '\r') && (c != '\n') ) {
        return 1;
    }
    else if (isalpha(c)) {
        return 2;
    }
    else if (isnumber(c)) {
        return 3;
    }
    else {
        switch (c) {
            case '+':
                // PLUS
                return 4;
            case '-':
                // MINUS
                return 5;
            case '/':
                // SLASH
                return 6;
            case '*':
                // STAR
                return 7;
            case '&':
                // AND
                return 8;
            case '#':
                // POUND
                return 9;
            case '!':
                // EXCL
                return 10;
            case '|':
                // PIPE
                return 11;
            case '=':
                // EQUALS
                return 12;
            case ';':
                // SEMICOLON
                return 13;
            case '"':
                // QUOTE
                return 14;
            case '(':
                // PARENL
                return 15;
            case ')':
                // PARENR
                return 16;
            case '<':
                // LT
                return 17;
            case '>' :
                // GT
                return 18;
            case ',':
                // COMMA
                return 19;
            case '{':
                // CURLL
                return 20;
            case '}':
                // CURLR
                return 21;
            case '\r':
            case '\n':
                // RETURN
                return 22;
            default:
                // UNKNOWN CHARACTER
                return 23;
                break;
        }
    }

    return 0;
}

std::string Tokenizer::get_word_type(std::string value) {
    // Get token type for word types

    if (value == "function") {
        return "FUNCTION";
    }
    else if (value == "var") {
        return "VAR";
    }
    else if (value == "if") {
        return "IF";
    }
    else if (value == "else") {
        return "ELSE";
    }
    else if (value == "while") {
        return "WHILE";
    }
    else if (value == "return") {
        return "WHILE";
    }
    else {
        return "ID";
    }
}

std::string Tokenizer::get_op_type(char op) {
    // Get token type for single characters

    switch (this->token[0]) {
        case '+':
        case '-':
            return "ADDOP";
            break;
        case '&':
            return "AND";
            break;
        case ',':
            return "COMMA";
            break;
        case '{':
            return "CURLL";
            break;
        case '}':
            return "CURLR";
            break;
        case '/':
        case '*':
            return "MULOP";
            break;
        case '!':
            return "NOT";
            break;
        case '|':
            return "OR";
            break;
        case '(':
            return "PARENL";
            break;
        case ')':
            return "PARENR";
            break;
        case '<':
        case '>':
            return "RELOP";
            break;
        case ';':
            return "SEMICOLON";
            break;
        default:
            return "ERROR";
            break;
    }
}
