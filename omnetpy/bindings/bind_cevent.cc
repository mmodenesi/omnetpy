#include <pybind11/pybind11.h>

#include <omnetpp.h>
#include <omnetpp/cevent.h>


void bind_cEvent(pybind11::module &m)
{
    pybind11::class_<omnetpp::cEvent> py_cEvent(
        m,
        "_cEvent",
        R"docstring(
        Represents an event in the discrete event simulator.

        When events are scheduled, they are inserted into the future events set (FES)
        where they are (conceptually) stored in timestamp (="arrival time") order.
        Events are removed from the FES one by one, and processed by invoking their
        execute() methods. execute() should be overridden in subclasses to carry out
        the actions associated with the event.

        If several events have identical timestamp values (arrival times), further fields
        decide their ordering: first, scheduling priority and then, insertion order.

        Pure event objects (cEvent) are normally of little interest to the user. Instead,
        one should use messages and packets (cMessage, cPacket). They are subclassed from
        cEvent, and their execute() methods automatically delivers the message/packet
        to the target module.
        )docstring"
    );

    py_cEvent.def(
        "isScheduled",
        &omnetpp::cEvent::isScheduled,
        R"docstring(
        Returns true if this event is in the future events set (FES).
        )docstring"
    );

    py_cEvent.def(
        "getArrivalTime",
        &omnetpp::cEvent::getArrivalTime,
        R"docstring(
        Returns the simulation time this event object has been last scheduled for
        (regardless whether it is currently scheduled), or zero if the event
        hasn't been scheduled yet.
        )docstring"
    );

    py_cEvent.def(
        "isMessage",
        &omnetpp::cEvent::isMessage,
        R"docstring(
        A fast way (that is, faster than dynamic_cast) to determine whether this
        event is a cMessage.
        )docstring"
    );

    py_cEvent.def(
        "isStale",
        &omnetpp::cEvent::isStale,
        R"docstring(
        Returns `True` if this event is stale. An event might go stale while
        staying in the future events set (FES), for example due to its target
        object being deleted. Stale events are discarded by the scheduler.
        )docstring"
    );

    py_cEvent.def(
        "setSchedulingPriority",
        &omnetpp::cEvent::setSchedulingPriority,
        R"docstring(
        Sets the scheduling priority of this event. Scheduling priority is
        used when the simulator inserts messages into the future events set
        (FES), to order events with identical arrival time values.
        )docstring",
        pybind11::arg("p")
    );

}
