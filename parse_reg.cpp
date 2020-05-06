#include "syntax.hpp"

Pair *ParseReg::braket_op()
{
    Vertex *v1 = new Vertex(this->global_status++);
    Vertex *v2 = new Vertex(this->global_status++);

    if (match_char('('))
    {
        match('(');
        Pair *p = or_match_op();
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

Pair *ParseReg::star_match_op()
{
    Vertex *v1 = new Vertex(this->global_status++);
    Vertex *v2 = new Vertex(this->global_status++);
    g->insert_vertex(v1);
    g->insert_vertex(v2);

    Pair *tmp_pair = braket_op();
    g->insert_edge(v1->vertex_number, '~', tmp_pair->src_num);
    g->insert_edge(tmp_pair->dest_num, '~', v2->vertex_number);

    if (match_char('*'))
    {
        match('*');
        g->insert_edge(tmp_pair->dest_num, '~', tmp_pair->src_num);
        g->insert_edge(tmp_pair->src_num, '~', tmp_pair->dest_num);
    }

    return new Pair(v1->vertex_number, v2->vertex_number);
}

Pair *ParseReg::and_match_op()
{
    Vertex *v1 = new Vertex(this->global_status++);
    // Vertex* v2 = new Vertex(this->global_status++);
    g->insert_vertex(v1);
    // g->insert_vertex(v2);

    Pair *B_or_op = star_match_op();
    g->insert_edge(v1->vertex_number, '~', B_or_op->src_num);
    // g->insert_edge(B_or_op->dest_num, '~', v2->vertex_number );
    Pair *B_or_op_other = B_or_op;

    while (match_char('-'))
    {
        match('-');
        B_or_op_other = star_match_op();
        g->insert_edge(B_or_op->dest_num, '~', B_or_op_other->src_num);
        // it seems like it does not matter
        // because it only use B_or_op's dest
        B_or_op = B_or_op_other;
        // g->insert_edge(B_or_op_other->dest_num, '~', v2->vertex_number );
    }

    return new Pair(v1->vertex_number, B_or_op_other->dest_num);
}

Pair *ParseReg::or_match_op()
{
    Vertex *v1 = new Vertex(this->global_status++);
    Vertex *v2 = new Vertex(this->global_status++);

    g->insert_vertex(v1);
    g->insert_vertex(v2);

    // both of B_or_op is already in the graph
    Pair *B_or_op = and_match_op();
    g->insert_edge(v1->vertex_number, '~', B_or_op->src_num);
    g->insert_edge(B_or_op->dest_num, '~', v2->vertex_number);

    while (match_char('|'))
    {
        match('|');
        Pair *B_or_op_other = and_match_op();
        g->insert_edge(v1->vertex_number, '~', B_or_op_other->src_num);
        g->insert_edge(B_or_op_other->dest_num, '~', v2->vertex_number);
    }

    return new Pair(v1->vertex_number, v2->vertex_number);
}

Pair *ParseReg::entry_match()
{
    s0_sAC = or_match_op();
    return s0_sAC;
}

bool ParseReg::match(char ch)
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

bool ParseReg::match_char(char ch)
{
    return (ch == this->str[this->cursor]);
}

bool ParseReg::is_symbol(char ch){
    if(ch == '-' || ch == '|' || ch == '*' || ch == '(' || ch == ')')
    {
        return false;
    }
    return true;
}

bool ParseReg::current_is_symbol()
{
    return is_symbol(str[cursor]);
}

char ParseReg::get_symbol()
{
    return str[cursor];
}

void ParseReg::get_sym_table()
{
    int length = strlen(str);

    for (size_t i = 0; i < length; i++)
    {
        if (!is_symbol(str[i]))
        {
            // all these are special symbol
            continue;
        }

        sym_table.insert(str[i]);
    }
}