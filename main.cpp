#include <iostream>

#include <boost/graph/graphviz.hpp>

#include "graph.h"
#include "pushrelabel.h"


Graph test_graph() {
    size_t num_vertices = 11; 
    Graph g(num_vertices);

    add_edge(9, 0, g);
    for (size_t i = 0; i < 9; i++)
        add_edge(i, i+1, g);

    add_edge(0, 10, g);

    return g;
}


int main() {
    std::cout << "Hello" << std::endl;

    auto g = test_graph();

    auto vi = vertices(g);
    for (auto &v = vi.first; v != vi.second; v++)
        std::cout << *v << std::endl;


    auto zero_neighbors = adjacent_vertices(0, g);

    for (auto &vp = zero_neighbors.first; vp!=zero_neighbors.second; vp++) {
        auto vtx = *vp;
        std::cout << vtx << " ";
    }


    std::ofstream dotfile("mygraph.dot");
    dynamic_properties dp;
    dp.property("potential", get(&PRPotential::potential, g));
    dp.property("capacity", get(&EdgeProperties::capacity, g));
    dp.property("flow", get(&EdgeProperties::flow, g));
    dp.property("node_id", get(boost::vertex_index, g));

    write_graphviz_dp(dotfile, g, dp);

    return 0;
}

