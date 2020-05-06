#ifndef __syntax
#define __syntax

#include "graph.hpp"

#include <map>
#include <set>
#include <algorithm>
#include <vector>

// std::set<int> reach_new_status(std::set<int>s, char ch);
class GlobalState;

class Table{
    std::map<int, std::map<char, int> > oldS2newS;
    std::map<int, std::set<int> > S2set;
    std::map<int, bool> SisAC;

public:
    Table();
    void init();                // init table
    void parse_from_graph(Graph *g);
};

class StatusRow{
public:
    int number;                 // used for next
    std::set<int> s;            // From status set
    std::map<char, std::set<int> > transit_table;   // each row has its transition row
    std::map<char, int> transit_table_num;   // each row has its transition row with new number
    bool is_accept;

    StatusRow(int num, std::set<int> s, int accept): number(num), s(s){
        auto search = s.find(accept);
        if (search == s.end())
        {
            is_accept = 0;
        }
        else{
            is_accept = 1;
        }
    };
    void insert_map(char ch, std::set<int> tt);
};

class ParseReg{
public:
    int global_status;
    char* str;
    int cursor;
    Pair* s0_sAC;
    std::set<char> sym_table;
    Graph *g;

    ParseReg(std::string s, Graph* g): str(const_cast<char*>(s.c_str())), g(g){};

    bool match(char ch);
    bool match_char(char ch);
    bool is_symbol(char ch);
    bool current_is_symbol();
    char get_symbol();
    void get_sym_table();

    Pair* braket_op();
    Pair* star_match_op();
    Pair* and_match_op();
    Pair* or_match_op();
    Pair* entry_match();
};

struct FA_info{
public:
    std::set<char> sym_table;
    Pair s0_sAC;
    Table t;
};

class NFA{
public:
    FA_info info;
    NFA(ParseReg* pr);
};

class DFA {
public:
    FA_info info;
    DFA(NFA nfa);
};

class GlobalState {
public:
    int global_status;
    char* str;
    int cursor;
    std::set<char> sym_table;
    int sym_num;
    Pair* s_start_end;

    std::set<std::set<int> > s;     // all status sets
    std::vector<StatusRow> st;      // all transition for all sets
    int counter_st;                 // the number of status sets
    std::map<std::set<int>, int> set2old;     // old set to old status set number
    std::map<int, StatusRow*> num2sr;     // old status number to status row

    int status_minDFA;
    std::map<int, int> old2new;     // old status number to new status set number
    std::map<int, std::set<int>* > new2set; // new status set number to set
    std::set<int> new_set;
    std::set<std::pair<int, std::set<int>* > > set_new_raw; // number of set of old status to new set
    std::vector<StatusRow> st_minDFA;

    GlobalState(std::string ch): global_status(1), str(const_cast<char *>(ch.c_str())), cursor(0), counter_st(1), status_minDFA(1) { st = std::vector<StatusRow>();};
    void get_transition_table(Graph* g);
    StatusRow* get_status_row_with_set(std::set<int> s);

    void setup();
    void setup_set2int();
    void setup_char2int();
    void setup_num2sr();

    void minimizeDFA();
    void get_minDFA_SR();
    void split_set_by_final(); // std::set<int>* s_belong_ac,std::set<int>* s_belong_na);
    std::set<int>* get_set_by_set_number(int num);
    bool modify_sets(std::set<int> *s, int num, std::set<int>* s_belong); // std::set<int> *s_belong);
    bool belong_same_set(int i, int j);
    bool transit_sym_ij_same(char ch, int i, int j);
    int transit_num2num(char ch, int i);
};

std::string set_to_string(std::set<int>* s);
void get_transit_table_num(StatusRow* sr , GlobalState* gs, StatusRow* sr_old);

#endif