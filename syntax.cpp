#include "syntax.hpp"

bool match(char ch, GlobalState *gs)
{
    while (ch != '~')
    {
        if (ch == gs->str[gs->cursor])
        {
            return true;
        }
        else
        {
            printf("ch = %c and cursor is %c", ch, gs->str[gs->cursor]);
            exit(1);
        }
    }
    return true;
}

bool is_char(char ch, GlobalState *gs)
{
    return (ch == gs->str[gs->cursor]);
}

bool is_symbol(GlobalState *gs)
{
    // for (size_t i = 0; i < gs->sym_num; i++)
    // {
        // if (gs->str[gs->cursor] == gs->sym_table[i])
        // {
            // return true;
        // }
    // }
    // return false;
    if (gs->str[gs->cursor] != '~')
    {
        return true;
    }
    return false;
}

char get_symbol(GlobalState *gs)
{
    return gs->str[gs->cursor];
}

// return new edge number

Pair *braket_op(Graph *g, GlobalState *gs)
{
    Vertex *v1 = new Vertex(gs->global_status++);
    Vertex *v2 = new Vertex(gs->global_status++);

    if (is_char('(', gs))
    {
        match('(', gs);
        Pair *p = or_match_op(g, gs);
        match(')', gs);

        return p;
    }

    char ch = '~';
    g->insert_vertex(v1);
    g->insert_vertex(v2);

    if (is_symbol(gs))
    {
        ch = get_symbol(gs);
    }
    g->insert_edge(v1->vertex_number, ch, v2->vertex_number);

    return new Pair(v1->vertex_number, v2->vertex_number);
}

Pair *star_match_op(Graph *g, GlobalState *gs)
{
    Vertex *v1 = new Vertex(gs->global_status++);
    Vertex *v2 = new Vertex(gs->global_status++);
    g->insert_vertex(v1);
    g->insert_vertex(v2);

    Pair *tmp_pair = braket_op(g, gs);
    g->insert_edge(v1->vertex_number, '~', tmp_pair->src_num);
    g->insert_edge(tmp_pair->dest_num, '~', v2->vertex_number);

    if (is_char('*', gs))
    {
        match('*', gs);
        g->insert_edge(tmp_pair->dest_num, '~', tmp_pair->src_num);
    }

    return new Pair(v1->vertex_number, v2->vertex_number);
}

Pair *and_match_op(Graph *g, GlobalState *gs)
{
    Vertex *v1 = new Vertex(gs->global_status++);
    // Vertex* v2 = new Vertex(gs->global_status++);
    g->insert_vertex(v1);
    // g->insert_vertex(v2);

    Pair *B_or_op = star_match_op(g, gs);
    g->insert_edge(v1->vertex_number, '~', B_or_op->src_num);
    // g->insert_edge(B_or_op->dest_num, '~', v2->vertex_number );
    Pair *B_or_op_other = B_or_op;

    while (is_char('-', gs))
    {
        match('-', gs);
        B_or_op_other = star_match_op(g, gs);
        g->insert_edge(B_or_op->dest_num, '~', B_or_op_other->src_num);
        B_or_op = B_or_op_other;
        // g->insert_edge(B_or_op_other->dest_num, '~', v2->vertex_number );
    }

    return new Pair(v1->vertex_number, B_or_op_other->dest_num);
}

Pair *or_match_op(Graph *g, GlobalState *gs)
{
    Vertex *v1 = new Vertex(gs->global_status++);
    Vertex *v2 = new Vertex(gs->global_status++);

    g->insert_vertex(v1);
    g->insert_vertex(v2);

    // both of B_or_op is already in the graph
    Pair *B_or_op = and_match_op(g, gs);
    g->insert_edge(v1->vertex_number, '~', B_or_op->src_num);
    g->insert_edge(B_or_op->dest_num, '~', v2->vertex_number);

    while (is_char('|', gs))
    {
        match('|', gs);
        Pair *B_or_op_other = and_match_op(g, gs);
        g->insert_edge(v1->vertex_number, '~', B_or_op_other->src_num);
        g->insert_edge(B_or_op_other->dest_num, '~', v2->vertex_number);
    }

    return new Pair(v1->vertex_number, v2->vertex_number);
}

Pair *entry_match(Graph *g, GlobalState *gs)
{
    return or_match_op(g, gs);
}