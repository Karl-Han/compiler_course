#include <fstream>
#include <stdio.h>

enum Type {
  IDENTIFIER,
  KEYWORD,
  NUMBER,
  STRING,
  COMMENT,
  SPECIAL_SYMBOL,
  OPERATOR,
  NOTHING,
  EOF_Type
};

class Token {
public:
  Type t;
  char *buf;

  Token() {
    t = Type::NOTHING;
    buf = NULL;
  }

  Token(Type type, char *p, size_t len) {
    t = type;
    buf = new char[256];

    memcpy(buf, p, len);
  }

  static Token *get_eof() { return new Token(Type::EOF_Type, NULL, 0); }

  static Token *word2keyword(char *buf, size_t len) {
    switch (len) {
    case 2: {
      if (memcmp(buf, "if", 2) == 0 && buf[2] == 0) {
        return new Token(Type::KEYWORD, buf, len);
      } else {
        return NULL;
      }
      break;
    }
    case 3: {
      if (memcmp(buf, "int", 3) == 0 && buf[3] == 0) {
        return new Token(Type::KEYWORD, buf, len);
      }
      if (memcmp(buf, "cin", 3) == 0 && buf[3] == 0) {
        return new Token(Type::KEYWORD, buf, len);
      } else {
        return NULL;
      }
      break;
    }
    case 4: {
      if (memcmp(buf, "main", 4) == 0 && buf[4] == 0) {
        return new Token(Type::KEYWORD, buf, len);
      }
      if (memcmp(buf, "char", 4) == 0 && buf[4] == 0) {
        return new Token(Type::KEYWORD, buf, len);
      }
      if (memcmp(buf, "long", 4) == 0 && buf[4] == 0) {
        return new Token(Type::KEYWORD, buf, len);
      }
      if (memcmp(buf, "cout", 4) == 0 && buf[4] == 0) {
        return new Token(Type::KEYWORD, buf, len);
      }
      if (memcmp(buf, "else", 4) == 0 && buf[4] == 0) {
        return new Token(Type::KEYWORD, buf, len);
      } else {
        return NULL;
      }
      break;
    }
    case 7: {
      if (memcmp(buf, "include", 7) == 0 && buf[7] == 0) {
        return new Token(Type::KEYWORD, buf, len);
      } else {
        return NULL;
      }
      break;
    }
    default: {
      return NULL;
    }
    }
  }

  bool is_eof() { return t == Type::EOF_Type; }
  void print() {
    printf("%s\t", buf);
    switch (t) {
      // IDENTIFIER,
    case Type::IDENTIFIER: {
      printf("IDENTIFIER\n");
      break;
    }
      // KEYWORD,
    case Type::KEYWORD: {
      printf("KEYWORD\n");
      break;
    }
      // NUMBER,
    case Type::NUMBER: {
      printf("NUMBER\n");
      break;
    }
      // STRING,
    case Type::STRING: {
      printf("STRING\n");
      break;
    }
      // COMMENT,
    case Type::COMMENT: {
      printf("COMMENT\n");
      break;
    }
      // SPECIAL_SYMBOL,
    case Type::SPECIAL_SYMBOL: {
      printf("SPECIAL_SYMBOL\n");
      break;
    }
      // OPERATOR,
    case Type::OPERATOR: {
      printf("OPERATOR\n");
      break;
    }
      // NOTHING,
    case Type::NOTHING: {
      printf("NOTHING\n");
      break;
    }
      // EOF_Type
    case Type::EOF_Type: {
      printf("EOF\n");
      break;
    }
    default: {
      printf("ERROR occur");
      exit(1);
    }
    }
  }
};

bool is_alpha(char ch) {
  short s = (short)ch;
  bool valid = false;
  
  // capital
  valid ^= (s > 64 && s < 91);
  // character 
  valid ^= (s > 96 && s < 123);
  
  return valid;
}

bool is_number(char ch) {
  short s = (short)ch;
  bool valid = false;
  
  // number
  valid ^= (s > 47 && s < 58);
  
  return valid;
}

