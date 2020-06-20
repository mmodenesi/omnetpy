#include <pybind11/pybind11.h>

#include <omnetpp.h>


void bind_cPSquare(pybind11::module &m)
{
    pybind11::class_<
        omnetpp::cPSquare,
        omnetpp::cAbstractHistogram,
        omnetpp::cStdDev,
        omnetpp::cStatistic,
        omnetpp::cOwnedObject,
        omnetpp::cNamedObject,
        omnetpp::cObject
        > py_cPSquare(
        m,
        "_cPSquare",
        R"docstring(
        Implements the P<sup>2</sup> algorithm, which calculates quantile
        values without storing the observations. See the seminal paper titled
        "The P^2 Algorithm for Dynamic Statistical Computing Calculation of
        Quantiles and Histograms Without Storing Observations" by Raj Jain and
        Imrich Chlamtac.
        )docstring"
    );

    py_cPSquare.def(
        pybind11::init<const char *, int>(),
        pybind11::arg("name") = nullptr, pybind11::arg("bins") = 10
    );

    py_cPSquare.def(
        "binsAlreadySetUp",
        &omnetpp::cPSquare::binsAlreadySetUp,
        R"docstring(
        Returns true if histogram is already available. This cPSquare implementation
        always returns true, since the algorithm does not contain a precollection stage.
        )docstring"
    );

    py_cPSquare.def(
        "setUpBins",
        &omnetpp::cPSquare::setUpBins,
        R"docstring(
        Transforms the array of pre-collected values into histogram structure.
        This cPSquare implementation does nothing.
        )docstring"
    );

    py_cPSquare.def(
        "collect",
        pybind11::overload_cast<double>(&omnetpp::cStatistic::collect),
        R"docstring(
        Collects one observation.
        )docstring",
        pybind11::arg("val")
    );

    py_cPSquare.def(
        "collectWeighted",
        pybind11::overload_cast<double, double>(&omnetpp::cStatistic::collectWeighted),
        R"docstring(
        Collects one observation with a given weight. Throws an error,
        as cPSquare does not support weighted statistics.
        )docstring",
        pybind11::arg("value"), pybind11::arg("weight")
    );

    py_cPSquare.def(
        "getBinEdge",
        &omnetpp::cPSquare::getBinEdge,
        R"docstring(
        Returns the kth bin boundary. Note that because of the P2 algorithm,
        bin boundaries are shifting during data collection, thus getBinValue() and
        other methods based on getBinValue() and getBinEdge() return approximate values.
        )docstring",
        pybind11::arg("k")
    );

    py_cPSquare.def(
        "getBinValue",
        &omnetpp::cPSquare::getBinValue,
        R"docstring(
        Returns the number of observations in the kth histogram bin.
        )docstring",
        pybind11::arg("k")
    );

    py_cPSquare.def(
        "getNumUnderflows",
        &omnetpp::cPSquare::getNumUnderflows,
        R"docstring(
        Returns number of observations that were below the histogram range,
        independent of their weights. In cPSquare, this is always the same as the
        number of negative infinities.
        )docstring"
    );

    py_cPSquare.def(
        "getNumOverflows",
        &omnetpp::cPSquare::getNumOverflows,
        R"docstring(
        Returns number of observations that were above the histogram range,
        independent of their weights. In cPSquare, this is always the same as the
        number of positive infinities.
        )docstring"
    );

    py_cPSquare.def(
        "getUnderflowSumWeights",
        &omnetpp::cPSquare::getUnderflowSumWeights,
        R"docstring(
        Returns the total weight of the observations that were below the histogram range.
        In cPSquare, this is always the same as the number of negative infinities.
        )docstring"
    );

    py_cPSquare.def(
        "getOverflowSumWeights",
        &omnetpp::cPSquare::getOverflowSumWeights,
        R"docstring(
        Returns the total weight of the observations that were above the histogram range.
        In cPSquare, this is always the same as the number of positive infinities.
        )docstring"
    );

    py_cPSquare.def(
        "getNegInfSumWeights",
        &omnetpp::cPSquare::getNegInfSumWeights,
        R"docstring(
        Returns the total weight of the observations that were negative infinity.
        Since cPSquare does not support weighted statistics, this is always the same
        as the number of observations that were negative infinity.
        )docstring"
    );

    py_cPSquare.def(
        "getPosInfSumWeights",
        &omnetpp::cPSquare::getPosInfSumWeights,
        R"docstring(
        Returns the total weight of the observations that were positive infinity.
        Since cPSquare does not support weighted statistics, this is always the same
        as the number of observations that were positive infinity.
        )docstring"
    );

    py_cPSquare.def(
        "merge",
        &omnetpp::cPSquare::merge,
        R"docstring(
        Merging is not supported by this class. This method throws an error.
        )docstring",
        pybind11::arg("other")
    );

    py_cPSquare.def(
        "clear",
        &omnetpp::cPSquare::clear,
        R"docstring(
        Clears the results collected so far.
        )docstring"
    );

    py_cPSquare.def(
        "saveToFile",
        &omnetpp::cPSquare::saveToFile,
        R"docstring(
        Writes the contents of the object into a text file.
        )docstring",
        pybind11::arg("fh")
    );

    py_cPSquare.def(
        "loadFromFile",
        &omnetpp::cPSquare::loadFromFile,
        R"docstring(
        Reads the object data from a file, in the format written out by saveToFile().
        )docstring",
        pybind11::arg("fh")
    );
}
