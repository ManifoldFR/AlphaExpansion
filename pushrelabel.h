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
#include <boost/graph/visitors.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>


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
    return (g[v].labeling == g[w].labeling + 1) && get_residual(g[e]) > 0;
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
    auto edge = boost::edge(v, w, g).first;

    int residual = get_residual(g[edge]);

    int delta = std::min(g[v].excess_flow, residual);

    push_flow(v, w, g, delta);
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
    auto push_preflow = [&g, src](Graph::vertex_descriptor neighbor)
    {
        auto e = edge(src, neighbor, g).first;
        auto rev_e = edge(neighbor, src, g).first;
        g[e].flow = g[e].capacity;
        g[rev_e].flow = -g[e].capacity;

        g[neighbor].excess_flow = g[e].flow;
    };

    auto neighs = adjacent_vertices(src, g);
    for_each(neighs.first, neighs.second, push_preflow);
}

/// Naive initial labelling strategy.
void init_labels_naive(Graph &g, const Graph::vertex_descriptor& src, const Graph::vertex_descriptor& sink)
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

/// Smarter initial labelling strategy.
/// See Goldberg (1988), p. 926.
/// The idea is to set d(v) = min(d_{G_f}(v,t), d_{G_f}(v,s) + n) for all vertices v
/// where d_{G_f} is the shortest-path metric inside of the residual graph
// TODO: FINISH THIS
void init_labels_smart(Graph &g, const Graph::vertex_descriptor& src, const Graph::vertex_descriptor& sink)
{
    auto vi = vertices(g);  // vertex iterator



    // STEP ONE define shortest-paths in the residual graph

    


    // STEP TWO assemble the labels

    for (auto &v = vi.first; v != vi.second; v++)
    {

    }
}

/// Applies push relabel for a given graph to compute maximum flow
/// This function should not modify any adjacency list (cf. https://www.boost.org/doc/libs/1_37_0/libs/graph/doc/adjacency_list.html)

bool push_relabel(Graph &g, const Graph::vertex_descriptor& src, const Graph::vertex_descriptor& sink, bool verbose = true)
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
        // only push in queue the now active nodes (rest are useless)
        if (is_active(g, *it, src, sink))
            q.push(*it);
    }

    bool continuer = true;

    int push_count(0), relabel_count(0);

    if (verbose) std::cout << "Source & sink are " << src << " & " << sink << std::endl;

    // Loop invariant : any vertex that goes in the queue remains active until pushed out
    while (continuer)
    {

        // Pop active vertex from queue
        Graph::vertex_descriptor current = q.front();
        if ( verbose ) std::cout << "cv " << current << " : "; 

        auto current_neighs = boost::adjacent_vertices(current, g);
        bool pushed = false;

        bool can_push_to_sink = boost::edge(current, sink, g).second && can_push(g, current, sink);

        if (can_push_to_sink)
        {
        
            auto e = boost::edge(current, sink, g).first;
            if (verbose) std::cout << " push to " << sink << ".old_flow=" << g[e].flow << ".excess_flow=" << g[current].excess_flow << ".capacity=" << g[e].capacity;
            if (verbose) std::cout << ".residual flow to sink is " << get_residual(g[e]);
            pushed = true;
            push_count ++;
            push(g, current, sink, src, sink);
            if (verbose) std::cout << ". new_flow " << g[e].flow << ".new_excess_flow" << g[current].excess_flow << std::endl;
        
        }
        

        if (pushed == false) {
            // loop over neighbors and find if one can be pushed
            for (auto &it = current_neighs.first; it != current_neighs.second; it++)
            {
                if (!pushed && can_push(g, current, *it))
                {
                    pushed = true;
                    auto e = edge(current, *it, g).first;
                    if (verbose) std::cout << " push to " << *it << ".old_flow=" << g[e].flow << ".excess_flow=" << g[current].excess_flow << ".capacity=" << g[e].capacity;
                    push_count ++;
                    bool was_in_queue = is_active(g, *it, src, sink);
                    push(g, current, *it, src, sink);
                    if (verbose) std::cout << ". new_flow " << g[e].flow << ".new_excess_flow" << g[current].excess_flow << std::endl;
                    // We never push the sink or the source into the queue
                    if (!was_in_queue && is_active(g, *it, src, sink))
                        q.push(*it);
                }
            }
        }

        // if not pushed, we have to relabel the vertex
        if (!pushed)
        {
            if (verbose) std::cout << ". relabel" << std::endl;
            relabel(g, current, src, sink);
            relabel_count++;
        }

        // Check if current vertex is no longer active
        if (!is_active(g, current, src, sink))
        {
            q.pop();
        }

        if (verbose) std::cin.get();

        // quit if queue is empty
        continuer = !q.empty();
    }

    std::cout << "Number of push and relabel done : " << push_count << " & " << relabel_count << std::endl;

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
            if ((get_residual(g[e]) > 0) && (g[*it].cut_class == 1)){
                q.push(*it);
            }
        }
    }
}

