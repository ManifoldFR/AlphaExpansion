#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <AlphaExpansion.h>
#include <vector>

namespace py = pybind11;

std::pair<int,std::vector<int>> my_func(std::vector<std::vector<int>> potentials, std::vector<std::vector<int>> edges)
{
    auto res_ = applyAlphaExpansion(potentials, edges);
    return res_;
}


PYBIND11_MODULE(pyalphaexpansion, m) {
    m.doc() = "Alpha expansion algorithm.";

    m.def("apply_alpha_expansion", &my_func,
          "Apply the Alpha Expansion algorithm to the input.");
}
