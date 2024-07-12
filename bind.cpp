#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "Simulation.h"

namespace py = pybind11;

PYBIND11_MODULE(Simulation,m){
    py::class_<NetWorkSimulation>(m, "Simulation")
    .def(py::init<double,double,int,double>())
    .def("initNode",&NetWorkSimulation::initNode)
    .def("initLink",&NetWorkSimulation::initLink)
    .def("UpdateLink",&NetWorkSimulation::UpdateLink)
    .def("UpdateRateMap",&NetWorkSimulation::UpdateRateMap)
    .def("createTask",&NetWorkSimulation::createTask)
    .def("NextFinish",[](NetWorkSimulation* NS){
        long long taskid;
        double now;
        NS->NextFinish(taskid,now);
        return std::make_pair(taskid,now);
    })
    .def("startEventFlash",&NetWorkSimulation::startEventFlash)
    .def("blockAll",&NetWorkSimulation::blockAll)
    .def("start",&NetWorkSimulation::start)
    .def("stop",&NetWorkSimulation::stop)
    ;

    // py::class_<IntervalTree>(m, "IntervalTree")
    // .def(py::init<double,double,double>())
    // .def("addValue",&IntervalTree::addValue)
    // .def("getValue",&IntervalTree::getValue)
    // .def("releaseLeft",&IntervalTree::releaseLeft)
    // .def("intoNextWindows",static_cast<void (IntervalTree::*)()>(&IntervalTree::intoNextWindows))
    // .def("intoNextWindows_D",static_cast<void (IntervalTree::*)(double)>(&IntervalTree::intoNextWindows))
    // .def("changeDefualtValue",&IntervalTree::changeDefualtValue)
    // .def("AllocatedArea_DD",&IntervalTree::AllocatedArea_DD)
    // .def("AllocatedArea_DDD",&IntervalTree::AllocatedArea_DDD)
    // .def("viewList",&IntervalTree::viewList);
}