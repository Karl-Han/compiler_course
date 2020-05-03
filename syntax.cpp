#include "syntax.hpp"
#include <iostream>
#include <sstream>
#include <vector>
#include <cstring>
#include <cassert>
// return new edge number

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

Pair *GlobalState::braket_op(Graph *g)
{
    Vertex *v1 = new Vertex(this->global_status++);
    Vertex *v2 = new Vertex(this->global_status++);

    if (match_char('('))
    {
        match('(');
        Pair *p = or_match_op(g);
        match(')');

        return p;
    }

    char ch = '~';
    g->insert_vertex(v1);
    g->insert_vertex(v2);

    if (current_is_symbol())
    {
        ch = get_symbol();
        this->cursor++;
    }
    g->insert_edge(v1->vertex_number, ch, v2->vertex_number);

    return new Pair(v1->vertex_number, v2->vertex_number);
}

Pair *GlobalState::star_match_op(Graph *g)
{
    Vertex *v1 = new Vertex(this->global_status++);
    Vertex *v2 = new Vertex(this->global_status++);
    g->insert_vertex(v1);
    g->insert_vertex(v2);

    Pair *tmp_pair = braket_op(g);
    g->insert_edge(v1->vertex_number, '~', tmp_pair->src_num);
    g->insert_edge(tmp_pair->dest_num, '~', v2->vertex_number);

    if (match_char('*'))
    {
        match('*');
        g->insert_edge(tmp_pair->dest_num, '~', tmp_pair->src_num);
    }

    return new Pair(v1->vertex_number, v2->vertex_number);
}

Pair *GlobalState::and_match_op(Graph *g)
{
    Vertex *v1 = new Vertex(this->global_status++);
    // Vertex* v2 = new Vertex(this->global_status++);
    g->insert_vertex(v1);
    // g->insert_vertex(v2);

    Pair *B_or_op = star_match_op(g);
    g->insert_edge(v1->vertex_number, '~', B_or_op->src_num);
    // g->insert_edge(B_or_op->dest_num, '~', v2->vertex_number );
    Pair *B_or_op_other = B_or_op;

    while (match_char('-'))
    {
        match('-');
        B_or_op_other = star_match_op(g);
        g->insert_edge(B_or_op->dest_num, '-', B_or_op_other->src_num);
        // it seems like it does not matter
        // because it only use B_or_op's dest
        B_or_op = B_or_op_other;
        // g->insert_edge(B_or_op_other->dest_num, '~', v2->vertex_number );
    }

    return new Pair(v1->vertex_number, B_or_op_other->dest_num);
}

Pair *GlobalState::or_match_op(Graph *g)
{
    Vertex *v1 = new Vertex(this->global_status++);
    Vertex *v2 = new Vertex(this->global_status++);

    g->insert_vertex(v1);
    g->insert_vertex(v2);

    // both of B_or_op is already in the graph
    Pair *B_or_op = and_match_op(g);
    g->insert_edge(v1->vertex_number, '~', B_or_op->src_num);
    g->insert_edge(B_or_op->dest_num, '~', v2->vertex_number);

    while (match_char('|'))
    {
        match('|');
        Pair *B_or_op_other = and_match_op(g);
        g->insert_edge(v1->vertex_number, '~', B_or_op_other->src_num);
        g->insert_edge(B_or_op_other->dest_num, '~', v2->vertex_number);
    }

    return new Pair(v1->vertex_number, v2->vertex_number);
}

Pair *GlobalState::entry_match(Graph *g)
{
    return or_match_op(g);
}

bool GlobalState::match(char ch)
{
    if (ch == this->str[this->cursor])
    {
        this->cursor++;
        return true;
    }
    else
    {

        exit(1);
    }
}

bool GlobalState::match_char(char ch)
{
    return (ch == this->str[this->cursor]);
}

bool GlobalState::current_is_symbol()
{
    // for (size_t i = 0; i < this->sym_num; i++)
    // {
    // if (this->str[this->cursor] == this->sym_table[i])
    // {
    // return true;
    // }
    // }
    // return false;
    if (this->str[this->cursor] != '~')
    {
        return true;
    }
    return false;
}

char GlobalState::get_symbol()
{
    return this->str[this->cursor];
}

void GlobalState::get_sym_table(Graph *g)
{
    int length = strlen(str);

    for (size_t i = 0; i < length; i++)
    {
        if (str[i] == '|' || str[i] == '*' || str[i] == '(' || str[i] == ')')
        {
            // all these are special symbol
            continue;
        }

        sym_table.insert(str[i]);
    }
}

