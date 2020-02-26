#include <iostream>
#include <utility>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

#include "graph.h"
#include "AlphaExpansion.h"

using namespace boost;

void test_graph() {
    size_t num_vertices = 10; 
    Graph g(10);

    Graph::vertex_iterator vi;

    for (vi=vertices(g).first; vi!=vertices(g).second; vi++) {
        auto vtx = *vi;
        printf("Vertex %ld\n", vtx);
    }

    std::cout << "Built graph" << std::endl;
}

