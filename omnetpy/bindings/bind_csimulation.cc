#include <pybind11/pybind11.h>

#include <omnetpp.h>
#include <omnetpp/csimulation.h>

void bind_cSimulation(pybind11::module &m)
{
    pybind11::class_<
        omnetpp::cSimulation
        > py_cSimulation(
        m,
        "_cSimulation",
        R"docstring(
        Simulation manager class.

        cSimulation is the central class in \opp. It stores the active simulation
        model, and provides methods for setting up, running and finalizing simulations.

        Most cSimulation methods are not of interest for simulation model code,
        they are used internally (e.g. by the user interface libraries (Envir,
        Cmdenv, Tkenv) to set up and run simulations).

        Some methods which can be of interest when programming simple modules:
        getUniqueNumber(), getModuleByPath(), getModule(), snapshot().
        )docstring"
    );

    py_cSimulation.def(
        "getModule",
        &omnetpp::cSimulation::getModule,
        R"docstring(
        Looks up a module by ID. If the ID does not identify a module (e.g. invalid ID,
        module already deleted, or object is not a module), it returns nullptr.
        )docstring",
        pybind11::return_value_policy::reference,
        pybind11::arg("id")
    );
}
