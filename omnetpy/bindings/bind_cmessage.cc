#include <pybind11/pybind11.h>

#include <omnetpp.h>
#include <omnetpp/cmessage.h>
#include <omnetpp/checkandcast.h>


void bind_cMessage(pybind11::module &m)
{
    pybind11::enum_<omnetpp::eMessageKind>(
            m,
            "_eMessageKind",
            R"docstring(
            Predefined message kind values (values for cMessage's getKind(),
            setKind() methods).

            Negative values are reserved for the \opp system and its
            standard libraries. Zero and positive values can be freely used
            by simulation models.
            )docstring"
        )
        .value("MK_STARTER", omnetpp::eMessageKind::MK_STARTER)
        .value("MK_TIMEOUT", omnetpp::eMessageKind::MK_TIMEOUT)
        .value("MK_PARSIM_BEGIN", omnetpp::eMessageKind::MK_PARSIM_BEGIN)
        .export_values();

    pybind11::class_<omnetpp::cMessage, omnetpp::cEvent, omnetpp::cObject> py_cMessage(
        m,
        "_cMessage",
        R"docstring(
        The message class in \opp.

        cMessage objects may represent events, messages, jobs or other entities
        in a simulation. To represent network packets, use the cPacket subclass.

        Messages may be scheduled (to arrive back at the same module at a later
        time), cancelled, sent out on a gate, or sent directly to another module;
        all via methods of cSimpleModule.

        Message can be assigned a name (a property inherited from cNamedObject);
        other attributes include message kind, priority, and time stamp.
        Messages may be cloned with the dup() function. The control info field
        facilitates modelling communication between protocol layers. The context
        pointer field makes it easier to work with several timers (self-messages)
        at a time. A message also stores information about its last sending,
        including sending time, arrival time, arrival module and gate.

        Useful methods are isSelfMessage(), which tells apart self-messages from
        messages received from other modules, and isScheduled(), which returns
        whether a self-message is currently scheduled.
        )docstring"
    );
    py_cMessage.def(
        pybind11::init<const char*, short>(),
        pybind11::arg("name") = nullptr, pybind11::arg("kind") = 0);

    py_cMessage.def_property(
        "name",
        &omnetpp::cMessage::getName,
        &omnetpp::cMessage::setName
    );

    py_cMessage.def_property(
        "kind",
        &omnetpp::cMessage::getKind,
        &omnetpp::cMessage::setKind
    );

    py_cMessage.def(
        "__repr__",
        [] (const omnetpp::cMessage & self)
        {
            std::stringstream ss;
            ss << "cMessage(name=" << self.getName() << ", kind=" << self.getKind() << ")";
            return ss.str();
        }
    );

    py_cMessage.def(
        "__str__",
        &omnetpp::cMessage::str,
        R"docstring(
        Produces a one-line description of the object's contents.
        See cObject for more details.
        )docstring"
    );

    py_cMessage.def(
        "isPacket",
        &omnetpp::cMessage::isPacket,
        R"docstring(
        Returns whether the current class is subclass of cPacket.

        The cMessage implementation returns `False`.
        )docstring"
    );

    py_cMessage.def(
        "dup",
        &omnetpp::cMessage::dup,
        R"docstring(
        Creates and returns an exact copy of this object, except for the
        message ID (the clone is assigned a new ID). Note that the message
        creation time is also copied, so clones of the same message object
        have the same creation time. See cObject for more details.
        )docstring"
    );

    py_cMessage.def(
        "str",
        &omnetpp::cMessage::str,
        R"docstring(
        Produces a one-line description of the object's contents.
        See cObject for more details.
        )docstring"
    );

    py_cMessage.def(
        "forEachChild",
        &omnetpp::cMessage::forEachChild,
        R"docstring(
        Calls v->visit(this) for each contained object.
        See cObject for more details.
        )docstring"
    );

    py_cMessage.def(
        "parsimPack",
        &omnetpp::cMessage::parsimPack,
        R"docstring(
        Serializes the object into an MPI send buffer
        Used by the simulation kernel for parallel execution.
        See cObject for more details.
        )docstring"
    );

    py_cMessage.def(
        "parsimUnpack",
        &omnetpp::cMessage::parsimUnpack,
        R"docstring(
        Deserializes the object from an MPI receive buffer
        Used by the simulation kernel for parallel execution.
        See cObject for more details.
        )docstring"
    );

    py_cMessage.def(
        "setKind",
        &omnetpp::cMessage::setKind,
        R"docstring(
        Sets the message kind. Nonnegative values can be freely used by
        the user; negative values are reserved by OMNeT++ for internal
        purposes.
        )docstring"
    );

    py_cMessage.def(
        "setTimestamp",
        pybind11::overload_cast<>(&omnetpp::cMessage::setTimestamp),
        R"docstring(
        Sets the message's time stamp to the current simulation time.
        )docstring"
    );

    py_cMessage.def(
        "setTimestamp",
        pybind11::overload_cast<omnetpp::simtime_t>(&omnetpp::cMessage::setTimestamp),
        R"docstring(
        Sets the message's time stamp to the current simulation time.
        )docstring"
    );

    py_cMessage.def(
        "setControlInfo",
        &omnetpp::cMessage::setControlInfo,
        R"docstring(
        Attaches a "control info" structure (object) to the message.
        This is most useful when passing packets between protocol layers
        of a protocol stack: e.g. when sending down an IP datagram to Ethernet,
        the attached "control info" can contain the destination MAC address.

        The "control info" object will be deleted when the message is deleted.
        Only one "control info" structure can be attached (the second
        setControlInfo() call throws an error).

        When the message is duplicated or copied, copies will have their
        control info set to `None` because the cObject interface
        does not define dup/copy operations.
        The assignment operator does not change control info.
        )docstring"
    );

    py_cMessage.def(
        "removeControlInfo",
        &omnetpp::cMessage::removeControlInfo,
        R"docstring(
        Removes the "control info" structure (object) from the message
        and returns its pointer. Returns `None` if there was no control info
        in the message.
        )docstring"
    );

    py_cMessage.def(
        "getKind",
        &omnetpp::cMessage::getKind,
        R"docstring(
        Returns the message kind.
        )docstring"
    );

    py_cMessage.def(
        "getTimestamp",
        &omnetpp::cMessage::getTimestamp,
        R"docstring(
        Returns the message time stamp.
        )docstring"
    );

    py_cMessage.def(
        "isSelfMessage",
        &omnetpp::cMessage::isSelfMessage,
        R"docstring(
        Return `True` if message was posted by scheduleAt().
        )docstring"
    );

    py_cMessage.def(
        "getSenderModule",
        &omnetpp::cMessage::getSenderModule,
        R"docstring(
        Returns a reference to the sender module. It returns `None` if the message
        has not been sent/scheduled yet, or if the sender module got deleted
        in the meantime.
        )docstring"
    );

    py_cMessage.def(
        "getSenderGate",
        &omnetpp::cMessage::getSenderGate,
        R"docstring(
        Returns reference to the gate from which the message was sent and
        on which gate it arrived. `None` is returned for new (unsent) messages
        and messages sent via scheduleAt().
        )docstring"
    );

    py_cMessage.def(
        "getArrivalModule",
        &omnetpp::cMessage::getArrivalModule,
        R"docstring(
        Returns a reference to the arrival module. It returns `None` if the message
        has not been sent/scheduled yet, or if the module was deleted
        in the meantime.
        )docstring"
    );

    py_cMessage.def(
        "getArrivalGate",
        &omnetpp::cMessage::getArrivalGate,
        R"docstring(
        Returns reference to the gate from which the message was sent and
        on which gate it arrived. `None` is returned for new (unsent) messages
        and messages sent via scheduleAt().
        )docstring"
    );

    py_cMessage.def(
        "getSenderModuleId",
        &omnetpp::cMessage::getSenderModuleId,
        R"docstring(
        Returns the module ID of the sender module, or -1 if the
        message has not been sent/scheduled yet.
        )docstring"
    );

    py_cMessage.def(
        "getSenderGateId",
        &omnetpp::cMessage::getSenderGateId,
        R"docstring(
        Returns the gate ID of the gate in the sender module on which the
        message was sent, or -1 if the message has not been sent/scheduled yet.
        Note: this is not the same as the gate's index (cGate::getIndex()).
        )docstring"
    );

    py_cMessage.def(
        "getArrivalModuleId",
        &omnetpp::cMessage::getArrivalModuleId,
        R"docstring(
        Returns the module ID of the receiver module, or -1 if the
        message has not been sent/scheduled yet.
        )docstring"
    );

    py_cMessage.def(
        "getArrivalGateId",
        &omnetpp::cMessage::getArrivalGateId,
        R"docstring(
        Returns the gate ID of the gate in the receiver module on which the
        message was received, or -1 if the message has not been sent/scheduled yet.
        Note: this is not the same as the gate's index (cGate::getIndex()).
        )docstring"
    );

    py_cMessage.def(
        "getCreationTime",
        &omnetpp::cMessage::getCreationTime,
        R"docstring(
        Returns time when the message was created; for cloned messages, it
        returns the creation time of the original message, not the time of the
        dup() call.
        )docstring"
    );

    py_cMessage.def(
        "getSendingTime",
        &omnetpp::cMessage::getSendingTime,
        R"docstring(
        Returns time when the message was sent/scheduled or 0 if the message
        has not been sent yet.
        )docstring"
    );

    py_cMessage.def(
        "getArrivalTime",
        &omnetpp::cMessage::getArrivalTime,
        R"docstring(
        Returns time when the message arrived (or will arrive if it
        is currently scheduled or is underway), or 0 if the message
        has not been sent/scheduled yet.

        When the message has nonzero length and it travelled though a
        channel with nonzero data rate, arrival time may represent either
        the start or the end of the reception, as returned by the
        isReceptionStart() method. By default it is the end of the reception;
        it can be changed by calling setDeliverOnReceptionStart(true) on the
        gate at receiving end of the channel that has the nonzero data rate.
        )docstring"
    );

    py_cMessage.def(
        "arrivedOn",
        pybind11::overload_cast<int>(&omnetpp::cMessage::arrivedOn, pybind11::const_),
        R"docstring(
        Return `True` if the message arrived through the given gate.
        )docstring",
        pybind11::arg("gateId")
    );

    py_cMessage.def(
        "arrivedOn",
        pybind11::overload_cast<const char *>(&omnetpp::cMessage::arrivedOn, pybind11::const_),
        R"docstring(
        Return `True` if the message arrived on the gate given with its name.
        If it is a vector gate, the method returns `True` if the message arrived
        on any gate in the vector.
        )docstring",
        pybind11::arg("gateName")
    );

    py_cMessage.def(
        "getId",
        &omnetpp::cMessage::getId,
        R"docstring(
        Returns a unique message identifier assigned upon message creation.
        )docstring"
    );

    py_cMessage.def(
        "getTreeId",
        &omnetpp::cMessage::getTreeId,
        R"docstring(
        Returns an identifier which is shared among a message object and all messages
        created by copying it (i.e. by dup() or the copy constructor).
        )docstring"
    );

    py_cMessage.def(
        "getDisplayString",
        &omnetpp::cMessage::getDisplayString,
        R"docstring(
        Override to define a display string for the message. Display string
        affects message appearance in graphical environments. The default implementation
        returns "".
        )docstring"
    );

    py_cMessage.def(
        "setArrival",
        pybind11::overload_cast<int, int>(&omnetpp::cMessage::setArrival),
        R"docstring(
        For use by custom scheduler classes (see cScheduler): set the arrival
        module and gate for messages inserted into the FES directly by the
        scheduler. If you pass gateId=-1, the message will arrive as a
        self-message.
        )docstring",
        pybind11::arg("moduleId"), pybind11::arg("gateId")
    );

    py_cMessage.def(
        "setArrival",
        pybind11::overload_cast<int, int, omnetpp::simtime_t_cref>(&omnetpp::cMessage::setArrival),
        R"docstring(
        Like setArrival(int moduleId, int gateId), but also sets the arrival
        time for the message.
        )docstring",
        pybind11::arg("moduleId"), pybind11::arg("gateId"), pybind11::arg("t")
    );

    py_cMessage.def(
        "getTotalMessageCount",
        &omnetpp::cMessage::getTotalMessageCount,
        R"docstring(
        Returns the total number of messages created since the last
        reset (reset is usually called by user interfaces at the beginning
        of each simulation run). The counter is incremented by cMessage constructor.
        Counter is signed to make it easier to detect if it overflows
        during very long simulation runs.
        May be useful for profiling or debugging memory leaks.
        )docstring"
    );

    py_cMessage.def_static(
        "getLiveMessageCount",
        &omnetpp::cMessage::getLiveMessageCount,
        R"docstring(
        Returns the number of message objects that currently exist in the
        program. The counter is incremented by cMessage constructor
        and decremented by the destructor.
        May be useful for profiling or debugging memory leaks caused by forgetting
        to delete messages.
        )docstring"
    );

    py_cMessage.def(
        "resetMessageCounters",
        &omnetpp::cMessage::resetMessageCounters,
        R"docstring(
        Reset counters used by getTotalMessageCount() and getLiveMessageCount().
        )docstring"
    );

    py_cMessage.def(
        "setSchedulingPriority",
        &omnetpp::cMessage::setSchedulingPriority,
        R"docstring(
        Sets the scheduling priority of this event. Scheduling priority is
        used when the simulator inserts messages into the future events set
        (FES), to order events with identical arrival time values.
        )docstring",
        pybind11::arg("p")
    );

    // TODO: move this to cObject
    py_cMessage.def("isScheduled", &omnetpp::cMessage::isScheduled);

    py_cMessage.def(
        "as_cPacket",
        [](omnetpp::cMessage &msg)
        {
            return omnetpp::check_and_cast<omnetpp::cPacket *>(&msg);
        }
    );

}
