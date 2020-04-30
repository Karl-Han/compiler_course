#include"syntax.hpp"

int main(){
    // char* str = "";
    char* str = "a";
    // char* str = "a-b";
    // char* str = "a-b|c";
    // char* str = "a-b|c-d*";
    // char* str = "a-(b|c)-d*";

    GlobalState* gs = new GlobalState(str);
    Graph* g = new Graph();

    // Pair* p = entry_match(g, gs);
    Pair* p = entry_match(g, gs);
    g->print_graph();
    g->walk_str(p->src_num, str);

    printf("SUCCESS");

    return 0;
}