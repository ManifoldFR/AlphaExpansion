#include <iostream>

#include <boost/graph/graphviz.hpp>

#include "graph.h"
#include "pushrelabel.h"


Graph test_pr() {
    /// Graph build : cf Cormen book page 727
    // Max flow should be 23
    
    size_t num_vertices = 6; 
    Graph g(num_vertices);

    add_edge(0, 1, EdgeProperties{16, 0}, g);
    add_edge(0, 2, EdgeProperties{13, 0}, g);
    add_edge(1, 3, EdgeProperties{12, 0}, g);
    add_edge(2, 1, EdgeProperties{4, 0}, g);
    add_edge(2, 4, EdgeProperties{14, 0}, g);
    add_edge(3, 2, EdgeProperties{9, 0}, g);
    add_edge(3, 5, EdgeProperties{20, 0}, g);
    add_edge(4, 3, EdgeProperties{7, 0}, g);
    add_edge(4, 5, EdgeProperties{4, 0}, g);

    // caution : need to symmetrize the graph
    symmetrize_graph(g);
    split_edges(g);

    return g;
}

Graph test_pr2() {
    /// https://www.cs.purdue.edu/homes/ayg/CS251/slides/chap10c.pdf
    // Max flow should be 5
    
    size_t num_vertices = 7; 
    Graph g(num_vertices);

    add_edge(0, 1, EdgeProperties{2, 0}, g);
    add_edge(0, 2, EdgeProperties{3, 0}, g);
    add_edge(0, 3, EdgeProperties{1, 0}, g);
    add_edge(1, 3, EdgeProperties{2, 0}, g);
    add_edge(1, 4, EdgeProperties{1, 0}, g);
    add_edge(2, 3, EdgeProperties{2, 0}, g);
    add_edge(2, 5, EdgeProperties{2, 0}, g);
    add_edge(3, 4, EdgeProperties{2, 0}, g);
    add_edge(3, 5, EdgeProperties{4, 0}, g);
    add_edge(3, 6, EdgeProperties{2, 0}, g);
    add_edge(4, 6, EdgeProperties{2, 0}, g);
    add_edge(5, 6, EdgeProperties{1, 0}, g);
    
    // caution : need to symmetrize the graph
    symmetrize_graph(g);
    split_edges(g);

    return g;
}

Graph test_pr3() {
    /// https://developers.google.com/optimization/flow/maxflow
    
    size_t num_vertices = 5; 
    Graph g(num_vertices);

    add_edge(0, 1, EdgeProperties{20, 0}, g);
    add_edge(0, 2, EdgeProperties{30, 0}, g);
    add_edge(0, 3, EdgeProperties{10, 0}, g);
    add_edge(1, 2, EdgeProperties{40, 0}, g);
    add_edge(1, 4, EdgeProperties{30, 0}, g);
    add_edge(2, 3, EdgeProperties{10, 0}, g);
    add_edge(2, 4, EdgeProperties{20, 0}, g);
    add_edge(3, 2, EdgeProperties{5, 0}, g);
    add_edge(3, 4, EdgeProperties{20, 0}, g);

    // caution : need to symmetrize the graph
    symmetrize_graph(g);
    split_edges(g);

    return g;
}

int main() {
    
    // EXAMPLE 1

    std::cout << "Beginning example 1" << std::endl;

    auto g = test_pr();
    auto min_cut = compute_min_cut(g, 0, 5);

    auto edges = boost::edges(g);
    for (auto &it = edges.first; it != edges.second; it++) {
        std::cout << source(*it, g) << " " << target(*it, g) << " " << g[*it].flow << "/" << g[*it].capacity << std::endl;
    }

    std::cout << "Value of min cut : " << min_cut << std::endl;

    std::cout << "Press key to continue ";
    std::cin.get();

    // EXAMPLE 2

    std::cout << "Beginning example 2" << std::endl;

    auto g2 = test_pr2();
    auto min_cut2 = compute_min_cut(g2, 0, 6);

    auto edges2 = boost::edges(g2);
    for (auto &it = edges2.first; it != edges2.second; it++) {
        std::cout << source(*it, g2) << " " << target(*it, g2) << " " << g2[*it].flow << "/" << g2[*it].capacity << std::endl;
    }

    std::cout << "Value of min cut : " << min_cut2 << std::endl;

    std::cout << "Press key to continue ";
    std::cin.get();

    // EXAMPLE 3

    std::cout << "Beginning example 2" << std::endl;

    auto g3 = test_pr3();
    auto min_cut3 = compute_min_cut(g3, 0, 4);

    auto edges3 = boost::edges(g3);
    for (auto &it = edges3.first; it != edges3.second; it++) {
        std::cout << source(*it, g3) << " " << target(*it, g3) << " " << g3[*it].flow << "/" << g3[*it].capacity << std::endl;
    }

    std::cout << "Value of min cut : " << min_cut3 << std::endl;

    return 0;
}

