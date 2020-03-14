#pragma once

#include <vector>

using namespace std;

#define BIG_INTEGER std::numeric_limits<int>::max()

pair<int,vector<int>> applyAlphaExpansion(vector<vector<int>> unaryPotential, vector<vector<int>> edges);