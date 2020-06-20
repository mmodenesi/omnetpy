#include <pybind11/pybind11.h>

#include <omnetpp.h>
#include <omnetpp/coutvector.h>

void bind_cOutVector(pybind11::module &m)
{
    pybind11::class_<omnetpp::cOutVector> py_cOutVector(m, "_cOutVector");

    py_cOutVector.def(
        pybind11::init<const char *>(),
        pybind11::arg("name") = nullptr
    );
    py_cOutVector.def(
        "record",
        // &cOutVector::record
        pybind11::overload_cast<double>(&omnetpp::cOutVector::record)
    );
    py_cOutVector.def(
        "setName",
        &omnetpp::cOutVector::setName
    );
}