bool is_valid_word(char ch) {
  short s = (short)ch;
  bool valid = false;
  // number
  valid ^= (s > 47 && s < 58);
  // capital
  valid ^= (s > 64 && s < 91);
  // character 
  valid ^= (s > 96 && s < 123);
  // dot
  valid ^= (s == 46);

  return valid;
}

bool is_blank(char ch){
  return (ch == ' ' || ch == '\t' || ch == 10);
}

bool is_digit_component(char ch){
  bool valid = false;
  // number
  valid ^= (ch > 47 && ch < 58);
  // dot
  valid ^= (ch == 46);

  return valid;
}

class TokenStream {
public:
  int cursor;
  char *buf;

  TokenStream(char *filename) {

    cursor = 0;
    buf = new char[4096];
    std::ifstream ifs("./textbook.cpp", std::ifstream::in);

    ifs.read(buf, 4096);
    if (!ifs.eof()) {
      printf("File too big.");
      exit(1);
    }
  }

  Token *deal_word() {
    char word[256];
    int counter = 0;

    while (is_valid_word(buf[cursor])) {
      word[counter] = buf[cursor];
      counter++;
      cursor++;
    }
    word[counter] = '\0';

    Token *token = Token::word2keyword(word, counter);

    if (token == NULL) {
      // it is not a keyword
      return new Token(Type::IDENTIFIER, word, counter);
    }

    return token;
  }

  Token* deal_number(){
    char word[256];
    int counter = 1;
    int status = 2;

    word[0] = buf[cursor];
    cursor++;

    while(status != 5){
      if(!is_digit_component(buf[cursor])){
        status = 5;
      }
      else{
        if(status == 2){
          if(buf[cursor] == '.'){
            status = 3;
            word[counter] = buf[cursor];
            counter++;
            cursor++;
          }
          else{
            word[counter] = buf[cursor];
            counter++;
            cursor++;
          }
        }
        else if(status == 3){
          if(buf[cursor] == '.'){
            printf("ERROR, invalid float number format");
            exit(1);
          }
          else{
            status = 4;
            word[counter] = buf[cursor];
            counter++;
            cursor++;
          }
        }
        else if(status == 4){
          if(is_number(buf[cursor])){
            word[counter] = buf[cursor];
            counter++;
            cursor++;
          }
          else{
            status = 5;
          }
        }
      }
    }
    word[counter] = '\0';

    return new Token(Type::NUMBER, word, counter);
  }

  int read_comment(char *t_buf) {
    int len = 0;
    int status = 3;
    while (status != 5) {
      if (status == 4) {
        if (buf[cursor] == '/') {
          cursor++;
          t_buf[len] = '\0';
          status = 5;
        } else {
          t_buf[len] = '*';
          if (buf[cursor] != '*') {
            status = 3;
          }

          len++;
          cursor++;
        }
      } else if (buf[cursor] == '*') {
        if (status == 3) {
          status = 4;
          cursor++;
        }
      } else {
        t_buf[len] = buf[cursor];
        len++;
        cursor++;
      }
    }
    return len;
  }

