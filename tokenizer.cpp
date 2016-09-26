//
//  Tokenizer.cpp
//  Tokenator
//
//  Created by Katie Schaffer on 9/4/16.
//

#include "tokenizer.h"

Tokenizer::Tokenizer() {
    // Initialize tokenizer
    current_state = 0;      // whitespace state
    buffer_ready = true;
}

std::vector<Token> Tokenizer::getTokens(std::ifstream & input) {
    // Reads input file, processing character by character.
    // Returns vector of tokens found.

    char c;

    // Read input file while processing characters
    while (input.get(c)) {
        // Analyze character
        handle_char(c);

        // Start processing next token
        if (buffer_ready) {
            init_state(c);
        }
    } // Finished processing intput file

    // Return vector of tokens
    return tokens;
}

void Tokenizer::init_state(char new_ch) {
    // Flushes token buffer, starting a new one
    token_buffer = "";

    // Initialize tokenizing state based on current character
    if ((isspace(new_ch) || new_ch == '\0') && (new_ch != '\r') && (new_ch != '\n')) {
        // Whitespace
        current_state = 0;
        return;
    } else if (isalpha(new_ch)) {
        // Alphabetical
        current_state = 1;
    } else if (isnumber(new_ch)) {
        // Numerical
        current_state = 2;
    } else {
        // Operators and punctuation
        switch (new_ch) {
        case '+':
        case '-':
        case '/':
        case '*':
        case '&':
        case '|':
        case ';':
        case ',':
        case '(':
        case ')':
        case '{':
        case '}':
            // Unambiguous single character
            current_state = 10;
            break;

        case '"':
            // String start
            current_state = 3;
            break;

        case '#':
            // Comment start
            current_state = 4;
            break;

        case '!':
            // Bang
            current_state = 5;
            break;

        case '=':
            // Equals
            current_state = 6;
            break;

        case '>':
            // GT
            current_state = 7;

        case '<':
            // LT
            current_state = 8;
            break;

        case '\r':
        case '\n':
            // Breaks - more whitespace
            current_state = 0;

        default:
            // UNKNOWN CHARACTER
            return; // ?
        }
    }

    // Begin new token;
    token_buffer+=new_ch;
    buffer_ready = false;
    return;
}

void Tokenizer::handle_char(char new_ch) {
    // Checks for token ending conditions and buffer tasks for each possible state.

    switch (current_state) {
    case 0: // ----- DEFAULT STATE
        // Buffer is now ready for character
        buffer_ready = true;
        return;

    case 1: // ----- WORD
        // Close word token on any non-alphanumeric character
        if (!isalpha(new_ch) && !isnumber(new_ch)) {
            // Determine token type and create token
            std::string type = get_word_type(token_buffer);
            create_token(type);
            // Open buffer to read incoming character
            buffer_ready = true;
        } else {
            // Add incoming character to token buffer
            token_buffer+=new_ch;
        }
        break;

    case 2: // ----- NUMBER
        // Close number token on anything non-numerical
        if (!isnumber(new_ch)) {
            if (isalpha(new_ch)) {
                // ERROR
                // !!
            } else {
                // Create NUMBER token
                create_token("NUMBER");
                // Open buffer to read incoming character
                buffer_ready = true;
            }
        } else {
            // Add incoming character to token buffer
            token_buffer+=new_ch;
        }
        break;

    case 3: // ----- STRING
        // Close string when a " is reached
        if (new_ch == '\"') {
            // Add last quotation mark to token
            token_buffer+=new_ch;
            // Keep buffer closed from handling incoming character
            current_state = 0;
            // Create STRING token
            create_token("STRING");
        } else {
            // Add incoming character to token buffer
            token_buffer+=new_ch;
        }
        break;

    case 4: // ----- COMMENT
        // Close comment on a newline
        if (new_ch == '\r' || new_ch == '\n') {
            // Ready to read tokens again
            buffer_ready = true;
        }
        // Otherwise, do nothing until comment ends
        break;

    case 5: // ----- ! EXC
        // Could be a NOT or a RELOP
        if (new_ch == '=') {
            // Add incoming character to token buffer
            token_buffer+=new_ch;
            // Keep buffer closed from handling incoming character
            current_state = 0;
            // Create a RELOP token
            create_token("RELOP");
        } else {
            // Create a NOT token
            create_token("NOT");
            // Open buffer to read incoming character
            buffer_ready = true;
        }
        break;

    case 6: // ----- = EQUALS
        // Check for second '='
        if (new_ch == '=') {
            // Add incoming character to token buffer
            token_buffer+=new_ch;
            // Keep buffer closed from handling incoming character
            current_state = 0;
            // Create RELOP token
            create_token("RELOP");
        } else {
            // Without the second '=', it's an error
            create_token("ERROR");
            // Open buffer to read incoming character
            buffer_ready = true;
        }
        break;

    case 7: // ----- > GT
        // Could be a > or a >=
        if (new_ch == '=') {
            // Add incoming character to token buffer
            token_buffer+=new_ch;
            // Keep buffer closed from handling incoming character
            current_state = 0;
            // Create RELOP token
            create_token("RELOP");
        } else {
            // Create '>' token
            create_token("RELOP");
            // Open buffer to read incoming character
            buffer_ready = true;
        }
        break;

    case 8: // ----- < LT
        // Could be a < or a <= or an ASSIGNOP
        if (new_ch == '-') {
            // Add incoming character to token buffer
            token_buffer+=new_ch;
            // Keep buffer closed from handling incoming character
            current_state = 0;
            // Create ASSIGNOP token
            create_token("ASSIGNOP");
        } else if (new_ch == '=') {
            // Add incoming character to token buffer
            token_buffer+=new_ch;
            // Keep buffer closed from handling incoming character
            current_state = 0;
            // Create RELOP token
            create_token("RELOP");
        } else {
            // Create '<' token
            create_token("RELOP");
            // Open buffer to read incoming character
            buffer_ready = true;
        }
        break;

    case 10: // ----- Unambiguous single character
        // Create token immediately
        std::string type = get_op_type(token_buffer[0]);
        create_token(type);
        // Open buffer to read incoming character
        buffer_ready = true;
        break;

    }
}

void Tokenizer::create_token(std::string type) {
    // Create a new token and save it to the token vector
    Token * new_token = new Token();
    // Set token type and value
    new_token->type = type;
    new_token->value = token_buffer;
    // Save token to token vector
    tokens.push_back(*new_token);
}

std::string Tokenizer::get_word_type(std::string value) {
    // Get token type for word types
    if (value == "function") {
        return "FUNCTION";
    } else if (value == "var") {
        return "VAR";
    } else if (value == "if") {
        return "IF";
    } else if (value == "else") {
        return "ELSE";
    } else if (value == "while") {
        return "WHILE";
    } else if (value == "return") {
        return "WHILE";
    } else {
        return "ID";
    }
}

std::string Tokenizer::get_op_type(char op) {
    // Get token type for single characters
    switch (op) {
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