// std::set<int> reach_new_status(Graph* g, std::set<int>s, char ch){
//     std::set<int> s_new;
//
//     for (auto i = s.begin(); i != s.end(); i++)
//     {
//         int dst = g->get_vertex_by_number(*i)->transit_new(ch);
//         if (dst != 0)
//         {
//             std::set<int> s_probe = g->probe_epsilon(g->get_vertex_by_number(dst));
//             s_new.insert(s_probe.begin(), s_probe.end());
//         }
//
//     }
//
//     return s_new;
// }

void GlobalState::get_transition_table(Graph *g)
{
    std::set<int> s0 = g->probe_epsilon(g->get_vertex_by_number(g->s0));
    s.insert(s0);
    auto it = st.begin();
    st.insert(it, StatusRow(this->counter_st++, s0, g->s_accept));
    std::set<std::set<int>> s_diff(s);
    std::set<std::set<int>> s_probed;
    std::set<int> s_empty;
    s.insert(s_empty);
    s_probed.insert(s_empty);

    // int counter = 0;

    // finding new status set
    while (!s_diff.empty()) // && counter < 5)
    {
        // find any status set in those not been found
        std::set<int> tmp_set = *(s_diff.begin());

        // enumerate all symbol for status in a set
        for (auto i = sym_table.begin(); i != sym_table.end(); i++)
        {
            // std::set<int> s_new = reach_new_status(tmp_set, *i);
            // new status set for symbol i
            std::set<int> s_new;

            // find direct transit from status `j` with specific symbol `i` to find new status
            // only one or none will match
            for (auto j = tmp_set.begin(); j != tmp_set.end(); j++)
            {
                // printf("j = %d\n", *j);
                // direct transit
                int dst = g->get_vertex_by_number(*j)->transit_new(*i);
                if (dst != 0)
                {
                    // get all epsilon closure
                    std::set<int> s_probe = g->probe_epsilon(g->get_vertex_by_number(dst));
                    s_new.insert(s_probe.begin(), s_probe.end());

                    // find out if it is new status set
                }
            }

            // for the status tmp_set and the symbol i
            auto search = s.find(s_new);
            if (search == s.end())
            {
                // new set
                // new Status Row
                auto it = st.begin();
                // st.insert(it, StatusRow(this->counter_st++, s_new));
                st.push_back(StatusRow(this->counter_st++, s_new, g->s_accept));
            }

            StatusRow *sr = get_status_row_with_set(tmp_set);
            if (sr == nullptr)
            {
                printf("No status row found");
                exit(1);
            }
            sr->insert_map(*i, s_new);
            s.insert(s_new);
        }

        s_probed.insert(tmp_set);
        s_diff.clear();
        std::set_difference(s.begin(), s.end(), s_probed.begin(), s_probed.end(), std::inserter(s_diff, s_diff.begin()));
        for (auto i : s_diff)
        {
            std::cout << set_to_string(&i);
        }
        // counter++;
    }
}

// for status s, transit from s to tt with ch
void StatusRow::insert_map(char ch, std::set<int> tt)
{
    auto search = transit_table.find(ch);

    if (search == transit_table.end())
    {
        // no such element found
        transit_table[ch] = tt;
    }
    else
    {
        std::set<int> tmp = search->second;
        printf("Found for %c, tt = %s, search = %s", ch, set_to_string(&tt).c_str(), set_to_string(&tmp).c_str());
        assert(tmp == tt);
        printf("They are the same");
    }
}

StatusRow *GlobalState::get_status_row_with_set(std::set<int> s)
{
    for (auto i = st.begin(); i != st.end(); i++)
    {
        if (i->s == s)
        {
            return &(*i);
        }
    }

    printf("No such status row, s = %s\n", set_to_string(&s).c_str());
    exit(1);
    // return nullptr;
}

bool GlobalState::modify_sets(std::set<int> *s, int num)
{
    bool changed = false;
    std::set<int> *s_belong = new2set[num];
    // useless
    // std::set<int> s_inc;
    for (auto s_num : *s_belong)
    {
        std::set<int> *s_tmp = get_set_by_set_number(old2new[s_num]);
        int i = *(s_tmp->begin());
        auto j = s_tmp->begin();
        j++;
        for (; j != s_tmp->end(); j++)
        {
            if (!belong_same_set(i, *j))
            {
                changed = true;
                bool found = false;
                for (auto s_num_other : *s_belong)
                {
                    if (belong_same_set(*j, s_num_other))
                    {
                        found = true;
                        std::set<int> *s_tmp_other = get_set_by_set_number(s_num_other);
                        s_tmp_other->insert(*j);
                        new2set[status_minDFA] = s_tmp_other;
                        std::pair<int, std::set<int> > p(status_minDFA, *s_tmp_other);
                        set_new_raw.insert(p);
                        break;
                    }
                }
                if (!found)
                {
                    // make a new set
                    std::set<int> *s_new = new std::set<int>();
                    s_new->insert(*j);
                    new2set[status_minDFA] = s_new;
                    std::pair<int, std::set<int> > p(status_minDFA, *s_new);
                    set_new_raw.insert(p);
                }
                s_belong->insert(status_minDFA++);
                s_tmp->erase(*j);
            }
        }
    }
    return changed;
}

