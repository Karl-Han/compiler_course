#include "graph.hpp"

void Vertex::insert_edge(char s, int dest)
{
    Edge *new_edge = new Edge(s, this->vertex_number, dest, this->edge_list);

    this->edge_list = new_edge;
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

void Vertex::print_all_edge()
{
    Edge *tmp = this->edge_list;
    while (tmp != nullptr)
    {
        printf("\tFrom %d to %d by %c\n", tmp->src_num, tmp->dest_num, tmp->terminal);
        tmp = tmp->edge_next;
    }
}