// Calls push_relabel and max_flow_to_min_cut to compute min cut from scratch
// returns the value of the min cut
long compute_min_cut(Graph &g, Graph::vertex_descriptor src, Graph::vertex_descriptor sink, bool verbose = false)
{
    if (verbose)
        std::cout << "Computing push relabel ... " << std::endl;

    std::cout << "Computing push relabel ... " << std::endl;

    // auto edge_capacities = get(&EdgeProperties::capacity, g);
    // auto edge_rev = get(boost::edge_reverse, g);
    // push_relabel_max_flow(g, src, sink);
    push_relabel(g, src, sink, verbose);

    if (verbose)
        std::cout << "Computing min cut ... " <<  std::endl;

    max_flow_to_min_cut(g, src, sink);

    if (verbose)
        std::cout << "Finished computing min cut" << std::endl;

    long min_cut = 0;
    auto edges = boost::edges(g);
    for(auto &it = edges.first; it != edges.second; it++) {
        auto v(source(*it, g)), w(target(*it, g));
        if (g[v].cut_class == 0 && g[w].cut_class == 1)
            min_cut += g[*it].capacity;
    }

    return min_cut;
}

long push_relabel_boost_version(BoostGraph& bg, Graph::vertex_descriptor src, Graph::vertex_descriptor sink)
{

    auto cap_map = get(&BoostEdgeProperties::capacity, bg);
    auto res_cap_map = get(&BoostEdgeProperties::residual_capacity, bg);
    auto rev_map = get(&BoostEdgeProperties::reverse, bg);
    auto indx_map = get(vertex_index, bg);

    long flow =  boost::push_relabel_max_flow(bg, src, sink, cap_map, res_cap_map, rev_map, indx_map);

    return flow;
}


/// Computes the min cut and returns its value
void max_flow_to_min_cut_boost(Graph& g, const BoostGraph& bg,  Graph::vertex_descriptor src, Graph::vertex_descriptor sink)
{
    auto num_verts = boost::num_vertices(bg);

    // Copy capacity into old graph
    for (size_t i = 0; i < num_verts; i++)
    {
        auto neighs = boost::adjacent_vertices(i, g);
        for (auto &j = neighs.first; j != neighs.second; j++)
        {
            auto edge = boost::edge(i, *j, g).first;
            auto b_edge = boost::edge(i, *j, bg).first;
            
            g[edge].flow = (bg[b_edge].capacity - bg[b_edge].residual_capacity);
        }
    }

    max_flow_to_min_cut(g, src, sink);
}

long compute_min_cut_boost(Graph &g, Graph::vertex_descriptor src, Graph::vertex_descriptor sink)
{
    
    auto bg = graph_to_boost_graph(g);
    long flow = push_relabel_boost_version(bg, src, sink);

    max_flow_to_min_cut_boost(g, bg, src, sink);

    return flow;
}

} // namespace
