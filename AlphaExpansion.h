#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>


typedef std::pair<int, int> Edge;

using namespace boost;
typedef adjacency_list<vecS, vecS, bidirectionalS,
    property<vertex_color_t, default_color_type>> Graph;

