#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <limits>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include<boost/range/numeric.hpp>

#include "graph.h"
#include "pushrelabel.h"
#include "AlphaExpansion.h"

using namespace std;


vector<int> buildLabels(const vector<vector<int>>& unaryPotential) {
    //Graph Constructor to use at the begining
    vector<int> labels;

    //Computations of the initial labels
    for (int i=0; i<unaryPotential.size(); i++){
        labels.push_back(std::distance(unaryPotential[i].begin(), min_element(unaryPotential[i].begin(), unaryPotential[i].end())));
    }
    return labels;
}

Graph buildGraph(const int label, const vector<int>& labels, const vector<vector<int>>& unaryPotential, vector<vector<int>> edges){
    
    // numberNodes is the number of "true" nodes, corresponding to data points
    int numberNodes = unaryPotential.size();

    // indices of sink and source
    Graph::vertex_descriptor sink = numberNodes;
    Graph::vertex_descriptor source = numberNodes + 1;
    
    //Initialize graph with approriate edges information
    Graph G(numberNodes + 2);

    // Nodes indexed by n > numberNodes + 1 are intermediary nodes 
    // added when two neighbors have different classes != label 

    auto z = numberNodes + 2;
    
    // first, add all edges to the graph, we'll change them later

    std::cout << "Adding all the edges " << std::endl;

    for (int e = 0; e < edges.size(); e++) {
        auto u(edges[e][0]), v(edges[e][1]);
        boost::add_edge(u, v, {0, 0}, G);
        boost::add_edge(v, u, {0, 0}, G);
    }

    int i = 0;
    int inf = std::numeric_limits<int>::max();
    // int inf = 10000;

    // first, potentials w/ source and sink

    std::cout << "Computing potentials w/ source and sink " << std::endl;

    for (Graph::vertex_descriptor i = 0; i < numberNodes; i++) {

        // add edge to source : weights are all w/ 'label'
        boost::add_edge(source, i, {unaryPotential[i][label], 0}, G);
        boost::add_edge(i, source, {0, 0}, G);

        // add edge to sink : weight depends on current class
        if (labels[i] == label)
            boost::add_edge(i, sink, {inf, 0}, G);
        else
            boost::add_edge(i, sink, {unaryPotential[i][labels[i]], 0}, G);
    
        boost::add_edge(sink, i, {0, 0}, G);

    }

    for (Graph::vertex_descriptor i = 0; i < numberNodes; i++) {

        auto neighs = boost::adjacent_vertices(i, G);
        // contains neighbors on which add intermediary node
        std::vector<int> to_add = std::vector<int>();     

        for (auto &n = neighs.first; n != neighs.second; n++) {
            // only consider inferior vertices (to not compute twice)
            if ((*n == source) || (*n == sink) || (*n >= i))
                continue;

            if (labels[i] == label && labels[*n] == label)
                continue;

            auto edge = boost::edge(i, *n, G).first;
            auto rev_edge = boost::edge(*n, i, G).first;

            // 4 cases for inter-node weights
            // Case 1 : one of them is alpha = 'label'
            if (labels[i] == label){
                G[edge].capacity = 100;
            }
            else if(labels[*n] == label) {
                G[rev_edge].capacity = 100;
            }
            else if (labels[i] == labels[*n])
            {
                G[edge].capacity = 100;
                G[rev_edge].capacity = 100;
            }
            else
            {
                to_add.push_back(*n);
            }

        }

        // add relevant intermediary nodes
        for (Graph::vertex_descriptor j = 0; j < to_add.size(); j++) {
            auto n = to_add[j];

            boost::add_vertex(G);
            boost::add_edge(i, z, {100, 0}, G);
            boost::add_edge(z, i, {inf, 0}, G);

            boost::add_edge(n, z, {100, 0}, G);
            boost::add_edge(z, n, {inf, 0}, G);
            
            boost::add_edge(z, sink, {100, 0}, G);
            boost::add_edge(sink, z, {0, 0}, G);
            z++;
        }
        
    }

    return G;
}

vector<int> getLabel(Graph G, const vector<int>& labels, const int label, const Graph::vertex_descriptor& src, const Graph::vertex_descriptor& sk){
    
    vector<int> new_labels;
    
    auto vs = vertices(G);

    for (auto &it = vs.first; it != vs.second; it++) {
        if ((*it != src) && (*it != sk)){
           new_labels.push_back(G[*it].cut_class * label + (1-G[*it].cut_class) * labels.at(*it));
        }
    }

    return new_labels;
}

void setLabel(vector<int>& oldLabels, vector<int>& newLabels){
    for (int i=0; i<oldLabels.size(); i++){
        oldLabels[i] = newLabels[i];
    }
}

int computeEnergy(const vector<int>& labels, const vector<vector<int>>& unaryPotential, const vector<vector<int>>& edges){
    int energy = 0;
    for (int i =0; i<labels.size(); i++){
        energy = energy + unaryPotential[i].at(labels[i]);
    }
    for (int j=0; j < edges.size(); j++){
        int idxS = edges[j].at(0);
        int idxT = edges[j].at(1);
        if (labels[idxS] != labels[idxT]){
            energy = energy + 100;
        }      
    }
    return energy;
}


bool expansion(vector<int>& labels, vector<vector<int>> unaryPotential, vector<vector<int>> edges){

    int minEnergy = computeEnergy(labels, unaryPotential, edges);
    std::cout << minEnergy << endl;
    int bestLabel = -1;
    vector<int> bestLabels;
    vector<int> localLabels;
    Graph G; 

    size_t numberNodes = unaryPotential.size();
    Graph::vertex_descriptor sk = numberNodes;
    Graph::vertex_descriptor src = numberNodes + 1;

    for (int i = 0; i<unaryPotential[0].size(); i++){
        int localLabel = i;

        std::cout << "Building graph ... " << std::endl;

        G = buildGraph(localLabel, labels, unaryPotential, edges);
        
        std::cout << "Computing min cut ... " << std::endl;
        
        compute_min_cut(G, src, sk);

        localLabels = getLabel(G, labels, localLabel, src, sk);

        int graphEnergy = computeEnergy(localLabels, unaryPotential, edges);
        
        std::cout << graphEnergy << endl;
        if (graphEnergy < minEnergy){
            minEnergy = graphEnergy;
            bestLabels = localLabels;
            bestLabel = i;
        }

    }
    
    if (bestLabel > -1){
        setLabel(labels, bestLabels);
        return true;
    }
    else{
        return false;
    }

}

pair<int,vector<int>> applyAlphaExpansion(vector<vector<int>> unaryPotential, vector<vector<int>> edges){

    pair <int,vector<int>>  result;

    vector<int> labels = buildLabels(unaryPotential);
    cout << "Pre label computed" <<endl;
    bool modified = true;
    while(modified){
        cout << "Computing expansion" << endl;
        modified = expansion(labels, unaryPotential, edges);
    }
    int finalEnergy = computeEnergy(labels, unaryPotential, edges);

    result = make_pair (finalEnergy, labels);
    return result;
}