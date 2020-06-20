#include <pybind11/pybind11.h>

#include <omnetpp.h>
#include <omnetpp/cstddev.h>


void bind_cStdDev(pybind11::module &m)
{
    pybind11::class_<omnetpp::cStdDev> py_cStdDev(m, "_cStdDev");

    py_cStdDev.def(
        pybind11::init<const char *, bool >(),
        pybind11::arg("name") = nullptr, pybind11::arg("weighted") = false
    );
    py_cStdDev.def("setName", &omnetpp::cStdDev::setName);
    py_cStdDev.def("getCount", &omnetpp::cStdDev::getCount);
    py_cStdDev.def("getMean", &omnetpp::cStdDev::getMean);
    py_cStdDev.def(
        "collect",
        pybind11::overload_cast<double>(&omnetpp::cStdDev::collect)
    );
}
