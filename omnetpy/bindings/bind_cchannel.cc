#include <pybind11/pybind11.h>

#include <omnetpp.h>


void bind_cChannel(pybind11::module &m)
{
    pybind11::class_<
        omnetpp::cChannel,
        omnetpp::cComponent,
        omnetpp::cOwnedObject,
        omnetpp::cNamedObject,
        omnetpp::cObject
        > py_cChannel(
        m,
        "_cChannel",
        R"docstring(
        Base class for channels.
        )docstring"
    );

    pybind11::class_<omnetpp::cChannel::result_t> py_cChannel_result_t(
        py_cChannel,
        R"docstring(
        Allows returning multiple values from the processMessage() method.

        The constructor initializes all fields to zero.
        )docstring"
    );

    py_cChannel_result_t.def(pybind11::init<>());

    py_cChannel_result_t.def_readwrite("delay", &omnetpp::cChannel::result_t::delay);
    py_cChannel_result_t.def_readwrite("duration", &omnetpp::cChannel::result_t::duration);
    py_cChannel_result_t.def_readwrite("discard", &omnetpp::cChannel::result_t::discard);

    py_cChannel.def(
        "str",
        &omnetpp::cChannel::str,
        R"docstring(
        Produces a one-line description of the object's contents.
        See cObject for more details.
        )docstring"
    );

    py_cChannel.def(
        "__str__",
        &omnetpp::cChannel::str,
        R"docstring(
        Produces a one-line description of the object's contents.
        See cObject for more details.
        )docstring"
    );

    py_cChannel.def(
        "callInitialize",
        pybind11::overload_cast<>(&omnetpp::cChannel::callInitialize),
        R"docstring(
        Interface for calling initialize() from outside. Implementation
        performs multi-stage initialization for this channel object.
        )docstring"
    );

    py_cChannel.def(
        "callInitialize",
        pybind11::overload_cast<int>(&omnetpp::cChannel::callInitialize),
        R"docstring(
        Interface for calling initialize() from outside. Implementation
        performs multi-stage initialization for this channel object.
        )docstring",
        pybind11::arg("stage")
    );

    py_cChannel.def(
        "callFinish",
        &omnetpp::cChannel::callFinish,
        R"docstring(
        Interface for calling finish() from outside.
        )docstring"
    );

    py_cChannel.def(
        "getComponentKind",
        &omnetpp::cChannel::getComponentKind,
        R"docstring(
        Redefined from cComponent to return KIND_CHANNEL.
        )docstring"
    );

    py_cChannel.def(
        "getParentModule",
        &omnetpp::cChannel::getParentModule,
        R"docstring(
        Returns the compound module containing this channel. That is,
        the channel is either between two submodules of getParentModule(),
        or between getParentModule() and one of its submodules.
        (For completeness, it may also connect two gates of getParentModule()
        on the inside).
        )docstring"
    );

    py_cChannel.def(
        "getChannelType",
        &omnetpp::cChannel::getChannelType,
        R"docstring(
        Convenience method: casts the return value of getComponentType() to cChannelType.
        )docstring"
    );

    py_cChannel.def(
        "getProperties",
        &omnetpp::cChannel::getProperties,
        R"docstring(
        Return the properties for this channel. Properties cannot be changed
        at runtime. Redefined from cComponent.
        )docstring"
    );

    py_cChannel.def(
        "getSourceGate",
        &omnetpp::cChannel::getSourceGate,
        R"docstring(
        Returns the gate this channel is attached to.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cChannel.def(
        "isTransmissionChannel",
        &omnetpp::cChannel::isTransmissionChannel,
        R"docstring(
        Returns true if the channel models a nonzero-duration transmission,
        that is, sets the duration field of cPacket. Only one transmission
        channel is allowed per connection path (see cGate methods getPreviousGate(),
        getNextGate(), getPathStartGate(), getPathEndGate()).
        )docstring"
    );
    //@}

    py_cChannel.def(
        "processMessage",
        &omnetpp::cChannel::processMessage,
        R"docstring(
        This method encapsulates the channel's functionality. The method should
        model the transmission of the given message starting at the given t time,
        and store the results (propagation delay, transmission duration,
        discard flag) in the result object. Only the relevant fields
        in the result object need to be changed, others can be left untouched.

        Transmission duration and bit error modeling only applies to packets
        (i.e. to instances of cPacket, where cMessage's isPacket() returns true),
        it should be skipped for non-packet messages. The method does not need
        to call the <tt>setDuration(duration)</tt> method on the packet; this is
        done by the simulation kernel. However, the method should call
        <tt>setBitError(true)</tt> on the packet if error modeling results
        in bit errors.

        If the method sets the discard flag in the result object, it means
        that the message object should be deleted by the simulation kernel;
        this facility can be used to model that the message gets lost in the
        channel.

        The method does not need to throw error on overlapping transmissions,
        or if the packet's duration field is already set; these checks are
        done by the simulation kernel before processMessage() is called.
        )docstring",
        pybind11::arg("msg"), pybind11::arg("t"), pybind11::arg("result")
    );

    py_cChannel.def(
        "getNominalDatarate",
        &omnetpp::cChannel::getNominalDatarate,
        R"docstring(
        For transmission channels: Returns the nominal data rate of the channel.
        The number returned from this method should be treated as informative;
        there is no strict requirement that the channel calculates packet
        duration by dividing the packet length by the nominal data rate.
        For example, specialized channels may add the length of a lead-in
        signal to the duration.
        )docstring"
    );

    py_cChannel.def(
        "calculateDuration",
        &omnetpp::cChannel::calculateDuration,
        R"docstring(
        For transmission channels: Calculates the transmission duration
        of the message with the current channel configuration (datarate, etc);
        it does not check or modify channel state. For non-transmission channels
        this method returns zero.

        This method is useful for transmitter modules that need to determine
        the transmission time of a packet without actually sending the packet.

        Caveats: this method is "best-effort" -- there is no guarantee that
        transmission time when the packet is actually sent will be the same as
        the value returned by this method. The difference may be caused by
        changed channel parameters (i.e. "datarate" being overwritten), or by
        a non-time-invariant transmission algorithm.

        Note that there is no requirement that processMessage() relies on this
        method to calculated the packet duration. That is, to change the
        duration computation algorithm via subclassing you need to redefine
        BOTH the processMessage() and calculateDuration() methods.
        )docstring",
        pybind11::arg("msg")
    );

    py_cChannel.def(
        "getTransmissionFinishTime",
        &omnetpp::cChannel::getTransmissionFinishTime,
        R"docstring(
        For transmission channels: Returns the simulation time
        the sender gate will finish transmitting. If the gate is not
        currently transmitting, the result is unspecified but less or equal
        the current simulation time.
        )docstring"
    );

    py_cChannel.def(
        "isBusy",
        &omnetpp::cChannel::isBusy,
        R"docstring(
        For transmission channels: Returns whether the sender gate
        is currently transmitting, ie. whether getTransmissionFinishTime()
        is greater than the current simulation time.
        )docstring"
    );

    py_cChannel.def(
        "forceTransmissionFinishTime",
        &omnetpp::cChannel::forceTransmissionFinishTime,
        R"docstring(
        For transmission channels: Forcibly overwrites the finish time of the
        current transmission in the channel (see getTransmissionFinishTime()).

        This method is a crude device that allows for implementing aborting
        transmissions; it is not needed for normal packet transmissions.
        Calling this method with the current simulation time will allow
        you to immediately send another packet on the channel without the
        channel reporting error due to its being busy.

        Note that this call does NOT affect the delivery of the packet being
        transmitted: the packet object is delivered to the target module
        at the time it would without the call to this method. The sender
        needs to inform the target module in some other way that the
        transmission was aborted and the packet should be treated accordingly
        (i.e. discarded as incomplete); for example by sending an out-of-band
        cMessage that the receiver has to understand.
        )docstring",
        pybind11::arg("t")
    );
}
