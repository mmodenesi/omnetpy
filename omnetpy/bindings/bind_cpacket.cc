#include <pybind11/pybind11.h>

#include <omnetpp.h>
#include <omnetpp/cpacket.h>


void bind_cPacket(pybind11::module &m)
{
    pybind11::class_<omnetpp::cPacket, omnetpp::cMessage> py_cPacket(
        m,
        "_cPacket"
    );

    py_cPacket.def(
        pybind11::init<const char *, short, int64_t>(),
        pybind11::arg("name") = nullptr, pybind11::arg("kind") = 0, pybind11::arg("bitLength") = 0
    );

    py_cPacket.def(
        "setBitLength",
        &omnetpp::cPacket::setBitLength,
        pybind11::arg("l")
    );

    py_cPacket.def(
        "getDuration",
        &omnetpp::cPacket::getDuration
    );

    py_cPacket.def(
        "getBitLength",
        &omnetpp::cPacket::getBitLength
    );

    py_cPacket.def(
        "setByteLength",
        &omnetpp::cPacket::setByteLength,
        pybind11::arg("l")
    );

    py_cPacket.def(
        "getByteLength",
        &omnetpp::cPacket::getByteLength
    );
}
