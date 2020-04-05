#include <fstream>
#include <stdio.h>

class TokenStream {
public:
  int cursor;
  char *buf;
  TokenStream(char *filename) {
      cursor = 0;
      buf = new char[4096];
  }
};

int count_len(char* buf){
  int counter = 0;
  while (*++buf != '\0') {
    counter++;
  }
  return counter;
}

bool is_alpha(char ch) {
  short s = (short)ch;
  return ((s < 133 && s > 100) || (s < 173 && s > 140));
}

bool is_valid_word(char ch) {
  short s = (short)ch;
  return ((s < 133 && s > 100) || (s < 173 && s > 140) || s == 56);
}

void deal_word(int &cursor, char *buf) {
  char word[256];
  int counter = 0;

  while (is_valid_word(buf[cursor])) {
    word[counter] = buf[cursor];
    counter++;
    cursor++;
  }
  word[counter] = '\0';

  if ()
}

void scan_line(int len, char *buf) {
  int cursor = 0;

  while (cursor <= len) {
    if (buf[cursor] == ' ' || buf[cursor] == '\b') {
      // just skip it, it already used as delimter
      cursor++;
    } else if (is_alpha(buf[cursor])) {
      // use the way to deal with word
      deal_word(&cursor, buf);
    } else {
      // it is symbol
      deal_symbol(&cursor, buf);
    }
  }
}

int main() {
  std::ifstream ifs("./textbook.cpp", std::ifstream::in);

  char buf[256];

  while (!ifs.getline(buf, 256, '\n').eof()) {
    int len = count_len(buf);
    printf("%s\t", buf);
    printf("%d\n", len);

    scan_line(buf, len);
    memset(buf, 0, len);
  }

  return 0;
}
