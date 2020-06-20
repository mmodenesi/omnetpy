#include <pybind11/pybind11.h>

#include <omnetpp.h>


void bind_cStatistic(pybind11::module &m)
{
    pybind11::class_<
        omnetpp::cStatistic,
        omnetpp::cOwnedObject,
        omnetpp::cObject> py_cStatistic(
        m,
        "_cStatistic",
        R"docstring(
        cStatistic is an abstract class for computing statistical properties
        of a random variable.

        This class inherits from cRandom, because instances are able to generate
        random variates from the estimated distribution.
        )docstring"
    );

    py_cStatistic.def(
        "isWeighted",
        &omnetpp::cStatistic::isWeighted,
        R"docstring(
        Returns true if this object collects weighted statistics.
        )docstring"
    );

    py_cStatistic.def(
        "collect",
        pybind11::overload_cast<double>(&omnetpp::cStatistic::collect),
        R"docstring(
        Collects one value.
        )docstring",
        pybind11::arg("value")
    );

    py_cStatistic.def(
        "collect",
        pybind11::overload_cast<omnetpp::SimTime>(&omnetpp::cStatistic::collect),
        R"docstring(
        Convenience method, delegates to collect(double).
        )docstring",
        pybind11::arg("value")
    );

    py_cStatistic.def(
        "collectWeighted",
        pybind11::overload_cast<double, double>(&omnetpp::cStatistic::collectWeighted),
        R"docstring(
        Collects one value with a given weight.
        )docstring",
        pybind11::arg("value"), pybind11::arg("weight")
    );

    py_cStatistic.def(
        "collectWeighted",
        pybind11::overload_cast<omnetpp::SimTime, double>(&omnetpp::cStatistic::collectWeighted),
        R"docstring(
        Convenience method, delegates to collectWeighted(double, double).
        )docstring",
        pybind11::arg("value"), pybind11::arg("weight")
    );

    py_cStatistic.def(
        "collectWeighted",
        pybind11::overload_cast<double, omnetpp::SimTime>(&omnetpp::cStatistic::collectWeighted),
        R"docstring(
        Convenience method, delegates to collectWeighted(double, double).
        )docstring",
        pybind11::arg("value"), pybind11::arg("weight")
    );

    py_cStatistic.def(
        "collectWeighted",
        pybind11::overload_cast<omnetpp::SimTime, omnetpp::SimTime>(&omnetpp::cStatistic::collectWeighted),
        R"docstring(
        Convenience method, delegates to collectWeighted(double, double).
        )docstring",
        pybind11::arg("value"), pybind11::arg("weight")
    );

    py_cStatistic.def(
        "merge",
        &omnetpp::cStatistic::merge,
        R"docstring(
        Updates this object with data coming from another statistics
        object. The result is as if this object had collected all the
        observations fed into the other object as well.
        )docstring",
        pybind11::arg("other")
    );

    py_cStatistic.def(
        "clear",
        &omnetpp::cStatistic::clear,
        R"docstring(
        This function should be redefined in derived classes to clear
        the results collected so far.
        )docstring"
    );

    py_cStatistic.def(
        "getCount",
        &omnetpp::cStatistic::getCount,
        R"docstring(
        Returns the number of the observations, regardless of their weights.
        )docstring"
    );

    py_cStatistic.def(
        "getSum",
        &omnetpp::cStatistic::getSum,
        R"docstring(
        Returns the sum of the values. Not available for weighted statistics.
        )docstring"
    );

    py_cStatistic.def(
        "getSqrSum",
        &omnetpp::cStatistic::getSqrSum,
        R"docstring(
        Returns the sum of the squared values. Not available for weighted statistics.
        )docstring"
    );

    py_cStatistic.def(
        "getMin",
        &omnetpp::cStatistic::getMin,
        R"docstring(
        Returns the minimum of the values.
        )docstring"
    );

    py_cStatistic.def(
        "getMax",
        &omnetpp::cStatistic::getMax,
        R"docstring(
        Returns the maximum of the values.
        )docstring"
    );

    py_cStatistic.def(
        "getMean",
        &omnetpp::cStatistic::getMean,
        R"docstring(
        Returns the mean of the (weighted) observations.
        )docstring"
    );

    py_cStatistic.def(
        "getStddev",
        &omnetpp::cStatistic::getStddev,
        R"docstring(
        Returns the standard deviation of the (weighted) observations.
        )docstring"
    );

    py_cStatistic.def(
        "getVariance",
        &omnetpp::cStatistic::getVariance,
        R"docstring(
        Returns the variance of the (weighted) observations.
        )docstring"
    );

    py_cStatistic.def(
        "getSumWeights",
        &omnetpp::cStatistic::getSumWeights,
        R"docstring(
        Returns the sum of weights.
        )docstring"
    );

    py_cStatistic.def(
        "getWeightedSum",
        &omnetpp::cStatistic::getWeightedSum,
        R"docstring(
        Returns the sum of weight*value products.
        )docstring"
    );

    py_cStatistic.def(
        "getSqrSumWeights",
        &omnetpp::cStatistic::getSqrSumWeights,
        R"docstring(
        Returns the sum of squared weights.
        )docstring"
    );

    py_cStatistic.def(
        "getWeightedSqrSum",
        &omnetpp::cStatistic::getWeightedSqrSum,
        R"docstring(
        Returns the sum of weight*value*value products.
        )docstring"
    );

    py_cStatistic.def(
        "saveToFile",
        &omnetpp::cStatistic::saveToFile,
        R"docstring(
        Writes the contents of the object into a text file.
        This method is pure virtual, implementation is provided in subclasses.
        )docstring",
        pybind11::arg("fh")
    );

    py_cStatistic.def(
        "loadFromFile",
        &omnetpp::cStatistic::loadFromFile,
        R"docstring(
        Reads the object data from a file written out by saveToFile().
        This method is pure virtual, implementation is provided in subclasses.
        )docstring",
        pybind11::arg("fh")
    );

    py_cStatistic.def(
        "record",
        &omnetpp::cStatistic::record,
        R"docstring(
        Records the statistics into the scalar result file. (Recorded data
        includes the number of observations, mean, standard deviation, min,
        max, potential histogram data, etc.)  The name of the recorded
        statistic will be the object name (see getFullName()).

        Note that this operation may have side effect: if this object is a
        histogram, the method may invoke the setUpBins() on the histogram
        object, to force it set up histogram bins before recording.
        )docstring"
    );

    py_cStatistic.def(
        "recordWithUnit",
        &omnetpp::cStatistic::recordWithUnit,
        R"docstring(
        Records the statistics into the scalar result file, with the given
        unit (e.g. "s", "m/s", etc). The name of the recorded statistic
        will be the object name (see getName()).

        Note that this operation may have side effect: if this object is a
        histogram, the method may invoke the setUpBins() on the histogram
        object, to force it set up histogram bins before recording.
        )docstring",
        pybind11::arg("unit")
    );

    py_cStatistic.def(
        "recordAs",
        &omnetpp::cStatistic::recordAs,
        R"docstring(
        Records the statistics into the scalar result file, with the given name,
        and optionally, the given unit (e.g. "s", "m/s", etc).

        Note that this operation may have side effect: if this object is a
        histogram, the method may invoke the setUpBins() on the histogram
        object, to force it set up histogram bins before recording.
        )docstring",
        pybind11::arg("name"), pybind11::arg("unit") = nullptr
    );
}
