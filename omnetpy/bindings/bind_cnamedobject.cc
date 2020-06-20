#include <pybind11/pybind11.h>

#include <omnetpp.h>
#include <omnetpp/cnamedobject.h>


void bind_cNamedObject(pybind11::module &m)
{
    pybind11::class_<omnetpp::cNamedObject> py_cNamedObject(
        m,
        "_cNamedObject",
        R"docstring(
        Extends cObject with a name string. Also includes a "flags" member,
        with bits open for use by subclasses.
        )docstring"
        );

    py_cNamedObject.def(
        "setName",
        &omnetpp::cNamedObject::setName,
        R"docstring(
        Sets object's name. The object creates its own copy of the string.
        `None` may also be passed, which will be interpreted as an
        empty string ("").
        )docstring",
        pybind11::arg("name")
    );

    py_cNamedObject.def(
        "getName",
        &omnetpp::cNamedObject::getName,
        R"docstring(
        Returns pointer to the object's name, a string stored in the object.
        This function never returns nullptr.
        )docstring"
    );
}
