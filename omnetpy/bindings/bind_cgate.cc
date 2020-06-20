#include <pybind11/pybind11.h>

#include <omnetpp.h>
#include <omnetpp/cgate.h>


void bind_cGate(pybind11::module &m)
{
    // private destructor
    pybind11::class_<
        omnetpp::cGate,
        std::unique_ptr<omnetpp::cGate, pybind11::nodelete>> py_cGate(
        m,
        "cGate",
        R"docstring(
        Represents a module gate.

        cGate object are created and managed by modules; the user typically
        does not want to directly create or destroy cGate objects. However,
        they are important if a simple module algorithm needs to know about
        its surroundings.
        )docstring"
    );

   py_cGate.def(
        "setDeliverOnReceptionStart",
        &omnetpp::cGate::setDeliverOnReceptionStart,
        R"docstring(
        This method may only be invoked on input gates of simple modules.
        Messages with nonzero length then have a nonzero
        transmission duration (and thus, reception duration on the other
        side of the connection). By default, the delivery of the message
        to the module marks the end of the reception. Setting this bit will cause
        the channel to deliver the message to the module at the start of the
        reception. The duration that the reception will take can be extracted
        from the message object, by its getDuration() method.
        )docstring",
        pybind11::arg("d")
    );

   py_cGate.def(
        "getNextGate",
        &omnetpp::cGate::getNextGate,
        R"docstring(
        Returns the next gate in the series of connections (the path) that
        contains this gate, or nullptr if this gate is the last one in the path.
        (E.g. for a simple module input gate, this function will return nullptr.)
        )docstring",
        pybind11::return_value_policy::reference
    );

   py_cGate.def(
        "getIndex",
        &omnetpp::cGate::getIndex,
        R"docstring(
        If the gate is part of a gate vector, returns the gate's index in the vector.
        Otherwise, it returns 0.
        )docstring"
    );

   py_cGate.def(
        "getVectorSize",
        &omnetpp::cGate::getVectorSize,
        R"docstring(
        If the gate is part of a gate vector, returns the size of the vector.
        For non-vector gates it returns 1.

        The gate vector size can also be obtained by calling the cModule::gateSize().
        )docstring"
    );

   py_cGate.def(
        "size",
        &omnetpp::cGate::size,
        R"docstring(
        Alias for getVectorSize().
        )docstring"
    );

   py_cGate.def(
        "getTransmissionChannel",
        &omnetpp::cGate::getTransmissionChannel,
        R"docstring(
        Typically invoked on an output gate, this method returns <i>the</i>
        channel in the connection path that supports datarate (as determined
        by cChannel::isTransmissionChannel(); it is guaranteed that there can be
        at most one such channel per path). If there is no such channel,
        an error is thrown.

        This method only checks the segment of the connection path that
        <i>starts</i> at this gate, so, for example, it is an error to invoke
        it on a simple module input gate.

        Note: this method searches the connection path linearly, so at
        performance-critical places it may be better to cache its return
        value (provided that connections are not removed or created dynamically
        during simulation.)
        )docstring",
        pybind11::return_value_policy::reference
    );

   py_cGate.def(
        "getId",
        &omnetpp::cGate::getId,
        R"docstring(
        Returns the gate ID, which uniquely identifies the gate within the
        module. IDs are guaranteed to be contiguous within a gate vector:
        <tt>module->gate(id+index) == module->gate(id)+index</tt>.

        Gate IDs are stable: they are guaranteed not to change during
        simulation. (This is a new feature of \opp 4.0. In earlier releases,
        gate IDs could change when the containing gate vector was resized.)

        Note: As of \opp 4.0, gate IDs are no longer small integers, and
        cannot be used for iterating over the gates of a module.
        Use cModule::GateIterator for iteration.
        )docstring"
    );

   py_cGate.def(
        "connectTo",
        &omnetpp::cGate::connectTo,
        R"docstring(
        Connects the gate to another gate, using the given channel object
        (if one is specified). This method can be used to manually create
        connections for dynamically created modules.

        This method invokes callInitialize() on the channel object, unless the
        compound module containing this connection is not yet initialized
        (then it assumes that this channel will be initialized as part of the
        compound module initialization process.) To leave the channel
        uninitialized, specify true for the leaveUninitialized parameter.

        If the gate is already connected, an error will occur. The gate
        argument cannot be nullptr, that is, you cannot use this function
        to disconnect a gate; use disconnect() for that.

        Note: When you set channel parameters after channel initialization,
        make sure the channel class is implemented so that the changes take
        effect; i.e. the channel should either override and properly handle
        handleParameterChange(), or should not cache any values from parameters.
        )docstring",
        pybind11::return_value_policy::reference,
        pybind11::arg("gate"), pybind11::arg("channel") = nullptr, pybind11::arg("leaveUninitialized") = false
    );

   py_cGate.def(
        "isConnected",
        &omnetpp::cGate::isConnected,
        R"docstring(
        Returns true if the gate fully connected. For a compound module gate
        this means both isConnectedInside() and isConnectedOutside() are true;
        for a simple module, only isConnectedOutside() is checked.
        )docstring"
    );
}
