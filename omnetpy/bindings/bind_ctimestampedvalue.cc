#include <pybind11/pybind11.h>

#include <omnetpp.h>
#include <omnetpp/ctimestampedvalue.h>


void bind_cTimestampedValue(pybind11::module &m)
{
    pybind11::class_<omnetpp::cTimestampedValue, omnetpp::cObject> py_cTimestampedValue(
        m,
        "_cTimestampedValue",
        R"docstring(
        A straightforward implementation of cITimestampedValue.
        )docstring"
    );

    py_cTimestampedValue.def(
        pybind11::init<>()
    );

    py_cTimestampedValue.def(
        pybind11::init<omnetpp::simtime_t, const omnetpp::SimTime&>(),
        pybind11::arg("timestamp"), pybind11::arg("simtime")
    );

    py_cTimestampedValue.def(
        pybind11::init<omnetpp::simtime_t, const omnetpp::cObject*>(),
        pybind11::arg("timestamp"), pybind11::arg("cobject")
    );

    py_cTimestampedValue.def(
        pybind11::init<omnetpp::simtime_t, long>(),
        pybind11::arg("timestamp"), pybind11::arg("long")
    );

    py_cTimestampedValue.def(
        pybind11::init<omnetpp::simtime_t, double>(),
        pybind11::arg("timestamp"), pybind11::arg("double")
    );

    py_cTimestampedValue.def(
        pybind11::init<omnetpp::simtime_t, const char *>(),
        pybind11::arg("timestamp"), pybind11::arg("str")
    );

    py_cTimestampedValue.def(
        pybind11::init<omnetpp::simtime_t, bool>(),
        pybind11::arg("timestamp"), pybind11::arg("boolean")
    );
}
