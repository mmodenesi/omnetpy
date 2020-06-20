#include <pybind11/pybind11.h>

#include <omnetpp.h>
#include <omnetpp/cmodule.h>


void bind_cModule(pybind11::module &m)
{
    pybind11::class_<
        omnetpp::cModule,
        omnetpp::cComponent
        > py_cModule(
        m,
        "_cModule",
        R"docstring(
        cModule can be used directly for compound modules. Simple module
        classes need to be subclassed from cSimpleModule, a class that
        adds more functionality to cModule.

        For navigating the module tree, see:
        getParentModule(), getSubmodule(), cModule::SubmoduleIterator,
        cModule::getModuleByPath(), cSimulation::getModuleByPath().
        )docstring"
    );

    py_cModule.def(
        "__str__",
        &omnetpp::cModule::str,
        R"docstring(
        Overridden to add the module ID.
        )docstring"
    );

    py_cModule.def(
        "getModuleByPath",
        &omnetpp::cModule::getModuleByPath,
        R"docstring(
        Sets object's name. Redefined to update the stored fullName string.
        )docstring",
        pybind11::arg("path"),
        pybind11::return_value_policy::reference
    );

    py_cModule.def(
        "setName",
        &omnetpp::cModule::setName,
        R"docstring(
        Sets object's name. Redefined to update the stored fullName string.
        )docstring"
    );

    py_cModule.def(
        "getFullName",
        &omnetpp::cModule::getFullName,
        R"docstring(
        Returns the full name of the module, which is getName() plus the
        index in square brackets (e.g. "module[4]"). Redefined to add the
        index.
        )docstring"
    );

    py_cModule.def(
        "getFullPath",
        &omnetpp::cModule::getFullPath,
        R"docstring(
        Returns the full path name of the module.  Example: `net.node[12].gen`

        The original getFullPath() was redefined in order to hide the global cSimulation
        instance from the path name.
        )docstring"
    );

    py_cModule.def(
        "str",
        &omnetpp::cModule::str,
        R"docstring(
        Overridden to add the module ID.
        )docstring"
    );

    py_cModule.def(
        "getParentModule",
        &omnetpp::cModule::getParentModule,
        R"docstring(
        Returns the module containing this module. For the system module,
        tt returns `None`.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cModule.def(
        "getSubmodule",
        &omnetpp::cModule::getSubmodule,
        R"docstring(
        Finds a direct submodule with the given name and index.
        If the submodule was not found, returns `None`.
        Index must be specified exactly if the module is member of a module vector.
        )docstring",
        pybind11::arg("name"), pybind11::arg("index") = -1,
        pybind11::return_value_policy::reference
    );

    py_cModule.def(
        "isSimple",
        &omnetpp::cModule::isSimple,
        R"docstring(
        Returns true if the class of this module is a subclass of cSimpleModule,
        and false otherwise.

        In contrast, to check whether this module is a NED compound or simple module
        (i.e. whether it was declared with the "module" or with the "simple" keyword in NED),
        use `cModuleType::isSimple()`: `getModuleType().isSimple()`.
        )docstring"
    );

    py_cModule.def(
        "getComponentKind",
        &omnetpp::cModule::getComponentKind,
        R"docstring(
        Redefined from cComponent to return KIND_MODULE.
        )docstring"
    );


    py_cModule.def(
        "isVector",
        &omnetpp::cModule::isVector,
        R"docstring(
        Returns true if this module is in a module vector.
        )docstring"
    );

    py_cModule.def(
        "gateSize",
        &omnetpp::cModule::gateSize,
        R"docstring(
        Returns the size of the gate vector with the given name. It returns 1 for
        non-vector gates, and 0 if the gate does not exist or the vector has size 0.
        (Zero-size vectors are represented by a single gate whose size() returns 0.)
        Gate names with the "$i" or "$o" suffix are also accepted.  Throws an error if
        there is no such gate or gate vector.

        Note: The gate vector size can also be obtained by calling the cGate::size()
        method of any gate object.
        )docstring",
        pybind11::arg("gatename")
    );

    py_cModule.def(
        "getIndex",
        &omnetpp::cModule::getIndex,
        R"docstring(
        Returns the index of the module if it is in a module vector, otherwise 0.
        )docstring"
    );

    py_cModule.def(
        "getVectorSize",
        &omnetpp::cModule::getVectorSize,
        R"docstring(
        Returns the size of the module vector the module is in. For non-vector
        modules it returns 1.
        )docstring"
    );

    py_cModule.def(
        "hasSubmodules",
        &omnetpp::cModule::hasSubmodules,
        R"docstring(
        Returns true if the module has submodules, and false otherwise.
        )docstring"
    );

    py_cModule.def(
        "getCanvas",
        &omnetpp::cModule::getCanvas,
        pybind11::return_value_policy::reference
    );

    py_cModule.def(
        "gate",
        pybind11::overload_cast<const char *, int>(&omnetpp::cModule::gate),
        R"docstring(
        Looks up a gate by its name and index. Gate names with the "$i" or "$o"
        suffix are also accepted. Throws an error if the gate does not exist.
        The presence of the index parameter decides whether a vector or a scalar
        gate will be looked for.
        )docstring",
        pybind11::arg("gatename"), pybind11::arg("index") = -1,
        pybind11::return_value_policy::reference
    );

    py_cModule.def(
        "buildInside",
        &omnetpp::cModule::buildInside,
        R"docstring(
        In compound modules, this method should be called to create submodules
        and internal connections after module creation.

        This method delegates to doBuildInside(), switching the context to this
        module for the duration of the call (see simulation.setContextModule()).
        )docstring"
    );

    py_cModule.def(
        "finalizeParameters",
        &omnetpp::cModule::finalizeParameters,
        R"docstring(
        Redefined from cComponent. This method must be called as part of the module
        creation process, after moduleType->create() and before mod->buildInside().
        It finalizes parameter values (e.g. reads the missing ones from omnetpp.ini),
        and adds gates and gate vectors (whose size may depend on parameter values)
        to the module.

        So the sequence of setting up a module is:
         1. modType->create()
         2. set parameter values
         3. mod->finalizeParameters() -- this creates gates too
         4. connect gates (possibly adding new gates via gate++ operations)
         5. mod->buildInside()

        The above sequence also explains why finalizeParameters() cannot by merged
        into either create() or buildInside().
        )docstring"
    );

    py_cModule.def(
        "getOrCreateFirstUnconnectedGatePair",
        [](omnetpp::cModule &self, const char *gatename, bool inside, bool expand) {
            omnetpp::cGate *in, *out;
            self.getOrCreateFirstUnconnectedGatePair(gatename, inside, expand, in, out);
            return pybind11::make_tuple(in, out);
        },
        R"docstring(
        Helper function to implement NED's "gate++" syntax.  The result is returned in a (gatein,
        gateout) tuple. The meaning of the inside and expand parameters is the same as with
        getOrCreateFirstUnconnectedGate().
        )docstring",
        pybind11::arg("gatename"), pybind11::arg("inside"), pybind11::arg("expand")
    );

    py_cModule.def(
        "callInitialize",
        pybind11::overload_cast<>(&omnetpp::cModule::callInitialize),
        R"docstring(
        Interface for calling initialize() from outside.
        )docstring"
    );

    py_cModule.def(
        "callInitialize",
        pybind11::overload_cast<int>(&omnetpp::cModule::callInitialize),
        R"docstring(
        Interface for calling initialize() from outside. It does a single stage
        of initialization, and returns <tt>true</tt> if more stages are required.
        )docstring",
        pybind11::arg("stage")
    );

}
