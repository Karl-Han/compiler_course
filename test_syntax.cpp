#include "syntax.hpp"
#include "graph.hpp"
#include "lib/fort.hpp"
#include <vector>


// `s` for all status sets, `m` for one status_set transition table
void print_transition_table(std::set<char> sym_table, 
        std::vector<StatusRow> st){
    fort::char_table table;

    table<< fort::header
        << "New number" << "Origin status"<<"Accept?";
    std::vector<char> v(sym_table.begin(), sym_table.end());
    std::sort(v.begin(), v.end());
    for (auto i = v.begin(); i != v.end(); i++)
    {
        table<<(*i);
    }
    table<<fort::endr;

    // single row means one status's transition table for all symbols
    for(StatusRow status_row : st){
        table<<status_row.number<<set_to_string(&status_row.s)<<status_row.is_accept;
        for (auto i = v.begin(); i != v.end(); i++)
        {
            auto search = status_row.transit_table.find(*i);
            if (search != status_row.transit_table.end())
            {
                // exist transition
                table<<set_to_string(&search->second);
            }
            
        }
        table<<fort::endr;
    }

    std::cout<<table.to_string()<<std::endl;

}

int main(){
// test construction of NFA
    // char* str = "";
    // char* str = "a";
    // char* str = "a-b";
    // char* str = "a-b|c";
    // char* str = "a-b*|c-d";
    // char* str = "a-(b|c)-d*";

    // GlobalState* gs = new GlobalState(str);
    // Graph* g = new Graph();

    // // Pair* p = entry_match(g, gs);
    // Pair* p = entry_match(g, gs);
    // p->print();
    // g->set_s0_accept(p);
    // g->print_graph();
    // g->walk_str(p->src_num, "abbbbbbb");

    // printf("SUCCESS");

// test print set
    // std::set<int>* s = new std::set<int>();
    // int arr[5] = {5, 4, 2, 3, 1};
    // s->insert(std::begin(arr), std::end(arr));
    // std::cout<<set_to_string(s);
    
    // char* str = "";
    // char* str = "a";
    // char* str = "a-b";
    char* str = "a-b|c";
    // char* str = "a*";
    // char* str = "a-b*|c-d";
    // char* str = "a-(b|c)-d*";
    GlobalState* gs = new GlobalState(str);
    Graph* g = new Graph();

    // Pair* p = entry_match(g, gs);
    Pair* p = gs->entry_match(g);
    printf("----p->print-----\n");
    p->print();
    printf("----g->set_s0_accept-----\n");
    g->set_s0_accept(p);
    printf("----g->print_graph-----\n");
    g->print_graph();
    // printf("----g->walk_str-----\n");
    // g->walk_str(p->src_num, "a-b");

    gs->get_sym_table(g);
    gs->get_transition_table(g);
    std::cout<<std::endl;
    // print_transition_table(gs->sym_table, gs->st);

    gs->setup();

    gs->minimizeDFA();
    gs->get_minDFA_SR();
    
    print_transition_table(gs->sym_table, gs->st_minDFA);

    return 0;
}