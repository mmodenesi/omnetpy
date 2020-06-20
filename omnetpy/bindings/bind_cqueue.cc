#include <pybind11/pybind11.h>
#include <pybind11/functional.h>

#include <omnetpp.h>
#include <omnetpp/cqueue.h>


void bind_cQueue(pybind11::module &m)
{

    pybind11::class_<omnetpp::cQueue> py_cQueue(m, "_cQueue");

    // TODO: this custom constructor only allows creating a FIFO
    py_cQueue.def(
        pybind11::init(
            [] (const char * name = nullptr)
            {
                return omnetpp::cQueue(name);
            }),
        pybind11::arg("name") = nullptr
    );

    py_cQueue.def("setName", &omnetpp::cQueue::setName);
    py_cQueue.def("getLength", &omnetpp::cQueue::getLength);
    py_cQueue.def("isEmpty", &omnetpp::cQueue::isEmpty);
    py_cQueue.def("pop", &omnetpp::cQueue::pop);
    py_cQueue.def("insert", &omnetpp::cQueue::insert);
}
