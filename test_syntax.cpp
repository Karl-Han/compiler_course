#include "syntax.hpp"
#include "graph.hpp"
#include "lib/fort.hpp"
#include <vector>
#include <fstream>

// `s` for all status sets, `m` for one status_set transition table
void print_transition_table(std::set<char> sym_table,
                            std::vector<StatusRow> st)
{
    fort::char_table table;

    table << fort::header
          << "New number"
          << "Origin status"
          << "Accept?";
    std::vector<char> v(sym_table.begin(), sym_table.end());
    std::sort(v.begin(), v.end());
    for (auto i = v.begin(); i != v.end(); i++)
    {
        table << (*i);
    }
    table << fort::endr;

    // single row means one status's transition table for all symbols
    for (StatusRow status_row : st)
    {
        table << status_row.number << set_to_string(&status_row.s) << status_row.is_accept;
        for (auto i = v.begin(); i != v.end(); i++)
        {
            auto search = status_row.transit_table.find(*i);
            if (search != status_row.transit_table.end())
            {
                // exist transition
                table << set_to_string(&search->second);
            }
        }
        table << fort::endr;
    }

    std::cout << table.to_string() << std::endl;
}

std::string generate_code(GlobalState *gs)
{
    std::string content;
    std::string header = "#include <cstring>\n#include <iostream>\nint main(){\n";
    std::string footer = "return 0;\n}";
    int s_accept[256];
    int counter = 0;
    memset(s_accept, 0, sizeof(int));

    std::ostringstream content_stringstream;

    content_stringstream << "int status = " << gs->old2new[1] << ";\n";
    content_stringstream << "char buf[256];\nstd::cin>>buf;\nint cursor = 0;\n";
    content_stringstream << "\twhile(buf[cursor] != \'\\0\'){\n";

    content_stringstream << "switch(status){\n";
    for (auto sr : gs->st_minDFA)
    {
        std::ostringstream sstream_sr;
        if (sr.is_accept)
        {
            s_accept[counter] = sr.number;
            counter++;
        }
        
        sstream_sr << "\tcase " << sr.number << ":{\n";

        sstream_sr << "\tswitch(buf[cursor]){\n";

        for (auto sym : gs->sym_table)
        {
            std::ostringstream sstream_sym;
            sstream_sym << "\t\tcase "
                        << "'" << sym << "':{\n";
            int dest = *(sr.transit_table[sym].begin());
            if (dest == 0)
            {
                sstream_sym << "printf(\"No such transition from %d by %c\","
                            << sr.number << ", buf[cursor]);\nexit(1);\n";
            }
            else
            {
                sstream_sym << "cursor++;\nstatus = " << dest << ";\n";
            }

            // symbol transit switch end
            sstream_sym << "break;\n}\n";
            sstream_sr << sstream_sym.str();
        }

        sstream_sr<<"default:\nprintf(\"No such symbol %c\", buf[cursor]);\nexit(1);";

        // status row switch end
        sstream_sr << "break;\n}\n";
        // case end
        sstream_sr << "break;\n}\n";

        content_stringstream << sstream_sr.str();
    }

    // switch
    content_stringstream << "break;\n}\n";
    // while
    content_stringstream << "}\n";

    if (counter == 0)
    {
        content_stringstream << "printf(\"No accept status.\");";
    }

    else
    {
        content_stringstream << "if(status != "<<s_accept[0];
        for (size_t i = 1; i < counter; i++)
        {
            content_stringstream<<"&& status != "<<s_accept[i];
        }
        content_stringstream << ")";
        content_stringstream << "\tprintf(\"Failed to accept, NOT match\");\n";
        content_stringstream << "else\n\tprintf(\"Program match %s\", buf);\n";
    }

    content = header + content_stringstream.str() + footer;

    return content;
}

int main()
{
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
    // char *str = "a-b|c";
    // char* str = "a*";
    // char* str = "a-b*|c-d";
    char* str = "a-(b|c)-d*";
    GlobalState *gs = new GlobalState(str);
    Graph *g = new Graph();

    // Pair* p = entry_match(g, gs);
    Pair *p = gs->entry_match(g);
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
    std::cout << std::endl;
    print_transition_table(gs->sym_table, gs->st);

    gs->setup();

    gs->minimizeDFA();
    gs->get_minDFA_SR();

    print_transition_table(gs->sym_table, gs->st_minDFA);

    // std::cout<<generate_code(gs)<<std::endl;
    std::fstream f("gen_code.cpp", std::ios::trunc | std::ios::out);
    f << generate_code(gs);
    f.close();
    return 0;
}