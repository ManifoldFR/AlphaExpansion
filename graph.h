#pragma once

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

using namespace boost;


struct PRPotential {
    /// Excess flow at the node.
    int excess_flow = 0;
    /// Label d for the PR algorithm.
    int labeling;
};

/// Edge property, with both flow value and edge capacity.
struct EdgeProperties {
    int capacity = 0;
    int flow = 0;
};

int get_residual(EdgeProperties e) {
    return e.capacity - e.flow;
}


typedef adjacency_list<vecS, vecS, undirectedS,
                       PRPotential, EdgeProperties> Graph;


typedef property_map<Graph, PRPotential> vertex_prop_map;
typedef property_map<Graph, EdgeProperties> edge_prop_map;

