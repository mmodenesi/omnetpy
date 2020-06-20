#include <pybind11/pybind11.h>

#include <omnetpp.h>
#include <omnetpp/cobject.h>


void bind_cObject(pybind11::module &m)
{
    // cObject ------------------------------------------------------------------------------------
    pybind11::class_<omnetpp::cObject, std::unique_ptr<omnetpp::cObject, pybind11::nodelete>> py_cObject(
        m,
        "_cObject",
        R"docstring(
        Root of the \opp class hierarchy.

        cObject is a lightweight class without any data members.

        cObject and its subclass cOwnedObject define an ownership mechanism.
        Any cObject may become owner of other objects, but owned objects must be
        subclassed from cOwnedObject. cObject also contains methods that allow
        the object to be displayed in graphical user interfaces (Tkenv, Qtenv).

        It is recommended to use cObject as a base class for any class
        that has at least one virtual member function. This makes the class more
        interoperable with \opp, and causes no extra overhead at all.
        sizeof(cObject) should yield 4 on a 32-bit architecture (4-byte vptr),
        and using cObject as a base class does not add anything to the size
        of an object, because a class with a virtual function already has a vptr.
        Subclasses are expected to redefine member functions such as dup(),
        str(), forEachChild(), etc.

        Note: The (cPolymorphic, cObject) classes in \opp versions 2.x and 3.x
        were renamed to (cObject, cOwnedObject) in version 4.0.
        )docstring"
    );

    py_cObject.def(
        "__str__",
        &omnetpp::cObject::str
    );

    py_cObject.def(
        "__repr__",
        [] (const omnetpp::cObject & self) { return "cObject()"; }
    );

    py_cObject.def(
        "getClassName",
        &omnetpp::cObject::getClassName,
        R"docstring(
        Returns the (fully qualified) class name. This method is implemented
        using typeid (C++ RTTI), and it does not need to be overridden in
        subclasses.
        )docstring"
    );

    py_cObject.def(
        "getName",
        &omnetpp::cObject::getName,
        R"docstring(
        Returns object's name. It should never return `None`.
        The default implementation just returns an empty string ("").
        )docstring"
    );

    py_cObject.def(
        "isName",
        &omnetpp::cObject::isName,
        R"docstring(
        Returns true if the object's name is identical to the string passed.
        )docstring"
    );

    py_cObject.def(
        "getFullName",
        &omnetpp::cObject::getFullName,
        R"docstring(
        When this object is part of a vector (like a submodule can be part of
        a module vector, or a gate can be part of a gate vector), this method
        returns the object's name with the index in brackets; for example:
        "out[5]".
        
        This default implementation just returns getName().
        )docstring"
    );

    py_cObject.def(
        "str",
        &omnetpp::cObject::str,
        R"docstring(
        Produce a one-line description of the object. The string is displayed
        at various places e.g. in graphical user interfaces.
        )docstring"
    );

    py_cObject.def(
        "parsimPack",
        &omnetpp::cObject::parsimPack,
        R"docstring(
        Serializes the object into a buffer. This default implementation throws
        an exception ("packing not implemented").
        )docstring"
    );

    py_cObject.def(
        "parsimUnpack",
        &omnetpp::cObject::parsimUnpack,
        R"docstring(
        Deserializes the object from a buffer. This default implementation just
        throws an exception ("packing not implemented").
        )docstring"
    );

    py_cObject.def(
        "getOwner",
        &omnetpp::cObject::getOwner,
        R"docstring(
        Returns owner (or parent) object. This default implementation just
        returns `None`.
        )docstring"
    );

    py_cObject.def(
        "isOwnedObject",
        &omnetpp::cObject::isOwnedObject,
        R"docstring(
        Returns true if this class is a subclass of cOwnedObject.
        This is a performance optimization, to avoid frequent calls to
        dynamic_cast\<\>. In cObject this method returns false, in
        cOwnedObject it returns true, and it MUST NOT be defined on
        any other class.
        )docstring"
    );

    py_cObject.def(
        "forEachChild",
        &omnetpp::cObject::forEachChild,
        R"docstring(
        Enables traversing the object tree, performing some operation on
        each object. The operation is encapsulated in the particular subclass
        of cVisitor.

        This method should be redefined in every subclass to call v->visit(obj)
        for every obj object contained.
        )docstring",
        pybind11::arg("visitor")
    );

    py_cObject.def(
        "findObject",
        &omnetpp::cObject::findObject,
        R"docstring(
        Finds the object with the given name. This function is useful when called
        on subclasses that are containers. This method
        finds the object with the given name in a container object and
        returns a pointer to it or nullptr if the object has not
        been found. If deep is false, only objects directly
        contained will be searched, otherwise the function searches the
        whole subtree for the object. It uses the forEachChild() mechanism.

        Do not use it for finding submodules! Use cModule::getModuleByRelativePath()
        instead.
        )docstring",
        pybind11::arg("name"), pybind11::arg("deep") = true
    );
}
