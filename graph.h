#pragma once

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <assert.h>

using namespace boost;

const int BIG_INTEGER = std::numeric_limits<int>::max();


/// Node properties for the PR algorithm
struct PRPotential {
    /// Excess flow at the node.
    int excess_flow = 0;
    /// Label d for the PR algorithm : height of the node
    int labeling;
    /// Class of the node after min cut : 0 if in class of source, 1 otherwise
    int cut_class;
};

/// Edge property, with both flow value and edge capacity.
struct EdgeProperties {
    int capacity = 0;
    int flow = 0;
};

/// 

int get_residual(EdgeProperties e) {
    return e.capacity - e.flow;
}

typedef adjacency_list<vecS, vecS, bidirectionalS,
                       PRPotential, EdgeProperties> Graph;

typedef property_map<Graph, PRPotential> vertex_prop_map;
typedef property_map<Graph, EdgeProperties> edge_prop_map;

void push_flow(const Graph::vertex_descriptor& v, const Graph::vertex_descriptor& w, Graph& g, int to_push) {
    auto edge = boost::edge(v, w, g).first;
    auto rev_edge = boost::edge(w, v, g).first;

    assert (g[edge].flow + to_push <= g[edge].capacity);
    
    g[edge].flow     += to_push;
    // get negative flow with capacity >= 0 so we can push back
    g[rev_edge].flow -= to_push;
    
    g[v].excess_flow -= to_push;
    g[w].excess_flow += to_push;

}
