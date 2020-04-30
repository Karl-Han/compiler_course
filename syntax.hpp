#ifndef __syntax
#define __syntax

#include"graph.hpp"

class GlobalState {
public:
	int global_status;
	char* str;
	int cursor;
    char* sym_table;
    int sym_num;

	GlobalState(char* s): global_status(1), str(s), cursor(0), sym_table(nullptr), sym_num(0){};
};

class Pair{
public:
	int src_num;
	int dest_num;

	Pair(int src, int dest): src_num(src), dest_num(dest){};
};

bool match(char ch, GlobalState* gs);
bool is_char(char ch, GlobalState* gs);
bool is_symbol(GlobalState* gs);
char get_symbol(GlobalState* gs);
Pair* braket_op(Graph* g, GlobalState* gs);
Pair* star_match_op(Graph* g, GlobalState* gs);
Pair* and_match_op(Graph* g, GlobalState* gs);
Pair* or_match_op(Graph* g, GlobalState* gs);
Pair* entry_match(Graph* g, GlobalState* gs);

#endif