void GlobalState::minimizeDFA()
{
    bool change_ac, change_na;
    change_ac = change_na = true;
    // std::set<std::set<int> > s;             // all new status
    // std::set<std::set<int> > s_belong_ac;   // status sets belong to accept
    // std::set<std::set<int> > s_belong_na;   // status sets belong to not accept
    std::set<int> s;

    split_set_by_final(); // &s_belong_ac, &s_belong_na);
    s.insert(1);
    s.insert(2);

    while (change_ac | change_na)
    {
        while (change_ac)
        {
            change_ac = false;
            change_ac = modify_sets(&s, 1);
        }

        while (change_na)
        {
            change_na = false;
            change_na = modify_sets(&s, 2);
        }
    }

    new_set = s;
}

// split the original DFA set into ac and na
void GlobalState::split_set_by_final()
{ //std::set<int>* s_belong_ac,std::set<int>* s_belong_na){
    std::set<int> *s_belong_ac = new std::set<int>();
    std::set<int> *s_belong_na = new std::set<int>();

    for (auto sr : st)
    {
        // for each status row
        if (sr.is_accept)
        {
            // s_belong_ac's new status = 1
            s_belong_ac->insert(sr.number);
            old2new[sr.number] = 1;
        }
        else
        {
            // s_belong_ac's new status = 2
            s_belong_na->insert(sr.number);
            old2new[sr.number] = 2;
        }
    }
    // not yet used, start from 3
    new2set[1] = s_belong_ac;
    new2set[2] = s_belong_na;
    this->status_minDFA = 3;
}
// new status number to the set of elements(old status)
std::set<int> *GlobalState::get_set_by_set_number(int num)
{
    auto search = new2set.find(num);
    if (search == new2set.end())
    {
        // not such set found
        printf("NO such new set found: %d", num);
        exit(1);
    }
    return new2set[num];
}

// this function is used to find every transit of i and j
//      if they are identical
bool GlobalState::belong_same_set(int i, int j)
{
    for (auto sym : sym_table)
    {
        if (!transit_sym_ij_same(sym, i, j))
        {
            // some of transition do not in the same set
            return false;
        }
    }
    return true;
}

bool GlobalState::transit_sym_ij_same(char ch, int i, int j)
{
    int i_ch = transit_num2num(ch, i);
    int j_ch = transit_num2num(ch, j);

    return (old2new[i_ch] == old2new[j_ch]);
}

int GlobalState::transit_num2num(char ch, int i)
{
    StatusRow *sr = num2sr[i];
    return sr->transit_table_num[ch];
}

void GlobalState::setup()
{
    setup_set2int();
    setup_char2int();
    setup_num2sr();
}

void GlobalState::setup_set2int()
{
    for (auto sr : st)
    {
        set2old[sr.s] = sr.number;
    }
}

void GlobalState::setup_char2int()
{
    for (auto sr : st)
    {
        for (auto sym : sym_table)
        {
            sr.transit_table_num[sym] = set2old[sr.transit_table[sym]];
        }
    }
}

void GlobalState::setup_num2sr()
{
    for(auto sr = st.begin(); sr != st.end(); sr++)
    {
        num2sr[(*sr).number] = &(*sr);
    }
}

void GlobalState::get_minDFA_SR(){
    for(auto p = set_new_raw.begin(); p != set_new_raw.end(); p++){
        int num_status_old = *(p->second.begin());
        StatusRow* sr_old = num2sr[num_status_old];
        StatusRow tmp_sr(p->first, p->second, sr_old->is_accept);

        get_transit_table_num(&tmp_sr, this);

        st_minDFA.push_back(tmp_sr);
    }
}
void get_transit_table_num(StatusRow* sr , GlobalState* gs){
    for (auto sym : gs->sym_table)
    {
        sr->transit_table_num[sym] = gs->set2old[sr->transit_table[sym]];
    }
}