  Token *deal_symbol() {
    switch (buf[cursor]) {
    case '+': {
      char *tmp = new char[3];
      int t_cur = 1;

      tmp[0] = buf[cursor];
      cursor++;

      switch (buf[cursor]) {
      case '=': {
        tmp[t_cur] = buf[cursor];
        t_cur++;
        cursor++;
        break;
      }
      case '+': {
        tmp[t_cur] = buf[cursor];
        t_cur++;
        cursor++;
        break;
      }
      }

      tmp[t_cur] = '\0';

      return new Token(Type::SPECIAL_SYMBOL, tmp, t_cur + 1);
    }
    case '-': {
      char *tmp = new char[3];
      int t_cur = 1;

      tmp[0] = buf[cursor];
      cursor++;

      switch (buf[cursor]) {
      case '=': {
        tmp[t_cur] = buf[cursor];
        t_cur++;
        cursor++;
        break;
      }
      case '-': {
        tmp[t_cur] = buf[cursor];
        t_cur++;
        cursor++;
        break;
      }
      }

      tmp[t_cur] = '\0';

      return new Token(Type::SPECIAL_SYMBOL, tmp, t_cur + 1);
    }
    case '*': {
      char *tmp = new char[3];
      int t_cur = 1;

      tmp[0] = buf[cursor];
      cursor++;

      switch (buf[cursor]) {
      case '=': {
        tmp[t_cur] = buf[cursor];
        t_cur++;
        cursor++;
        break;
      }
      case '+': {
        tmp[t_cur] = buf[cursor];
        t_cur++;
        cursor++;
        break;
      }
      }

      tmp[t_cur] = '\0';

      return new Token(Type::SPECIAL_SYMBOL, tmp, t_cur + 1);
    }
    case '/': {
      char *tmp = new char[3];
      int t_cur = 1;

      tmp[0] = buf[cursor];
      cursor++;

      switch (buf[cursor]) {
      case '=': {
        tmp[t_cur] = buf[cursor];
        t_cur++;
        cursor++;
        tmp[t_cur] = '\0';
        break;
      }
      case '*': {
        cursor++;
        delete[] tmp;
        tmp = new char[256];
        memset(tmp, 0, sizeof(char) * 256);
        read_comment(tmp);
        return new Token(Type::COMMENT, tmp, t_cur + 1);
        break;
      }
      }

      return new Token(Type::SPECIAL_SYMBOL, tmp, t_cur + 1);
    }
    case '<': {
      char *tmp = new char[3];
      int t_cur = 1;

      tmp[0] = buf[cursor];
      cursor++;

      switch (buf[cursor]) {
      case '=': {
        tmp[t_cur] = buf[cursor];
        t_cur++;
        cursor++;
        break;
      }
      case '<': {
        tmp[t_cur] = buf[cursor];
        t_cur++;
        cursor++;
        break;
      }
      }

      tmp[t_cur] = '\0';

      return new Token(Type::SPECIAL_SYMBOL, tmp, t_cur + 1);
    }
    case '>': {
      char *tmp = new char[3];
      int t_cur = 1;

      tmp[0] = buf[cursor];
      cursor++;

      switch (buf[cursor]) {
      case '=': {
        tmp[t_cur] = buf[cursor];
        t_cur++;
        cursor++;
        break;
      }
      case '>': {
        tmp[t_cur] = buf[cursor];
        t_cur++;
        cursor++;
        break;
      }
      }

      tmp[t_cur] = '\0';

      return new Token(Type::SPECIAL_SYMBOL, tmp, t_cur + 1);
    }
    case '"': {
      char *tmp = new char[256];
      size_t t_cur = 0;
      cursor++;

      while (buf[cursor] != '"') {
        tmp[t_cur] = buf[cursor];
        t_cur++;
        cursor++;
      }
      cursor++;

      return new Token(Type::STRING, tmp, t_cur + 1);
    }
      // single symbol
    case '#':
    case ';':
    case '(':
    case ')':
    case '{':
    case '}':
    case '[':
    case ']': {
      char tmp[2];
      tmp[0] = buf[cursor];
      cursor++;
      tmp[1] = '\0';
      return new Token(Type::SPECIAL_SYMBOL, tmp, 1);
      break;
    }

    default: {
      printf("Unknown SYMBOL: %d\n", (short)buf[cursor]);
      exit(1);
    }
    }
  }

  Token *get_token() {
    while(is_blank(buf[cursor])) {
      // just skip it, it already used as delimter
      cursor++;
    } 
    if (buf[cursor] == 0) {
      return Token::get_eof();
    } else if (is_alpha(buf[cursor])) {
      // use the way to deal with word
      return deal_word();
    } else if(is_number(buf[cursor])){
      return deal_number();
    }
    else {
      // it is symbol
      return deal_symbol();
    }

    exit(1);
  }
};

int main(int argc, char** argv) {
  if(argc != 2){
    printf("Please use the format: ./a.out <filename>");
    exit(1);
  }
  TokenStream tokenStream(argv[1]);
  Token *token;
  while (!(token = tokenStream.get_token())->is_eof()) {
    token->print();
  }
  return 0;
}
