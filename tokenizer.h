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
#include <fstream>

struct Token {
  std::string type;
  std::string value;
  int val_i;
  double val_d;
  float val_f;
  long val_l;

  Token() {
    // Initialize token
    type = "";
    value = "";
  }
};

class Tokenizer {
private:
    bool buffer_ready;
    int current_state;
    std::string token_buffer;
    std::vector<Token> tokens;

    void init_state(char new_ch);
    void handle_char(char new_ch);
    void create_token(std::string type);
    std::string get_word_type(std::string value);
    std::string get_op_type(char op);

public:
    Tokenizer();
    std::vector<Token> getTokens(std::ifstream & input_file);
};

#endif /* tokenizer_h */
