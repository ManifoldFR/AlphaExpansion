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


vector<int> buildLabels(vector<vector<int>> unaryPotential) {
    //Graph Constructor to use at the begining
    vector<int> labels;

    //Computations of the initial labels
    for (int i=0; i<unaryPotential.size(); i++){
        labels.push_back(std::distance(unaryPotential[i].begin(), min_element(unaryPotential[i].begin(), unaryPotential[i].end())));
    }
    return labels;
}

Graph buildGraph(int label, vector<int> labels, vector<vector<int>> unaryPotential, vector<vector<int>> edges){
    //Initialize graph with approriate edges information
    Graph G;
    int numberNodes = labels.size();
    int sink = numberNodes;
    int source = numberNodes + 1;
    int new_node = source +1;
    for (int i = 0; i<edges.size(); i++){
        int idxS = edges[i].at(0);
        int idxT = edges[i].at(1);
        if (labels.at(idxS) == labels.at(idxT) ){
            boost::add_edge(idxS, idxT, EdgeProperties{0,0}, G);
            boost::add_edge(idxT, idxS, EdgeProperties{0,0}, G);
        }
        else if (labels.at(idxS) == label){
            boost::add_edge(idxS, idxT, EdgeProperties{0,0}, G);
            boost::add_edge(idxT, idxS, EdgeProperties{100,0}, G);
        }
        else if (labels.at(idxT) == label){
            boost::add_edge(idxS, idxT, EdgeProperties{100,0}, G);
            boost::add_edge(idxT, idxS, EdgeProperties{0,0}, G);
        }
        else{
            boost::add_edge(idxS, new_node, EdgeProperties{100,0}, G);
            boost::add_edge(new_node, idxS, EdgeProperties{0,0}, G);
            boost::add_edge(new_node, idxT, EdgeProperties{0,0}, G);
            boost::add_edge(idxT, new_node, EdgeProperties{100,0}, G);
            boost::add_edge(new_node, sink, EdgeProperties{100,0}, G);
            boost::add_edge(sink, new_node, EdgeProperties{0,0}, G);
            boost::add_edge(source, new_node, EdgeProperties{numeric_limits<int>::max(),0}, G);
            boost::add_edge(new_node, source, EdgeProperties{0,0}, G);
            new_node++;
        }
    }

    for (int i=0; i<unaryPotential.size(); i++){
        boost::add_edge(source, i, EdgeProperties{unaryPotential[i].at(label),0}, G);
        boost::add_edge(i, source, EdgeProperties{0,0}, G);
        if (labels.at(i) == label){
            boost::add_edge(i, sink, EdgeProperties{numeric_limits<int>::max(),0}, G);
            boost::add_edge(sink, i, EdgeProperties{0,0}, G);
        }
        else{
            boost::add_edge(i, sink, EdgeProperties{unaryPotential[i].at(labels[i]),0}, G);
            boost::add_edge(sink, i, EdgeProperties{0,0}, G);
        }

    }

    return G;
}

vector<int> getLabel(Graph G, vector<int> labels, int label, const Graph::vertex_descriptor& src, const Graph::vertex_descriptor& sk){
    
    vector<int> new_labels;
    
    auto vs = vertices(G);

    for (auto &it = vs.first; *it != labels.size(); it++) {
        if ((*it != src) && (*it != sk)){
           new_labels.push_back(G[*it].cut_class * label + (1-G[*it].cut_class) * labels.at(*it));
        }
    }

    return new_labels;
    /*Graph::vertex_iterator v, vend;
    for (boost::tie(v, vend) = vertices(G); v != vend; v++) {
        int i = *v;
        new_labels.push_back(G[*v].cut_class * label + (1 - G[*v].cut_class) * labels.at(i));

    }
    return new_labels;
    */
}

void setLabel(vector<int>& oldLabels, vector<int>& newLabels){
    for (int i=0; i<oldLabels.size(); i++){
        oldLabels[i] = newLabels[i];
    }
}

int computeEnergy(vector<int> labels, vector<vector<int>> unaryPotential, vector<vector<int>> edges){
    int energy = 0;
    for (int i =0; i<labels.size(); i++){
        energy = energy + unaryPotential[i].at(labels[i]);
    }
    for (int j=0; j<edges.size(); j++){
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
    cout << minEnergy << endl;
    int bestLabel = -1;
    vector<int> bestLabels;
    vector<int> localLabels;
    Graph G; 

    size_t numberNodes = unaryPotential.size();
    Graph::vertex_descriptor sk = numberNodes;
    Graph::vertex_descriptor src = numberNodes + 1;

    for (int i=0; i<unaryPotential[0].size(); i++){
        int localLabel = i;
        G = buildGraph(localLabel, labels, unaryPotential, edges);
        compute_min_cut(G, src, sk);

        localLabels = getLabel(G, labels, localLabel, src, sk);

        int graphEnergy = computeEnergy(localLabels, unaryPotential, edges);
        
        cout << graphEnergy << endl;
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