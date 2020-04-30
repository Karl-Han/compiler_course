#ifndef __graph
#define __graph

#include<iostream>
/*
 * Convention:
 *  Edge:
 *      "" means epsilon
 *      - means and
 *      * means closure
 * 
 */

class Edge {
public:
    char terminal;         // accept terminal and transit
    int src_num;            // destination vertex number
    int dest_num;           // destination vertex number
    Edge* edge_next;        // next transitable edge

    Edge(): terminal('\0'), src_num(0),dest_num(0), edge_next(nullptr){};
    Edge(char s, int src, int num, Edge* next): terminal(s),src_num(src), dest_num(num), edge_next(next){};
};

class Vertex {
public:
    int vertex_number;      // this vertex number
    Vertex* vertex_next;    // another vertex
    Edge* edge_list;        // this vertex transit edge list

    Vertex(): vertex_number(0), vertex_next(nullptr), edge_list(nullptr){};
    Vertex(int num): vertex_number(num), vertex_next(nullptr), edge_list(nullptr){};

    void insert_edge(char s, int dest);
    Edge* transit(char* ch, size_t* i);
    void print_vertex();
};

// Graph is initialized with an vertex 0
class Graph {
public:
    Vertex* starter_vertex;
    Vertex* last_vertex;

    Graph() {
        last_vertex = starter_vertex = new Vertex();
    }
    void insert_vertex(int num);
    void insert_vertex(Vertex* v);
    void insert_edge(int num, char ch, int dest);
    Vertex* get_vertex_by_number(int num);
    void walk_str(int init, char* ch);
    void print_graph();
};

#endif