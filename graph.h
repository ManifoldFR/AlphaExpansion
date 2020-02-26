#pragma once

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

using namespace boost;


struct PRPotential {
    int potential;
    int excess_flow = 0;
};

/// Edge property, with both flow value and edge capacity.
struct EdgeProperties {
    int capacity = 0;
    int flow = 0;
};

typedef adjacency_list<vecS, vecS, undirectedS,
                       PRPotential, EdgeProperties> Graph;


typedef property_map<Graph, PRPotential> vertex_prop_map;
typedef property_map<Graph, EdgeProperties> edge_prop_map;

