#include <pybind11/pybind11.h>

#include <omnetpp.h>
#include <omnetpp/cpar.h>


void bind_cPar(pybind11::module &m)
{
    // must use pybind11::nodelete since the destructor of cPar is private
    pybind11::class_<omnetpp::cPar, std::unique_ptr<omnetpp::cPar, pybind11::nodelete>> py_cPar(
        m,
        "_cPar",
        R"docstring(
        Represents a module or channel parameter.

        When a module or channel is created, parameter objects are added
        automatically, based on the NED declaration of the module/channel.
        It is not possible to create further parameters (or to remove parameters)
        at runtime. This is enforced by the cPar constructor being private.

        Parameters get their initial values automatically, from the NED
        declarations and the configuration. It is possible to change the
        parameter value during runtime (see various setter methods and
        operator='s), but not the type of the parameter (see getType()).
        The type correspond to NED types (bool, double, int, string, xml),
        and cannot be changed at runtime.

        The module or channel object can get notified when a parameter is
        changed; one has to override cComponent::handleParameterChange()
        for that.

        .. note::

            In earlier versions of \opp, cPar could be used as a
            general value storage object, and attached to cMessages as well.
            From the 4.0 version, simulation models should use cMsgPar for that.

        **Implementation note:** from the 4.0 version, almost all methods
        of cPar delegates to an internal cParImpl object, which actually stores
        the value, and generally does the real job. This was done to allow
        sharing parameter objects which have the same name, same value, etc.
        among module/channel instances. This significantly reduces memory
        consumption of most simulation models. Because cPar is just a thin
        wrapper around cParImpl, cPar is not meant for subclassing, and
        none if its methods are virtual. cParImpl and subclasses should also
        be regarded as internal data structures, and they should not be
        directly accessed or manipulated from model code.
        )docstring"
    );

    py_cPar.def(
        "getName",
        &omnetpp::cPar::getName,
        R"docstring(
        Returns the parameter name.
        )docstring"
    );

    py_cPar.def(
        "str",
        &omnetpp::cPar::str,
        R"docstring(
        Returns the value in text form.
        )docstring"
    );

    py_cPar.def(
        "isNumeric",
        &omnetpp::cPar::isNumeric,
        R"docstring(
        Returns true if the stored value is of a numeric type.
        )docstring"
    );

    py_cPar.def(
        "isVolatile",
        &omnetpp::cPar::isVolatile,
        R"docstring(
        Returns true if this parameter is marked in the NED file as "volatile".
        This flag affects the operation of setExpression().
        )docstring"
    );

    py_cPar.def(
        "isExpression",
        &omnetpp::cPar::isExpression,
        R"docstring(
        Returns false if the stored value is a constant, and true if it is
        an expression. (It is not examined whether the expression yields
        a constant value.)
        )docstring"
    );

    py_cPar.def(
        "isSet",
        &omnetpp::cPar::isSet,
        R"docstring(
        Returns true if the parameter is assigned a value, and false otherwise.
        Parameters of an already initialized module or channel are guaranteed to
        assigned, so this method will return true for them.
        )docstring"
    );

    py_cPar.def(
        "containsValue",
        &omnetpp::cPar::containsValue,
        R"docstring(
        Returns true if the parameter is set (see isSet()) or contains a default
        value, and false otherwise. Parameters of an already initialized module or
        channel are guaranteed to be assigned, so this method will return true for them.
        )docstring"
    );

    py_cPar.def(
        "boolValue",
        &omnetpp::cPar::boolValue,
        R"docstring(
        Returns value as a boolean. The cPar type must be BOOL.
        )docstring"
    );

    py_cPar.def(
        "intValue",
        &omnetpp::cPar::intValue,
        R"docstring(
        Returns value as intpar_t. The cPar type must be INT.
        Note: Implicit conversion from DOUBLE is intentionally missing.
        )docstring"
    );

    py_cPar.def(
        "doubleValue",
        &omnetpp::cPar::doubleValue,
        R"docstring(
        Returns value as double. The cPar type must be DOUBLE.
        Note: Implicit conversion from INT is intentionally missing.
        )docstring"
    );

    py_cPar.def(
        "doubleValueInUnit",
        &omnetpp::cPar::doubleValueInUnit,
        R"docstring(
        Returns the numeric value as a double converted to the given unit.
        If the current unit cannot be converted to the given one, an error
        will be thrown. The type must be DOUBLE or INT.
        )docstring",
        pybind11::arg("targetUnit")
    );

    py_cPar.def(
        "getUnit",
        &omnetpp::cPar::getUnit,
        R"docstring(
        Returns the parameter's unit ("s", "mW", "Hz", "bps", etc) as declared
        with the @unit property of the parameter in NED, or nullptr if no unit
        was specified. Unit is only valid for INT and DOUBLE types.
        )docstring"
    );

    py_cPar.def(
        "stringValue",
        &omnetpp::cPar::stringValue,
        R"docstring(
        Returns value as const char *. The cPar type must be STRING.
        This method may only be invoked when the parameter's value is a
        string constant and not the result of expression evaluation, otherwise
        an error is thrown. This practically means this method cannot be used
        on parameters declared as "volatile string" in NED; they can only be
        accessed using stdstringValue().
        )docstring"
    );

    py_cPar.def(
        "stdstringValue",
        &omnetpp::cPar::stdstringValue,
        R"docstring(
        Returns value as string. The cPar type must be STRING.
        )docstring"
    );
}
