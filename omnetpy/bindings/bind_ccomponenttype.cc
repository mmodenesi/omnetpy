#include <pybind11/pybind11.h>

#include <omnetpp.h>
#include <omnetpp/ccomponenttype.h>

void bind_cComponentType(pybind11::module &m)
{

    pybind11::class_<
        omnetpp::cComponentType,
        //,
        // omnetpp::cNoncopyableOwnedObject,
        std::unique_ptr<omnetpp::cComponentType, pybind11::nodelete>
        > py_cComponentType(
        m,
        "_cComponentType",
        R"docstring(
        Common base class for cModuleType and cChannelType.

        The getName() method returns the unqualified name (without namespace, e.g.
        "Queue"), and getFullName() returns the qualified name (with namespace,
        e.g. "inet.network.Queue").
        )docstring"
    );

    py_cComponentType.def(
        "getFullName",
        &omnetpp::cComponentType::getFullName,
        R"docstring(
        Returns the fully qualified name (i.e. the simple name prefixed
        with the package name and any existing enclosing NED type names).

        Examples:

        The fully qualified name for a module named <tt>Fifo</tt> which is
        in the default package is <tt>"Fifo"</tt>.

        For a module named <tt>Host</tt> which is in the package <tt>some.package</tt>,
        the fully qualified name is <tt>"some.package.Host"</tt>.

        For a channel type <tt>Ch</tt> which is defined as an inner type inside
        the network <tt>Network</tt> in a package named <tt>some.package</tt>, the
        fully qualified name is <tt>"some.package.Network.Ch"</tt>.
        )docstring"
    );

    py_cComponentType.def_static(
        "find",
        &omnetpp::cComponentType::find,
        R"docstring(
        Finds a component type by fully qualified NED type name.
        Returns nullptr if not found.
        )docstring",
        pybind11::arg("qname"),
        pybind11::return_value_policy::reference
    );

    py_cComponentType.def_static(
        "get",
        &omnetpp::cComponentType::get,
        R"docstring(
        Finds a component type by fully qualified NED type name.
        Throws an error if not found.
        )docstring",
        pybind11::arg("qname"),
        pybind11::return_value_policy::reference
    );

    pybind11::class_<
        omnetpp::cModuleType,
        omnetpp::cComponentType,
        std::unique_ptr<omnetpp::cModuleType, pybind11::nodelete>
    > py_cModuleType(
        m,
        "_cModuleType",
        R"docstring(
        Abstract class for creating a module of a specific type.

        A cModuleType object exist for each module type (simple or compound).
        )docstring"
    );

    py_cModuleType.def(
        "_createScheduleInit",
        &omnetpp::cModuleType::createScheduleInit,
        R"docstring(
        This is a convenience function to get a module up and running in one step.

        First, the module is created using create() and buildInside(), then
        a starter message is created (for activity() modules only),
        then initialize() is called (mod->callInitialize()).

        This method works for simple and compound modules alike. Not applicable
        if the module:
         - has parameters to be set
         - gate vector sizes to be set
         - gates to be connected before initialize()
        )docstring",
        pybind11::arg("name"), pybind11::arg("parentmod"),
        pybind11::return_value_policy::reference
    );

    py_cModuleType.def_static(
        "find",
        &omnetpp::cModuleType::find,
        R"docstring(
        Finds a module type by fully qualified NED type name.
        Returns nullptr if not found.
        )docstring",
        pybind11::arg("qname"),
        pybind11::return_value_policy::reference
    );

    py_cModuleType.def_static(
        "get",
        &omnetpp::cModuleType::get,
        R"docstring(
        Finds a module type by fully qualified NED type name.
        Throws an error if not found.
        )docstring",
        pybind11::arg("qname"),
        pybind11::return_value_policy::reference
    );

    py_cModuleType.def(
        "create",
        pybind11::overload_cast<const char*, omnetpp::cModule *>(&omnetpp::cModuleType::create),
        R"docstring(
        Creates a module which is not element of a module vector.
        In addition to creating an object of the correct type,
        this function inserts the module into the simulation's data structure,
        and adds the parameters and gates specified in the NED declaration.
        )docstring",
        pybind11::arg("name"), pybind11::arg("parentmod"),
        pybind11::return_value_policy::reference
    );

    py_cModuleType.def(
        "create",
        pybind11::overload_cast<const char*, omnetpp::cModule *, int, int>(&omnetpp::cModuleType::create),
        R"docstring(
        Creates a module to be an element of a module vector.
        The last two arguments specify the vector size and the index
        of the new module within the vector.
        )docstring",
        pybind11::arg("name"), pybind11::arg("parentmod"), pybind11::arg("vectorsize"), pybind11::arg("index"),
        pybind11::return_value_policy::reference
    );
};
