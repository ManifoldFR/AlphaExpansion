#include <iostream>
#include <utility>
#include <vector>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

#include "graph.h"
#include "pushrelabel.h"
#include "AlphaExpansion.h"

std::vector<int> getLabel(Graph G){
    std::vector<int> labels;
    return labels;
}

void setLabel(Graph G, std::vector<int> labels){
}

int computeEnergy(Graph G, int* unaryPotential, int* binaryPotential){
    int energy = 0;
    return energy;
}

bool expansion(Graph G, std::vector<int> labels, int* unaryPotential, int* binaryPotential){
    int minEnergy = computeEnergy(G, unaryPotential, binaryPotential);
    int bestLabel = -1;
    std::vector<int> bestLabels = getLabel(G);
    std::vector<int> originLabels = getLabel(G);
    for (int i=0; i<labels.size(); i++){
        if (i>0){
            setLabel(G, originLabels);
        }
        int localLabel = labels[i];
        compute_min_cut(G, src, sink);
        int graphEnergy = computeEnergy(G, unaryPotential, binaryPotential);
        if (graphEnergy < minEnergy){
            minEnergy = graphEnergy;
            bestLabels = getLabel(G);
            bestLabel = i;
        }

    }
    setLabel(G, bestLabels);
    if (bestLabel > -1){
        return true;
    }
    else{
        return false;
    }

}

int applyAlphaExpansion(Graph G, std::vector<int> labels, int* unaryPotential, int* binaryPotential){
    bool modified = true;
    while(modified){
        modified = expansion(G, labels, unaryPotential, binaryPotential);
    }
    int finalEnergy = computeEnergy(G, unaryPotential, binaryPotential);
    return finalEnergy;
}