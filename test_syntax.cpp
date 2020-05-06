#include "syntax.hpp"
#include "graph.hpp"
#include "lib/fort.hpp"
#include <vector>
#include <fstream>

std::string set_to_string(std::set<int> *s)
{
    std::stringstream istream;
    std::vector<int> v(s->begin(), s->end());
    std::sort(v.begin(), v.end());

    istream << "{";

    for (auto i = v.begin(); i != v.end(); i++)
    {
        istream << *i << ",";
    }

    istream << "}";

    return istream.str();
}


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
    std::string header = "// use ~ as epsilon\n#include <cstring>\n#include <iostream>\nint main(){\n";
    std::string footer = "\treturn 0;\n}";
    int s_accept[256];
    int counter = 0;
    memset(s_accept, 0, sizeof(int));

    std::ostringstream content_stringstream;

    content_stringstream << "int status = " << gs->old2new[1] << ";\n";
    content_stringstream << "printf(\"Use ~ as empty string\\nEnter the string to identify: \");\n";
    content_stringstream << "char buf[256];\nstd::cin>>buf;\nint cursor = 0;\n";
    content_stringstream << "\twhile(buf[cursor] != \'\\0\'){\n";

    content_stringstream << "\t\tswitch(status){\n";
    for (auto sr : gs->st_minDFA)
    {
        std::ostringstream sstream_sr;
        if (sr.is_accept)
        {
            s_accept[counter] = sr.number;
            counter++;
        }
        
        sstream_sr << "\t\t\tcase " << sr.number << ":{\n";

        sstream_sr << "\t\t\t\tswitch(buf[cursor]){\n";

        for (auto sym : gs->sym_table)
        {
            std::ostringstream sstream_sym;
            sstream_sym << "\t\t\t\t\tcase "
                        << "'" << sym << "':{\n";
            int dest = *(sr.transit_table[sym].begin());
            if (dest == 0)
            {
                sstream_sym << "\t\t\t\t\t\tprintf(\"No such transition from %d by %c\","
                            << sr.number << ", buf[cursor]);\n\t\t\t\t\t\texit(1);\n";
            }
            else
            {
                sstream_sym << "\t\t\t\t\t\tcursor++;\n\t\t\t\t\tstatus = " << dest << ";\n";
            }

            // symbol transit switch end
            sstream_sym << "\t\t\t\t\tbreak;}\n";
            sstream_sr << sstream_sym.str();
        }
        sstream_sr<<"\t\t\t\t\tcase '~':{\n\t\t\t\t\t\tbuf[cursor] = '\\0';\n";
        sstream_sr<< "\t\t\t\t\tbreak;}\n";

        sstream_sr<<"\t\t\t\t\tdefault:\n\t\t\t\t\t\tprintf(\"No such symbol %c\", buf[cursor]);\n\t\t\t\t\t\texit(1);\n";

        // status row switch end
        sstream_sr << "\t\t\t\t\tbreak;}\n";
        // case end
        sstream_sr << "\t\t\tbreak;}\n";

        content_stringstream << sstream_sr.str();
    }

    // switch
    content_stringstream << "\t\tbreak;}\n";
    // while
    content_stringstream << "\t\t}\n";

    if (counter == 0)
    {
        content_stringstream << "\tprintf(\"No accept status.\");";
    }

    else
    {
        content_stringstream << "\tif(status != "<<s_accept[0];
        for (size_t i = 1; i < counter; i++)
        {
            content_stringstream<<"&& status != "<<s_accept[i];
        }
        content_stringstream << ")\n";
        content_stringstream << "\t\tprintf(\"Failed to accept, NOT match\");\n";
        content_stringstream << "\telse\n\t\tprintf(\"Program match %s\", buf);\n";
    }

    content = header + content_stringstream.str() + footer;

    return content;
}

std::string parse_args(int argc, char* argv[]){
    std::string str;
    if (argc == 1)
    {
        // str = "";
        // str = "a";
        // str = "a-b";
        // str = "a-b|c";
        // str = "a*";
        str = "a-b*|c-d";
        // str = "a-(b|c)-d*";
    }
    else if (argc == 2)
    {
        if (strcmp(argv[1], "help") == 0)
        {
            printf("main [OP] [REG]\nDefault REG is a-b|c.\nSupport - for concatenation, | for or operation, * for closure operation.\nGenerated code is in gen_code.cpp\n");
            printf("Examples:\n\tmain \"a-b|c\"\n\tmain file reg.txt");
            exit(0);
        }
        
        str = argv[1];
        printf("%s\n", str.c_str());
    }
    else{
        if (argc == 3)
        {
            if (strcmp(argv[1], "file") == 0)
            {
                std::ifstream f(argv[2]);
                if (!f.is_open())
                {
                    printf("ERROR occur when opening file %s", argv[2]);
                    exit(1);
                }
                
                std::string s;
                std::getline(f, s);
                return s;
            }
        }
        else{
            printf("main [OP] [REG]\nDefault REG is a-b|c.\nSupport - for concatenation, | for or operation, * for closure operation.\nGenerated code is in gen_code.cpp\n");
            printf("Examples:\n\tmain \"a-b|c\"\n\tmain file reg.txt\t----read regex from reg.txt");
            exit(0);
        }
    }
}

int main(int argc, char* argv[])
{
    
    std::string str = parse_args(argc, argv);
    
    // initialize
    GlobalState *gs = new GlobalState(str);
    Graph *g = new Graph();

    // get NFA
    Pair *p = gs->entry_match(g);
    g->set_s0_accept(p);
    gs->get_sym_table(g);

    // transform NFA to DFA
    gs->get_transition_table(g);
    std::cout << std::endl;
    print_transition_table(gs->sym_table, gs->st);

    gs->setup();

    // transform DFA to minDFA
    gs->minimizeDFA();
    gs->get_minDFA_SR();

    print_transition_table(gs->sym_table, gs->st_minDFA);

    // generate runnable identification code
    std::fstream f("gen_code.cpp", std::ios::trunc | std::ios::out);
    f << generate_code(gs);
    f.close();

    return 0;
}