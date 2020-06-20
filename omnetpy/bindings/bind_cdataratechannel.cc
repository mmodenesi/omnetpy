#include <pybind11/pybind11.h>

#include <omnetpp.h>


void bind_cDatarateChannel(pybind11::module &m)
{
    pybind11::class_<
        omnetpp::cDatarateChannel,
        omnetpp::cChannel
        > py_cDatarateChannel(
        m,
        "_cDatarateChannel",
        R"docstring(
        Channel that models delay, bit error rate and data rate.
        )docstring"
    );


    py_cDatarateChannel.def_static(
        "create",
        &omnetpp::cDatarateChannel::create,
        R"docstring(
        Utility function for dynamic channel creation. Equivalent to
        ``cChannelType.getDatarateChannelType().create(name)``.
        )docstring",
        pybind11::arg("name")
    );

    py_cDatarateChannel.def(
        "str",
        &omnetpp::cDatarateChannel::str,
        R"docstring(
        Produces a one-line description of the object's contents.
        See cObject for more details.
        )docstring"
    );

    py_cDatarateChannel.def(
        "__str__",
        &omnetpp::cDatarateChannel::str,
        R"docstring(
        Produces a one-line description of the object's contents.
        See cObject for more details.
        )docstring"
    );

    py_cDatarateChannel.def(
        "isTransmissionChannel",
        &omnetpp::cDatarateChannel::isTransmissionChannel,
        R"docstring(
        Returns true.
        )docstring"
    );

    py_cDatarateChannel.def(
        "setDelay",
        &omnetpp::cDatarateChannel::setDelay,
        R"docstring(
        Sets the propagation delay of the channel, in seconds.
        )docstring",
        pybind11::arg("d")
    );

    py_cDatarateChannel.def(
        "setDatarate",
        &omnetpp::cDatarateChannel::setDatarate,
        R"docstring(
        Sets the data rate of the channel, in bit/second.
        )docstring",
        pybind11::arg("d")
    );

    py_cDatarateChannel.def(
        "setDatarate",
        &omnetpp::cDatarateChannel::setDatarate,
        R"docstring(
        Sets the bit error rate (BER) of the channel.
        )docstring",
        pybind11::arg("d")
    );

    py_cDatarateChannel.def(
        "setPacketErrorRate",
        &omnetpp::cDatarateChannel::setPacketErrorRate,
        R"docstring(
        Sets the packet error rate (PER) of the channel.
        )docstring",
        pybind11::arg("d")
    );

    py_cDatarateChannel.def(
        "setDisabled",
        &omnetpp::cDatarateChannel::setDisabled,
        R"docstring(
        Disables or enables the channel.
        )docstring",
        pybind11::arg("d")
    );

    py_cDatarateChannel.def(
        "getDelay",
        &omnetpp::cDatarateChannel::getDelay,
        R"docstring(
        Returns the propagation delay of the channel, in seconds.
        This method is equivalent to reading the "delay" parameter, via par("delay").
        )docstring"
    );

    py_cDatarateChannel.def(
        "getDatarate",
        &omnetpp::cDatarateChannel::getDatarate,
        R"docstring(
        Returns the data rate of the channel, in bit/second.
        This method is equivalent to reading the "datarate" parameter, via par("datarate").
        This value affects the transmission time of messages sent through
        the channel.
        )docstring"
    );

    py_cDatarateChannel.def(
        "getBitErrorRate",
        &omnetpp::cDatarateChannel::getBitErrorRate,
        R"docstring(
        Returns the bit error rate (BER) of the channel.
        This method is equivalent to reading the "ber" parameter, via par("ber").
        When a message sent through the channel suffers at least
        one bit error, its bit error flag will be set.
        )docstring"
    );

    py_cDatarateChannel.def(
        "getPacketErrorRate",
        &omnetpp::cDatarateChannel::getPacketErrorRate,
        R"docstring(
        Returns the packet error rate (PER) of the channel.
        This method is equivalent to reading the "per" parameter, via par("per").
        When a message is sent through the channel, its bit error flag
        will be set with this probability.
        )docstring"
    );

    py_cDatarateChannel.def(
        "isDisabled",
        &omnetpp::cDatarateChannel::isDisabled,
        R"docstring(
        Returns whether the channel is disabled.
        This method is equivalent to reading the "disabled" parameter, via par("disabled").
        A disabled channel discards all messages sent on it.
        )docstring"
    );

    py_cDatarateChannel.def(
        "getNominalDatarate",
        &omnetpp::cDatarateChannel::getNominalDatarate,
        R"docstring(
        Same as getDatarate().
        )docstring"
    );

    py_cDatarateChannel.def(
        "calculateDuration",
        &omnetpp::cDatarateChannel::calculateDuration,
        R"docstring(
        Returns the message length in bits divided by the datarate.

        Note that processMessage() does NOT call this method, so in order to
        change the duration computation algorithm via subclassing you need
        to redefine both this and processMessage().
        )docstring"
    );

    py_cDatarateChannel.def(
        "getTransmissionFinishTime",
        &omnetpp::cDatarateChannel::getTransmissionFinishTime,
        R"docstring(
        Returns the simulation time the sender gate will finish transmitting.
        If the gate is not currently transmitting, the result is undefined but
        less or equal the current simulation time.

        The transmission duration of a message depends on the message length
        and the data rate assigned to the channel.
        )docstring"
    );

    py_cDatarateChannel.def(
        "isBusy",
        &omnetpp::cDatarateChannel::isBusy,
        R"docstring(
        Returns whether the sender gate is currently transmitting, ie. whether
        transmissionFinishTime() is greater than the current simulation time.
        )docstring"
    );

    py_cDatarateChannel.def(
        "forceTransmissionFinishTime",
        &omnetpp::cDatarateChannel::forceTransmissionFinishTime,
        R"docstring(
        Forcibly overwrites the finish time of the current transmission in the
        channel (see getTransmissionFinishTime()). This method can be used for
        implementing transmission abortion; see declaration in base class
        (cChannel::forceTransmissionFinishTime()) for more information.
        )docstring"
    );

    py_cDatarateChannel.def(
        "initialize",
        &omnetpp::cDatarateChannel::initialize,
        R"docstring(
        Initialization.
        )docstring"
    );

    /*
    py_cDatarateChannel.def(
        "processMessage",
        &omnetpp::cDatarateChannel::processMessage,
        R"docstring(
        Performs bit error rate, delay and transmission time modelling.
        )docstring",
        pybind11::arg("msg"), pybind11::arg("t"), pybind11::arg("result")
    );
    */
}
