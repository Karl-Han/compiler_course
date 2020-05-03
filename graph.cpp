#include "graph.hpp"
#include <cstring>


void Vertex::insert_edge(char s, int dest)
{
    Edge *new_edge = new Edge(s, this->vertex_number, dest, this->edge_list);

    this->edge_list = new_edge;
}

// incomplete, it did not consider all the branches
Edge *Vertex::transit(char *ch, size_t *i)
{
    Edge *tmp = this->edge_list;
    Edge *epsilon = nullptr;

    // go through all the valid edge until match ch
    while (tmp != nullptr && ch[*i] != tmp->terminal)
    {
        // this is incomplete because there is a lot of ~
        if (tmp->terminal == '~')
        {
            epsilon = tmp;
        }

        tmp = tmp->edge_next;
    }

    if (tmp == nullptr && epsilon == 0)
    {
        printf("NO transit found for %d with %d", this->vertex_number, ch);
        exit(1);
    }
    if (tmp == nullptr)
    {
        return epsilon;
    }

    (*i)++;
    return tmp;
}

// transit from v with ch **directly**
int Vertex::transit_new(char ch){
    Edge* tmp = this->edge_list;

    while(tmp != nullptr){
        if (tmp->terminal == ch)
        {
            // get one
            return tmp->dest_num;
        }
        tmp = tmp->edge_next;
    }

    return 0;
}

// insert a new vertex with num
void Graph::insert_vertex(int num)
{
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
        last_vertex->vertex_next = new Vertex(num);
        last_vertex = last_vertex->vertex_next;
    }
}

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

void Graph::walk_str(int init_vertex, char *ch)
{
    int length = strlen(ch);
    int num_vertex = init_vertex;
    size_t i = 0;
    Vertex *v = nullptr;

    while (i < length)
    {
        v = this->get_vertex_by_number(num_vertex);
        Edge *new_vertex = v->transit(ch, &i);
        printf("%c: transit from %d to %d\n", new_vertex->terminal, new_vertex->src_num, new_vertex->dest_num);
        num_vertex = new_vertex->dest_num;
        printf("i = %d, length = %d\n", i, length);
        if (num_vertex == 0)
        {
            printf("NO such transit from %c in status %d\n", ch[i], v->vertex_number);
            v->print_vertex();
            exit(1);
        }
    }

    if (num_vertex != this->s_accept)
    {
        // all the symbol in ch is gone
        // walk until it is accept
        v = get_vertex_by_number(num_vertex);
        printf("NOT accepted yet and in %d\n", num_vertex);
        std::set<int> s = probe_epsilon(v);

        auto search = s.find(s_accept);

        if (search != s.end())
        {
            // find it
            printf("SUCCESS in finding %d in probeble set from %d\n", s_accept, v->vertex_number);
            return;
        }
        else
        {
            printf("Failed in finding %d in probeble set from %d\n", s_accept, v->vertex_number);
        }
        for (auto i = s.begin(); i != s.end(); i++)
        {
            printf("%d ", *i);
        }
        printf("\n ");
        exit(1);
    }
}

void Graph::set_s0_accept(Pair *p)
{
    this->s0 = p->src_num;
    this->s_accept = p->dest_num;
}

void Graph::set_s0_accept(int start, int final)
{
    this->s0 = start;
    this->s_accept = final;
}

void Vertex::print_vertex()
{
    Edge *tmp = this->edge_list;
    while (tmp != nullptr)
    {
        printf("\tFrom %d to %d by %c\n", tmp->src_num, tmp->dest_num, tmp->terminal);
        tmp = tmp->edge_next;
    }
}

void Graph::print_graph()
{
    Vertex *tmp = starter_vertex->vertex_next;
    while (tmp != nullptr)
    {
        tmp->print_vertex();
        tmp = tmp->vertex_next;
    }
}

// all reachable from v by epsilon
std::set<int> Graph::probe_epsilon(Vertex *v)
{
    Edge *tmp = v->edge_list;
    std::set<int> s;

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
        std::set<int> s_probe = probe_epsilon(get_vertex_by_number(*i));

        s.insert(s_probe.begin(), s_probe.end());
    }

    s.insert(v->vertex_number);
    return s;
}
