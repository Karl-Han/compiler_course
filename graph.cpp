#include "graph.hpp"

void Vertex::insert_edge(char s, int dest)
{
    Edge *new_edge = new Edge(s, this->vertex_number, dest, this->edge_list);

    this->edge_list = new_edge;
}

Edge* Vertex::transit(char* ch, size_t* i)
{
    Edge *tmp = this->edge_list;
    Edge* epsilon = nullptr;
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
        printf("NO transit found for %d with %c", this->vertex_number, ch);
        exit(1);
    }
    if (tmp == nullptr)
    {
        return epsilon;
    }
    
    (*i)++;
    return tmp;
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

void Graph::insert_vertex(Vertex* v)
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

Vertex* Graph::get_vertex_by_number(int num){
    Vertex* tmp = this->starter_vertex;

    while (tmp != nullptr)
    {
        if(tmp->vertex_number == num){
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

    while(i < length)
    {
        Vertex *v = this->get_vertex_by_number(num_vertex);
        Edge* new_vertex = v->transit(ch, &i);
        printf("%c: transit from %d to %d\n", new_vertex->terminal, new_vertex->src_num, new_vertex->dest_num);
        num_vertex = new_vertex->dest_num;
        if (num_vertex == 0)
        {
            printf("NO such transit from %c in status %d\n", ch[i], v->vertex_number);
            v->print_vertex();
            exit(1);
        }
    }
}

void Vertex::print_vertex(){
    Edge* tmp = this->edge_list;
    while (tmp != nullptr)
    {
        printf("\tFrom %d to %d by %c\n", tmp->src_num, tmp->dest_num, tmp->terminal);
        tmp = tmp->edge_next;
    }
}

void Graph::print_graph(){
    Vertex* tmp = starter_vertex->vertex_next;
    while (tmp != nullptr)
    {
        tmp->print_vertex();
        tmp = tmp->vertex_next;
    }
}