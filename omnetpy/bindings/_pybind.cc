/*
 * Python bindings for OMNeT++ classes
 *
 */

#include <omnetpp.h>
#include <pybind11/pybind11.h>


void bind_EV(pybind11::module &);
void bind_WATCH(pybind11::module &);
void bind_cAbstractHistogram(pybind11::module &);
void bind_cCanvas(pybind11::module &);
void bind_cChannel(pybind11::module &);
void bind_cComponent(pybind11::module &);
void bind_cComponentType(pybind11::module &);
void bind_cDatarateChannel(pybind11::module &);
void bind_cDisplayString(pybind11::module &);
void bind_cEvent(pybind11::module &);
void bind_cGate(pybind11::module &);
void bind_cHistogram(pybind11::module &);
void bind_cHistogramStrategy(pybind11::module &);
void bind_cKSplit(pybind11::module &);
void bind_cMessage(pybind11::module &);
void bind_cModule(pybind11::module &);
void bind_cNamedObject(pybind11::module &);
void bind_cObject(pybind11::module &);
void bind_cOutVector(pybind11::module &);
void bind_cOwnedObject(pybind11::module &);
void bind_cPSquare(pybind11::module &);
void bind_cPacket(pybind11::module &);
void bind_cPar(pybind11::module &);
void bind_cQueue(pybind11::module &);
void bind_cSimTime(pybind11::module &);
void bind_cSimpleModule(pybind11::module &);
void bind_cSimulation(pybind11::module &);
void bind_cStatistic(pybind11::module &);
void bind_cStdDev(pybind11::module &);
void bind_cTimestampedValue(pybind11::module &);
void bind_cTopology(pybind11::module &);


PYBIND11_MODULE(_pybind, m) {

    m.doc() = "";  // this module will not be visible by the end user

    // the order matters
    bind_cObject(m);
    bind_cNamedObject(m);
    bind_cOwnedObject(m);
    bind_cComponent(m);
    bind_cGate(m);
    bind_EV(m);
    bind_WATCH(m);
    bind_cDisplayString(m);
    bind_cEvent(m);
    bind_cCanvas(m);

    bind_cChannel(m);
    bind_cDatarateChannel(m);

    bind_cStatistic(m);
    bind_cStdDev(m);
    bind_cAbstractHistogram(m);
    bind_cHistogram(m);
    bind_cHistogramStrategy(m);
    bind_cOutVector(m);

    bind_cMessage(m);
    bind_cPacket(m);

    bind_cPar(m);
    bind_cQueue(m);
    bind_cSimTime(m);
    bind_cModule(m);
    bind_cSimpleModule(m);
    bind_cKSplit(m);
    bind_cPSquare(m);
    bind_cTimestampedValue(m);
    bind_cComponentType(m);
    bind_cSimulation(m);

    bind_cTopology(m);

    pybind11::register_exception<omnetpp::cRuntimeError>(m, "_cRuntimeError");
    pybind11::register_exception<omnetpp::cDeleteModuleException>(m, "_cDeleteModuleException");
}
