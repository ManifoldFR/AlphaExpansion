#include <iostream>

#include <boost/graph/graphviz.hpp>

#include "graph.h"
#include "pushrelabel.h"


Graph test_pr() {
    /// Graph build : cf Cormen book page 727
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
    auto es = edges(g);
    for (auto &it = es.first; it != es.second; it++){
        auto v(source(*it, g)), w(target(*it, g));
        if (!edge(w, v, g).second)
            add_edge(w, v, EdgeProperties{0, 0}, g);
    }

    // symmetrize_graph(g);

    return g;
}


int main() {
    auto g = test_pr();

    std::cout << "Finished building graph" << std::endl; 

    compute_min_cut(g, 0, 5);

    auto vs = vertices(g);
    for (auto &it = vs.first; it != vs.second; it++) {
        std::cout << "Vertex " << *it << " has class " << g[*it].cut_class << std::endl;
    }

    auto edges = boost::edges(g);
    for (auto &it = edges.first; it != edges.second; it++) {
        std::cout << source(*it, g) << " to " << target(*it, g) << " : " << g[*it].flow << " / " << g[*it].capacity << std::endl;
    }

    return 0;
}

