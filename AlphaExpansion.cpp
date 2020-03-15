#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <limits>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/range/numeric.hpp>

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

/// Reparametrization for binary graph cuts.
/// This algorithm ensures all capacities are non-negative.
/// Book URL: http://www0.cs.ucl.ac.uk/external/s.prince/book/Algorithms.pdf
void reparametrize(Graph& G)
{

}


/// Build the graph for alpha expansion.
/// See Simon J.D. Prince, "Computer Vision: Models, Learning, and Inference", p. 41
/// Book URL: http://www0.cs.ucl.ac.uk/external/s.prince/book/Algorithms.pdf
Graph buildGraph(int label, const vector<int>& labels, const vector<vector<int>>& unaryPotential, const vector<vector<int>>& edges){
    //Initialize graph with approriate edges information
    int numberNodes = labels.size();
    
    Graph G(numberNodes + 2);
    
    int sink = numberNodes;
    int source = sink + 1;
    int new_node = source + 1;  // denoted z in the book

    for (int i = 0; i<edges.size(); i++){
        int idxS = edges[i].at(0);
        int idxT = edges[i].at(1);
        if (labels.at(idxS) == labels.at(idxT) && labels.at(idxS) == label ){
            // Case 1
            ;
        }
        else if (labels.at(idxS) == labels.at(idxT) && labels.at(idxS) != label ){
            // Case 3
            add_edge_clean(idxS, idxT, G, 100, 100); // CAUTION
            /*
            boost::add_edge(idxS, idxT, EdgeProperties{100,0}, G);
            boost::add_edge(idxT, idxS, EdgeProperties{100,0}, G);         
            */
        }
        else if (labels.at(idxS) == label){
            // Case 2b
            add_edge_clean(idxT, idxS, G, 100);
            // boost::add_edge(idxT, idxS, EdgeProperties{100,0}, G);
        }
        else if (labels.at(idxT) == label){
            // Case 2a
            add_edge_clean(idxS, idxT, G, 100);
            // boost::add_edge(idxS, idxT, EdgeProperties{100,0}, G);
        }
        else{
            // Case 4
            new_node = boost::num_vertices(G); // CAUTION
            boost::add_vertex(G);

            add_edge_clean(idxS, new_node, G, 100, BIG_INTEGER); // CAUTION
            add_edge_clean(idxT, new_node, G, 100, BIG_INTEGER); // CAUTION

            /*
            boost::add_edge(idxS, new_node, EdgeProperties{100,0}, G);
            boost::add_edge(new_node, idxS, EdgeProperties{BIG_INTEGER,0}, G);
            
            boost::add_edge(new_node, idxT, EdgeProperties{BIG_INTEGER,0}, G);
            boost::add_edge(idxT, new_node, EdgeProperties{100,0}, G);
            */

            boost::add_edge(new_node, sink, EdgeProperties{100,0},  G);
            /*
            new_node++;
            */
        }
    }

    for (int i=0; i<unaryPotential.size(); i++){
        if (labels.at(i) == label){ 
            boost::add_edge(source, i, EdgeProperties{0,0}, G);
            boost::add_edge(i, sink, EdgeProperties{BIG_INTEGER, 0}, G);
        }
        else{
            int smallest = min(unaryPotential[i].at(label),unaryPotential[i].at(labels[i]));
            boost::add_edge(source, i, EdgeProperties{unaryPotential[i].at(label) - smallest,0}, G);
            boost::add_edge(i, sink, EdgeProperties{unaryPotential[i].at(labels[i]) - smallest, 0}, G);
        }
    }

    return G;
}

vector<int> getLabel(const Graph& G, vector<int> labels, int label, const Graph::vertex_descriptor& src, const Graph::vertex_descriptor& sk){
    
    vector<int> new_labels;
    
    auto vs = vertices(G);

    for (auto &it = vs.first; it != vs.second; it++) {
        if (*it < labels.size() && (*it != src) && (*it != sk)){
           new_labels.push_back(G[*it].cut_class * label + (1-G[*it].cut_class) * labels.at(*it));
        }
    }

    return new_labels;
}

void setLabel(vector<int>& oldLabels, const vector<int>& newLabels){
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

        std::cout << "Building graph to expand " << i << std::endl;

        G = buildGraph(localLabel, labels, unaryPotential, edges);

        symmetrize_graph(G);

        std::cout << "Computing min cut" << std::endl;

        compute_min_cut(G, src, sk);

        std::cout << "Computing local labels" << std::endl;

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