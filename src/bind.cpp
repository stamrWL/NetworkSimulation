#include "pybind11/pybind11.h"

namespace py = pybind11;

PYBIND11_MODULE(pybind11, m) {
    m.doc() = "pybind11 example plugin";

    m.def("add", [](int i, int j) { return i + j; }, "A function which adds two numbers");
}