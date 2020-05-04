#ifndef __syntax
#define __syntax

#include "graph.hpp"

#include <map>
#include <set>
#include <algorithm>
#include <vector>

// std::set<int> reach_new_status(std::set<int>s, char ch);
class GlobalState;

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

    GlobalState(char* ch): global_status(1), str(ch), cursor(0), counter_st(1), status_minDFA(1) { st = std::vector<StatusRow>();};
    void get_symbol_table();
    bool match(char ch);
    bool match_char(char ch);
    bool current_is_symbol();
    char get_symbol();
    Pair* braket_op(Graph* g);
    Pair* star_match_op(Graph* g);
    Pair* and_match_op(Graph* g);
    Pair* or_match_op(Graph* g);
    Pair* entry_match(Graph* g);

    void get_sym_table(Graph* g);
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