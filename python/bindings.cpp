#include <pybind11/pybind11.h>


namespace py = pybind11;

PYBIND11_MODULE(pyalphaexpansion, m) {
    m.doc() = "Alpha expansion algorithm.";
}
