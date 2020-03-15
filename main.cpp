#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include <time.h>
#include <algorithm>
#include "AlphaExpansion.h"
#include <vector>


////////////////////////////////////////////////////////////////////////////////
/**
 * argv[0] nodes/edges folder path
 * 
 */
int main(int argc, char **argv)
{
	using namespace std;
	string folder_path(argv[1]);
	cout << "Looking for data files in " << folder_path << endl;

	auto nodes_file = folder_path + "/nodes.txt";
	cout << nodes_file << endl;

	//get nodes
	ifstream infile2(nodes_file);
	int a,b,c,d,e,f ;
	vector<vector<int>> probabilites;
	while (infile2 >> a >> b >> c >> d >> e >> f)
	{
        vector<int> local_proba;
        local_proba.push_back(-a);
        local_proba.push_back(-b);
        local_proba.push_back(-c);
        local_proba.push_back(-d);
        local_proba.push_back(-e);
        local_proba.push_back(-f);
        probabilites.push_back(local_proba);
	}

	auto edge_file = folder_path + "/edges.txt";
	cout << edge_file << endl;

	//get edges

    vector<vector<int>> edges;
	ifstream infile4(edge_file);
	int a2,b2 ;
	while (infile4 >> a2 >> b2)
	{
        vector<int> local_edge;
        local_edge.push_back(a2);
        local_edge.push_back(b2);
        edges.push_back(local_edge);
	}

	auto label_file = folder_path + "/labels.txt";
	//Will pretend our graph is general, and set up a neighborhood system
	// which actually is a grid. Also uses spatially varying terms
	cout << "Start alpha expansion" << endl;
    pair<int,vector<int>> results = applyAlphaExpansion(probabilites, edges);
    vector<int> result = get<1>(results);
	ofstream o(label_file);
	for(int i = 0; i<result.size();i++){
		o<<result[i]+1<<"\n";
	}
	printf("Alpha expansion done.\n");
	flush(o);

	// Call python code to check if the results are good
	system("python check_labels.py");

	return 0;
}

/////////////////////////////////////////////////////////////////////////////////

