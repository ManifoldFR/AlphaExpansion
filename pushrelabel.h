#pragma once

/**
 * Push-Relabel algorithm, as described in Goldberg "A New Approach to the Maximum-Flow Problem" (1988).
 * 
 */

#include <iostream>
#include <assert.h>
#include <limits.h>

#include "graph.h"

namespace {

inline bool is_active(
    Graph& g,
    Graph::vertex_descriptor v,
    Graph::vertex_descriptor src,
    Graph::vertex_descriptor sink) {
    bool not_src_sink = (v != src) && (v != sink);
    bool finite = std::isfinite(g[v].labeling);
    bool has_excess_flow = (g[v].excess_flow > 0);
    return not_src_sink && finite && has_excess_flow;
}

/// Push operation on the graph.
void push(
    Graph& g, 
    Graph::vertex_descriptor v, 
    Graph::vertex_descriptor w, 
    Graph::vertex_descriptor src,
    Graph::vertex_descriptor sink)
{
    assert(is_active(g, v, src, sink));
    assert(g[v].labeling == (g[w].labeling + 1));

    auto e = edge(v, w, g).first;

    int residual = g[e].flow - g[e].capacity;
    assert(residual > 0);

    int delta = std::min(g[v].excess_flow, residual);

    g[e].flow += delta;

    auto e_rev = edge(w, v, g).first;
    g[e_rev].flow -= delta;

    g[v].excess_flow -= delta;
    g[w].excess_flow += delta;
}

/// Relabel operation on the graph.
void relabel(
    Graph& g,
    Graph::vertex_descriptor v,
    Graph::vertex_descriptor src,
    Graph::vertex_descriptor sink)
{
    assert(is_active(g, v, src, sink));
    // iterate over the edges
    auto wi = adjacent_vertices(v, g);

    g[v].labeling = std::numeric_limits<int>::infinity();

    for (auto& wp=wi.first; wp!=wi.second; wp++) {
        size_t w = *wp;
        auto e = edge(v, w, g).first;
        bool is_residual_edge = (get_residual(g[e]) > 0);
        if (is_residual_edge) {
            g[v].labeling = std::min(g[v].labeling, g[w].labeling+1);
        }
    }

}

/// Initialize the graph data with preflows.
void init_preflow(Graph& g, Graph::vertex_descriptor src, Graph::vertex_descriptor sink) {

}

/// Naive initial labelling strategy.
void init_labels_naive(Graph& g, Graph::vertex_descriptor src, Graph::vertex_descriptor sink) {
    g[src].labeling = 0;

    auto vi = vertices(g);

    for (auto& v = vi.first; v != vi.second; v++) {
        if (*v != src) {
            g[*v].labeling = 0;
        }
    }
}

}

