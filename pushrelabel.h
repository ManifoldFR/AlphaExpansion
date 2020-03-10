#pragma once

/**
 * Push-Relabel algorithm, as described in Goldberg "A New Approach to the Maximum-Flow Problem" (1988).
 * 
 */

#include <assert.h>
#include <iostream>
#include <iterator>
#include <limits.h>
#include <queue>

/// Definition of class Graph
#include "graph.h"

namespace
{

inline bool is_active(
    Graph &g,
    Graph::vertex_descriptor v,
    Graph::vertex_descriptor src,
    Graph::vertex_descriptor sink)
{
    bool not_src_sink = (v != src) && (v != sink);
    bool finite = std::isfinite(g[v].labeling);
    bool has_excess_flow = (g[v].excess_flow > 0);
    return not_src_sink && finite && has_excess_flow;
}

inline bool can_push(
    Graph &g,
    Graph::vertex_descriptor v,
    Graph::vertex_descriptor w)
{
    auto e = edge(v, w, g).first;
    return (g[v].labeling == g[w].labeling + 1) && (g[e].capacity - g[e].flow > 0);
}

// TODO : Function to identify to which neighbor we must push

/// Push operation on the graph.
void push(
    Graph &g,
    Graph::vertex_descriptor v,
    Graph::vertex_descriptor w,
    Graph::vertex_descriptor src,
    Graph::vertex_descriptor sink)
{
    auto e = edge(v, w, g).first;

    int residual = g[e].capacity - g[e].flow;

    int delta = std::min(g[v].excess_flow, residual);

    g[e].flow += delta;

    auto e_rev = edge(w, v, g).first;
    g[e_rev].flow -= delta;

    g[v].excess_flow -= delta;
    g[w].excess_flow += delta;
}

/// Relabel operation on the graph.
void relabel(
    Graph &g,
    Graph::vertex_descriptor v,
    Graph::vertex_descriptor src,
    Graph::vertex_descriptor sink)
{
    // iterate over the edges
    auto wi = adjacent_vertices(v, g);

    // can't label inf here so we take 2*|V| (should not be attainable I think)
    g[v].labeling = 2 * size(vertices(g));

    for (auto &wp = wi.first; wp != wi.second; wp++)
    {
        auto w = *wp;
        auto e = edge(v, w, g).first;
        bool is_residual_edge = (get_residual(g[e]) > 0);
        if (is_residual_edge)
        {
            g[v].labeling = std::min(g[v].labeling, g[w].labeling + 1);
        }
    }
}

/// Initialize the graph data with preflows.
void init_preflow(Graph &g, Graph::vertex_descriptor src, Graph::vertex_descriptor sink)
{
    auto neighs = adjacent_vertices(src, g);
    for (auto &it = neighs.first; it != neighs.second; it++)
    {
        auto e = edge(src, *it, g).first;
        auto rev_e = edge(*it, src, g).first;
        g[e].flow = g[e].capacity;
        g[rev_e].flow = -g[e].capacity;

        g[*it].excess_flow = g[e].flow;
    }
}

/// Naive initial labelling strategy.
void init_labels_naive(Graph &g, Graph::vertex_descriptor src, Graph::vertex_descriptor sink)
{
    auto vi = vertices(g);

    g[src].labeling = size(vi);

    for (auto &v = vi.first; v != vi.second; v++)
    {
        if (*v != src)
        {
            g[*v].labeling = 0;
        }
    }
}

/// Applies push relabel for a given graph to compute maximum flow
bool push_relabel(Graph &g, Graph::vertex_descriptor src, Graph::vertex_descriptor sink)
{

    // Push flow from source to its neighbors
    init_preflow(g, src, sink);
    // All heights are 0 initially except source (card(V))
    init_labels_naive(g, src, sink);

    // Need a queue to store active vertices
    typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
    std::queue<Vertex> q;
    // Push neighbors of source into queue
    auto src_neighs = boost::adjacent_vertices(src, g);
    for (auto &it = src_neighs.first; it != src_neighs.second; it++)
    {
        q.push(*it);
    }

    bool continuer = true;

    // Loop invariant : any vertex that goes in the queue remains active until pushed out
    while (continuer)
    {
        // Pop active vertex from queue
        Graph::vertex_descriptor current = q.front();

        auto current_neighs = boost::adjacent_vertices(current, g);
        bool pushed = false;
        // loop over neighbors and find if one can be pushed
        for (auto &it = current_neighs.first; it != current_neighs.second; it++)
        {
            auto e = edge(current, *it, g).first;
            if (can_push(g, current, *it))
            {
                pushed = true;
                bool was_in_queue = is_active(g, *it, src, sink);
                push(g, current, *it, src, sink);
                // We never push the sink or the source into the queue
                if (!was_in_queue && is_active(g, *it, src, sink))
                    q.push(*it);
                break;
            }
        }

        // if not pushed, we have to relabel the vertex
        if (!pushed)
        {
            relabel(g, current, src, sink);
        }

        // Check if current vertex is no longer active
        if (!is_active(g, current, src, sink))
        {
            q.pop();
        }

        // quit if queue is empty
        continuer = !q.empty();
    }

    return true;
}

/// Using the maximum flow, computes the min cut and assign a class to each vertex
void max_flow_to_min_cut(Graph &g, Graph::vertex_descriptor src, Graph::vertex_descriptor sink)
{
    typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
    std::queue<Vertex> q;

    // step 1 : assign all vertices to target's class
    auto vs = vertices(g);
    for (auto &it = vs.first; it != vs.second; it++)
        g[*it].cut_class = 1;

    q.push(vertex(0, g));
    while (!q.empty()) {
        auto next = q.front();
        q.pop();
        g[next].cut_class = 0;

        // check neighbors accessible not using saturated edges
        auto neighs = adjacent_vertices(next, g);
        for (auto &it = neighs.first; it != neighs.second; it++) {
            auto e = edge(next, *it, g).first;
            if ((g[e].capacity - g[e].flow > 0) && (g[*it].cut_class == 1)){
                q.push(*it);
            }
        }
    }
}

// Calls push_relabel and max_flow_to_min_cut to compute min cut from scratch
void compute_min_cut(Graph &g, Graph::vertex_descriptor src, Graph::vertex_descriptor sink)
{

    push_relabel(g, src, sink);
    max_flow_to_min_cut(g, src, sink);

}

} // namespace
