#include "graph.hpp"
#include <cstring>

void Graph::insert_vertex(Vertex *v)
{
    int num = v->vertex_number;
    Vertex *tmp = starter_vertex;

    while (tmp->vertex_number != num && tmp != last_vertex)
    {
        tmp = tmp->vertex_next;
    }

    // reach or not exist

    if (tmp->vertex_number == num)
    {
        // std::cout << "ERROR, exist that vertex" << std::endl;
        printf("ERROR, exist that vertex %d", num);
        exit(1);
    }
    else
    {
        last_vertex->vertex_next = v;
        last_vertex = last_vertex->vertex_next;
    }
}

// ASSUME it must exist this vertex
// insert a new edge from num to dest with ch
void Graph::insert_edge(int num, char ch, int dest)
{
    Vertex *tmp = starter_vertex;

    while (tmp->vertex_number != num && tmp != last_vertex)
    {
        tmp = tmp->vertex_next;
    }
    tmp->insert_edge(ch, dest);
}

Vertex *Graph::get_vertex_by_number(int num)
{
    Vertex *tmp = this->starter_vertex;

    while (tmp != nullptr)
    {
        if (tmp->vertex_number == num)
        {
            return tmp;
        }
        tmp = tmp->vertex_next;
    }

    printf("ERROR, no such Vertex %d", num);
    exit(1);
}

void Graph::set_s0_accept(Pair *p)
{
    this->s0 = p->src_num;
    this->s_accept = p->dest_num;
}

void Graph::print_graph()
{
    Vertex *tmp = starter_vertex->vertex_next;
    while (tmp != nullptr)
    {
        tmp->print_all_edge();
        tmp = tmp->vertex_next;
    }
}

// all reachable from v by epsilon
std::set<int> Graph::probe_epsilon(Vertex *v, std::set<int>* s_probed)
{
    auto search = s_probed->find(v->vertex_number);
    if (search != s_probed->end())
    {
        return std::set<int>();
    }
    
    Edge *tmp = v->edge_list;
    std::set<int> s;

    s.insert(v->vertex_number);
    s_probed->insert(v->vertex_number);

    // enumerate all with one epsilon
    while (tmp != nullptr)
    {
        if (tmp->terminal == '~')
        {
            s.insert(tmp->dest_num);
        }
        tmp = tmp->edge_next;
    }

    for (auto i = s.begin(); i != s.end(); i++)
    {
        std::set<int> s_probe = probe_epsilon(get_vertex_by_number(*i), s_probed);

        s.insert(s_probe.begin(), s_probe.end());
        s_probed->insert(s_probe.begin(), s_probe.end());
    }

    return s;
}
