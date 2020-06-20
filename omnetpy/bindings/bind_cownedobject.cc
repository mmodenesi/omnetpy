#include <pybind11/pybind11.h>

#include <omnetpp.h>
#include <omnetpp/cownedobject.h>


void bind_cOwnedObject(pybind11::module &m)
{
    pybind11::class_<omnetpp::cOwnedObject, omnetpp::cObject> py_cOwnedObject(
        m,
        "_cOwnedObject",
        R"docstring(
        A cObject that keeps track of its owner. It serves as base class
        for many classes in the \opp library.

        Instances of cOwnedObjects are kept track of by the simulation kernel,
        and may be inserted into cQueue and cArray.

        It is not always a good idea to subclass your own classes from
        cOwnedObject, especially if they are small data objects. The more
        lightweight cObject is often a better choice.

        Ownership management helps \opp catch common programming errors.
        The term ownership means the exclusive right and duty to delete
        owned objects.

        cOwnedObjects hold a pointer to their owner objects; the getOwner() method
        returns this pointer. An example will help to understand how it is used:

           - when you insert a cMessage into a cQueue, the cQueue will become
             the owner of the message, and will set the message's owner to itself.

           - a message object can be at one place only at any given time.
             When you try to insert the same cMessage again into another (or the same)
             cQueue, you'll get an error message that it is already in a cQueue --
             sparing you a sure crash later.

           - similarly, if you try to send the same message, you'll get an error
             message that it cannot be sent because it is still enqueued --
             another crash scenario eliminated. Like the previous one, this
             test is done by checking the owner pointer in cMessage.

           - even if you try to delete the message while it is in the queue,
             you'll get an error message instead of just a crash. This is because
             cOwnedObject destructor asks for the owner's blessing -- but cQueue will
             protest by throwing an exception.

           - when you remove the message from the cQueue, the cQueue will "release"
             the object; the current module will become the message's "soft" owner,
             changing the owner pointer in the message object.

           - "soft" owner means that now you can send the message object or insert
             it into another cQueue -- the module as a soft owner will let it go.

           - the same mechanism can ensure that when a self-message is currently
             scheduled (owner is the scheduled-events list) or sent to another module
             (owner is the other module) you cannot send or schedule it, or
             insert it into a queue. *In short: the ownership mechanism is good to your
             health.*

           - when the queue is deleted, it also deletes all objects it contains.
             (The cQueue always owns all objects inserted into it -- no exception).

        The above ownership mechanisms are at work when any cOwnedObject-subclass object
        gets inserted into any cOwnedObject-subclass container (cQueue, cArray).

        Some more details, in case you are writing a class that acts as a container:

           - you should use the functions take(), drop() on inserting/removing objects
           - you should delete the owned objects in the destructor
           - the copy constructor of a container should dup() the owned objects
             and take() the copies
           - if you want to have a class which contains cOwnedObject-subclasses as
             data members: your class (the enclosing object) should own them --
             call take() from the constructor and drop() from the destructor.
        )docstring"
    );

    py_cOwnedObject.def(
        "getOwner",
        &omnetpp::cOwnedObject::getOwner,
        R"docstring(
        Returns pointer to the owner of the object.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cOwnedObject.def(
        "isOwnedObject",
        &omnetpp::cOwnedObject::isOwnedObject,
        R"docstring(
        Returns true.
        )docstring"
    );

    py_cOwnedObject.def(
        "isSoftOwner",
        &omnetpp::cOwnedObject::isSoftOwner,
        R"docstring(
        Returns false in cOwnedObject and in all derived classes except cDefaultList.
        An object A is a "soft owner" if it allows a B object take() an object A owns.
        "Hard owners" will raise an error if some other object tries to take()
        an object they own. The only soft owner class is cDefaultList.
        )docstring"
    );

    py_cOwnedObject.def_static(
        "getTotalObjectCount",
        &omnetpp::cOwnedObject::getTotalObjectCount,
        R"docstring(
        Returns the total number of objects created since the start of the program
        (or since the last reset). The counter is incremented by cOwnedObject constructor.
        Counter is `signed` to make it easier to detect if it overflows
        during very long simulation runs.
        May be useful for profiling or debugging memory leaks.
        )docstring"
    );

    py_cOwnedObject.def_static(
        "getLiveObjectCount",
        &omnetpp::cOwnedObject::getLiveObjectCount,
        R"docstring(
        Returns the number of objects that currently exist in the program.
        The counter is incremented by cOwnedObject constructor and decremented by
        the destructor.
        May be useful for profiling or debugging memory leaks.
        )docstring"
    );

    py_cOwnedObject.def_static(
        "resetObjectCounters",
        &omnetpp::cOwnedObject::resetObjectCounters,
        R"docstring(
        Reset counters used by getTotalObjectCount() and getLiveObjectCount().
        (Note that getLiveObjectCount() may go negative after a reset call.)
        )docstring"
    );
}
