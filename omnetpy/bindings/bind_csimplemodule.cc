#include <pybind11/pybind11.h>

#include <omnetpp.h>
#include <omnetpy.h>
#include <omnetpp/csimplemodule.h>


void bind_cSimpleModule(pybind11::module &m)
{
    pybind11::class_<
            omnetpp::cSimpleModule,
            PycSimpleModule,
            omnetpp::cModule,
            omnetpp::cComponent,
            omnetpp::cNamedObject,
            omnetpp::cObject,
            std::unique_ptr<omnetpp::cSimpleModule, pybind11::nodelete>> py_cSimpleModule(
        m,
        "_cSimpleModule",
        R"docstring(
        Base class for all simple module classes.

        cSimpleModule, although packed with simulation-related functionality,
        does not do anything useful by itself: one has to subclass from it
        and redefine one or more abstract member functions to make it do useful work.
        These functions are:

           - initialize()
           - handleMessage(msg)
           - activity()
           - finish()

        initialize() is called after \opp created the module. Multi-stage
        initialization can be achieved by redefining the initialize(int stage)
        method instead, and also redefining the numInitStages() method to
        return the required number of stages.

        One has to redefine handleMessage() to contain the internal logic of
        the module. handleMessage() is called by the simulation kernel when the
        module receives a message. (An alternative to handleMessage() is
        activity(), but activity() is not recommended for serious model development
        because of scalability and debugging issues. activity() also tends to lead
        to messy module implementations.)

        You can send() messages to other modules, or use scheduleAt()+cancelEvent()
        to implement delays, timers or timeouts. Messages sent or scheduled (but
        not cancelled) are delivered to modules via handleMessage(), or, when using
        activity(), via receive().

        The finish() functions are called when the simulation terminates
        successfully. Typical use of finish() is recording statistics collected
        during simulation.
        )docstring"
    );

    py_cSimpleModule.def(
        pybind11::init<unsigned>(),
        R"docstring(
        Constructor. Note that module objects should not be created directly,
        only via their cModuleType objects. See cModule constructor for more info.
        The stacksize parameter should only be used with simple modules using activity().
        )docstring",
        pybind11::arg("stacksize") = 0);

    py_cSimpleModule.def(
        "__str__",
        &omnetpp::cSimpleModule::str,
        R"docstring(
        Produces a one-line description of the object's contents.
        See cObject for more details.
        )docstring"
    );

    py_cSimpleModule.def(
        "activity",
        &cSimpleModulePublicist::activity,
        R"docstring(
        Should be redefined to contain the module activity function.
        For several good reasons, you should prefer handleMessage() to activity().
        This default implementation issues an error message (throws cRuntimeError).
        )docstring"
    );

    py_cSimpleModule.def(
        "handleMessage",
        &cSimpleModulePublicist::handleMessage,
        R"docstring(
        Should be redefined to contain the module's message handling function.
        The default implementation issues an error message (throws cRuntimeError).
        )docstring"
    );

    py_cSimpleModule.def(
        "send",
        pybind11::overload_cast<omnetpp::cMessage*, int>(&cSimpleModulePublicist::send),
        R"docstring(
        Sends a message through the gate given with its ID.
        )docstring",
        pybind11::arg("msg"), pybind11::arg("gateindex") = -1
    );

    py_cSimpleModule.def(
        "send",
        pybind11::overload_cast<omnetpp::cMessage*, const char*, int>(&cSimpleModulePublicist::send),
        R"docstring(
        Sends a message through the gate given with its name and index
        (if multiple gate).
        )docstring",
        pybind11::arg("msg"), pybind11::arg("gatename"), pybind11::arg("gateindex") = -1
    );

    py_cSimpleModule.def(
        "sendDelayed",
        pybind11::overload_cast<omnetpp::cMessage *, omnetpp::simtime_t, int>(&cSimpleModulePublicist::sendDelayed),
        R"docstring(
        Delayed sending. Sends a message through the gate given with
        its index as if it was sent delay seconds later.
        )docstring",
        pybind11::arg("msg"), pybind11::arg("delay"), pybind11::arg("gateid")
    );

    py_cSimpleModule.def(
        "sendDelayed",
        pybind11::overload_cast<omnetpp::cMessage *, omnetpp::simtime_t, const char *, int>(&cSimpleModulePublicist::sendDelayed),
        R"docstring(
        Delayed sending. Sends a message through the gate given with
        its name and index (if multiple gate) as if it was sent delay
        seconds later.
        )docstring",
        pybind11::arg("msg"), pybind11::arg("delay"), pybind11::arg("gatename"), pybind11::arg("gateindex") = -1
    );

    py_cSimpleModule.def(
        "sendDirect",
        pybind11::overload_cast<omnetpp::cMessage *, omnetpp::cModule *, const char *, int>(&cSimpleModulePublicist::sendDirect),
        R"docstring(
        Sends a message directly to another module, with zero propagation delay
        and duration. See sendDirect(cMessage *, simtime_t, simtime_t, cGate *)
        for a more detailed description.
        )docstring",
        pybind11::arg("msg"), pybind11::arg("mod"), pybind11::arg("gatename"), pybind11::arg("gateindex") = -1
    );

    py_cSimpleModule.def(
        "sendDirect",
        pybind11::overload_cast<omnetpp::cMessage *, omnetpp::cModule *, int>(&cSimpleModulePublicist::sendDirect),
        R"docstring(
        Sends a message directly to another module, with zero propagation delay
        and duration. See sendDirect(cMessage *, simtime_t, simtime_t, cGate *)
        for a more detailed description.
        )docstring",
        pybind11::arg("msg"), pybind11::arg("mod"), pybind11::arg("gateid")
    );

    py_cSimpleModule.def(
        "sendDirect",
        pybind11::overload_cast<omnetpp::cMessage *, omnetpp::simtime_t, omnetpp::simtime_t, omnetpp::cModule *, int>(&cSimpleModulePublicist::sendDirect),
        R"docstring(
        Sends a message directly to another module.
        See sendDirect(cMessage *, simtime_t, simtime_t, cGate *) for a more
        detailed description.
        )docstring",
        pybind11::arg("msg"), pybind11::arg("propagationDelay"), pybind11::arg("duration"), pybind11::arg("mod"), pybind11::arg("gateid")
    );

    py_cSimpleModule.def(
        "sendDirect",
        [] (omnetpp::cSimpleModule &self, omnetpp::cMessage *msg, double propagationDelay, double duration, omnetpp::cGate * gate)
        {
            self.sendDirect(msg, omnetpp::SimTime(propagationDelay), omnetpp::SimTime(duration), gate);
        },
        pybind11::arg("msg"), pybind11::arg("propagationDelay"), pybind11::arg("duration"), pybind11::arg("inputGate")
    );

    py_cSimpleModule.def(
        "sendDirect",
        pybind11::overload_cast<omnetpp::cMessage *, omnetpp::simtime_t, omnetpp::simtime_t, omnetpp::cModule *, const char *, int>(&cSimpleModulePublicist::sendDirect),
        R"docstring(
        Sends a message directly to another module.
        See sendDirect(cMessage *, simtime_t, simtime_t, cGate *) for a more
        detailed description.
        )docstring",
        pybind11::arg("msg"), pybind11::arg("propagationDelay"), pybind11::arg("duration"), pybind11::arg("mod"), pybind11::arg("inputgatename"), pybind11::arg("gateindex") = -1
    );

    py_cSimpleModule.def(
        "sendDirect",
        pybind11::overload_cast<omnetpp::cMessage *, omnetpp::cGate *>(&cSimpleModulePublicist::sendDirect),
        R"docstring(
        Sends a message directly to another module, with zero propagation delay
        and duration. See sendDirect(cMessage *, simtime_t, simtime_t, cGate *)
        for a more detailed description.
        )docstring",
        pybind11::arg("msg"), pybind11::arg("inputGate")
    );

    py_cSimpleModule.def(
        "endSimulation",
        &omnetpp::cSimpleModule::endSimulation,
        R"docstring(
        Causes the whole simulation to stop. The implementation simply
        throws a cTerminationException.
        )docstring"
    );

    py_cSimpleModule.def(
        "scheduleAt",
        pybind11::overload_cast<omnetpp::simtime_t, omnetpp::cMessage*>(&omnetpp::cSimpleModule::scheduleAt),
        R"docstring(
        Schedules a self-message. It will be delivered back to the module
        via receive() or handleMessage() at simulation time t. This method
        is the way you can implement timers or timeouts. Timers can also
        be cancelled via cancelEvent() (See below.)

        When the message is delivered at the module, you can call
        `msg.isSelfMessage()` to tell it apart from messages arriving
        from other modules. `msg.getKind()` can be used to further
        classify it.

        self.cancelEvent() can be used to cancel the self-message before it arrives.
        This is useful for implementing timeouts: if the event occurs "in time"
        (before timeout), the scheduled self-message can be cancelled.

        Given a cMessage, you can check whether it is currently
        scheduled by calling `msg.isScheduled()`. If it is scheduled,
        you cannot schedule it again without calling self.cancelEvent() first.
        However, after the message was delivered to the module or cancelled,
        you can schedule it again -- so you can reuse the same message
        object for timeouts over and over during the whole simulation.
        )docstring",
        pybind11::arg("t"), pybind11::arg("msg")
    );

    py_cSimpleModule.def(
        "scheduleAt",
        [](omnetpp::cSimpleModule &self, double t, omnetpp::cMessage* msg) {
            self.scheduleAt(omnetpp::SimTime(t), msg);
        },
        pybind11::arg("t"), pybind11::arg("msg"));

    py_cSimpleModule.def(
        "cancelEvent",
        &omnetpp::cSimpleModule::cancelEvent, pybind11::arg("msg"),
        R"docstring(
        Removes the given message from the future events. The message
        needs to have been sent using the `self.scheduleAt()` function.
        This function can be used to cancel a timer implemented with `self.scheduleAt()`.
        If the message is not currently scheduled, nothing happens.
        )docstring"
    );

    py_cSimpleModule.def(
        "receive",
        pybind11::overload_cast<>(&omnetpp::cSimpleModule::receive),
        R"docstring(
        Remove the next message from the event queue and return a pointer
        to it.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cSimpleModule.def(
        "receive",
        pybind11::overload_cast<omnetpp::simtime_t>(&omnetpp::cSimpleModule::receive),
        R"docstring(
        Removes the next message from the event queue and returns a pointer
        to it. If there is no message in the event
        queue, the function waits with t timeout until a message will be
        available. If the timeout expires and there is still no message
        in the queue, the function returns nullptr.
        )docstring",
        pybind11::arg("timeout"),
        pybind11::return_value_policy::reference
    );

    py_cSimpleModule.def(
        "wait",
        [] (omnetpp::cSimpleModule &self, double time) {self.wait(time); },
        R"docstring(
        Waits for the given interval. (Some other simulators call this
        functionality hold()).
        It is intended for use only if you do not expect other messages
        to arrive at the module during the wait period. To assert this,
        it throws an exception if a message arrives during the wait.

        If you expect to receive messages during the call, you should use
        waitAndEnqueue() instead.

        This function can only be used with activity(), but not with
        handleMessage().
        )docstring",
        pybind11::arg("time")
    );

    py_cSimpleModule.def(
        "wait",
        pybind11::overload_cast<omnetpp::simtime_t>(&omnetpp::cSimpleModule::wait),
        R"docstring(
        Waits for the given interval. (Some other simulators call this
        functionality hold()).
        It is intended for use only if you do not expect other messages
        to arrive at the module during the wait period. To assert this,
        it throws an exception if a message arrives during the wait.

        If you expect to receive messages during the call, you should use
        waitAndEnqueue() instead.

        This function can only be used with activity(), but not with
        handleMessage().
        )docstring",
        pybind11::arg("time")
    );

    py_cSimpleModule.def(
        "waitAndEnqueue",
        &omnetpp::cSimpleModule::waitAndEnqueue,
        R"docstring(
        Waits for the given interval. The messages received during the wait
        period are inserted into the queue passed as argument.

        This function can only be used with activity(), but not with
        handleMessage().
        )docstring",
        pybind11::arg("time"), pybind11::arg("queue")
    );

    py_cSimpleModule.def(
        "deleteModule",
        &omnetpp::cSimpleModule::deleteModule,
        /*
        [] (omnetpp::cSimpleModule & self)
        {
            if (self.getSimulation()->getContextModule() == &self)
            {
                INFO("Active module is deleting itself, setting global context");
                self.getSimulation()->setGlobalContext();
            }
            // call c++ implementation
            INFO("calling deleteModule()");
            self.deleteModule();
            INFO("back from deleteModule()");
        },
        */
        R"docstring(
        Deletes the module and all its (dynamically created) submodules.

        A running module can also delete itself. When an activity()-based
        simple module deletes itself from within its activity(), the
        deleteModule() call will not return (it throws an exception which
        gets caught by the simulation kernel, and the simulation kernel
        will delete the module).

        When a handleMessage()-based module deletes itself, the deleteModule()
        returns normally -- then, of course, the code should not try to
        access data members or functions of the deleted module, an should
        return as soon as possible.
        )docstring"
    );

    py_cSimpleModule.def(
        "snapshot",
        &omnetpp::cSimpleModule::snapshot,
        R"docstring(
        To be called from module functions. Outputs textual information
        about all objects of the simulation (including the objects created
        in module functions by the user!) into the snapshot file. The
        output is detailed enough to be used for debugging the simulation:
        by regularly calling snapshot(), one can trace how the
        values of variables, objects changed over the simulation. The
        arguments: label is a string that will appear in the
        output file; obj is the object whose inside is of interest.
        By default, the whole simulation (all modules etc) will be written
        out.

        Tkenv also supports making snapshots manually, from menu.

        See also class cWatch and the WATCH() macro.
        )docstring",
        pybind11::arg("obj") = nullptr, pybind11::arg("label") = nullptr
    );
}
