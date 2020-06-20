#include <pybind11/pybind11.h>

#include <omnetpp.h>
#include <omnetpp/csimulation.h>
#include <omnetpp/simtime.h>
#include <omnetpp/cpar.h>


void bind_cSimTime(pybind11::module &m)
{
    // simTime ------------------------------------------------------------------------------------
    pybind11::enum_<omnetpp::SimTimeUnit>(m, "_SimTimeUnit")
        .value("SIMTIME_S",  omnetpp::SimTimeUnit::SIMTIME_S)
        .value("SIMTIME_MS", omnetpp::SimTimeUnit::SIMTIME_MS)
        .value("SIMTIME_US", omnetpp::SimTimeUnit::SIMTIME_US)
        .value("SIMTIME_NS", omnetpp::SimTimeUnit::SIMTIME_NS)
        .value("SIMTIME_PS", omnetpp::SimTimeUnit::SIMTIME_PS)
        .value("SIMTIME_FS", omnetpp::SimTimeUnit::SIMTIME_FS)
        .value("SIMTIME_AS", omnetpp::SimTimeUnit::SIMTIME_AS)
        .export_values();

    m.def("_simTime", &omnetpp::simTime);

    pybind11::class_<omnetpp::SimTime> py_SimTime(m, "_SimTime");

    py_SimTime.def(pybind11::init<>());
    py_SimTime.def(pybind11::init<double>());
    py_SimTime.def(pybind11::init<omnetpp::cPar &>());
    py_SimTime.def(pybind11::init<int, omnetpp::SimTimeUnit>());
    py_SimTime.def(pybind11::init<const omnetpp::SimTime&>());

    py_SimTime.def("isZero", &omnetpp::SimTime::isZero);
    py_SimTime.def("dbl", &omnetpp::SimTime::dbl);
    py_SimTime.def("raw", &omnetpp::SimTime::raw);
    py_SimTime.def("getMaxTime", &omnetpp::SimTime::getMaxTime);
    py_SimTime.def("setRaw", &omnetpp::SimTime::setRaw);
    py_SimTime.def("__str__", pybind11::overload_cast<>(&omnetpp::SimTime::str, pybind11::const_));
    py_SimTime.def("str", pybind11::overload_cast<>(&omnetpp::SimTime::str, pybind11::const_));
    py_SimTime.def("__add__", [] (const omnetpp::simtime_t &a, const omnetpp::simtime_t &b) {return a + b;});
    py_SimTime.def("__add__", [] (const omnetpp::simtime_t &a, double b) {return a + omnetpp::SimTime(b);});
    py_SimTime.def("__mul__", [] (const omnetpp::simtime_t &a, double b) {return a * b;});
    py_SimTime.def("__truediv__", [] (const omnetpp::simtime_t &a, const omnetpp::simtime_t &b) {return a / b;});
    py_SimTime.def("__truediv__", [] (const omnetpp::simtime_t &a, double b) {return a / omnetpp::SimTime(b);});
    py_SimTime.def("__sub__", [] (const omnetpp::simtime_t &a, const omnetpp::simtime_t &b) {return a - b;});
    py_SimTime.def("__gt__", [] (const omnetpp::simtime_t &a, const omnetpp::simtime_t &b) {return a > b;});
    py_SimTime.def("__gte__", [] (const omnetpp::simtime_t &a, const omnetpp::simtime_t &b) {return a >= b;});
    py_SimTime.def("__lt__", [] (const omnetpp::simtime_t &a, const omnetpp::simtime_t &b) {return a < b;});
    py_SimTime.def("__lte__", [] (const omnetpp::simtime_t &a, const omnetpp::simtime_t &b) {return a <= b;});
    py_SimTime.def_readonly_static("ZERO", &omnetpp::SimTime::ZERO);
}
