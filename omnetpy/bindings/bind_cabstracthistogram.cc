#include <pybind11/pybind11.h>

#include <omnetpp.h>


void bind_cAbstractHistogram(pybind11::module &m)
{
    pybind11::class_<omnetpp::cAbstractHistogram, omnetpp::cStdDev> py_cAbstractHistogram(
        m,
        "_cAbstractHistogram",
        R"docstring(
        Interface and base class for histogram-like density estimation classes.
        )docstring"
    );

    pybind11::class_<omnetpp::cAbstractHistogram::Bin> py_cAbstractHistogram_Bin(
        py_cAbstractHistogram,
        "Bin",
        R"docstring(
        Information about a histogram bin. This struct is not used
        internally by the histogram classes, only to return information to the user.
        )docstring"
    );

    py_cAbstractHistogram_Bin.def_readwrite("lower", &omnetpp::cAbstractHistogram::Bin::lower);
    py_cAbstractHistogram_Bin.def_readwrite("upper", &omnetpp::cAbstractHistogram::Bin::upper);
    py_cAbstractHistogram_Bin.def_readwrite("value", &omnetpp::cAbstractHistogram::Bin::value);
    py_cAbstractHistogram_Bin.def_readwrite("relativeFreq", &omnetpp::cAbstractHistogram::Bin::relativeFreq);
    py_cAbstractHistogram_Bin.def(pybind11::init<>());

    py_cAbstractHistogram.def(
        "binsAlreadySetUp",
        &omnetpp::cAbstractHistogram::binsAlreadySetUp,
        R"docstring(
        Returns true if histogram is already available. See setUpBins().
        )docstring"
    );

    py_cAbstractHistogram.def(
        "setUpBins",
        &omnetpp::cAbstractHistogram::setUpBins,
        R"docstring(
        Sets up histogram bins, possibly based on data collected during a
        precollection phase. Calling this if the bins are already set up
        raises an error.
        )docstring"
    );

    py_cAbstractHistogram.def(
        "getNumBins",
        &omnetpp::cAbstractHistogram::getNumBins,
        R"docstring(
        Returns the number of histogram bins.
        )docstring"
    );

    py_cAbstractHistogram.def(
        "getBinEdge",
        &omnetpp::cAbstractHistogram::getBinEdge,
        R"docstring(
        Returns the kth histogram bin edge. Legal values for k are 0 through
        getNumBins(), that is, there's one more edge than the number of bins.
        getBinEdge(0) returns the lower edge of the first bin, and
        getBinEdge(getNumBins()) returns the upper edge of the last bin.
        The lower edge is inclusive, the upper edge is exclusive.
        )docstring",
        pybind11::arg("k")
    );

    py_cAbstractHistogram.def(
        "getBinValue",
        &omnetpp::cAbstractHistogram::getBinValue,
        R"docstring(
        Returns the total weight of the observations in the k'th bin of the histogram.
        (In the unweighted case, every observation is regarded as having the weight 1.0.)
        )docstring",
        pybind11::arg("k")
    );

    py_cAbstractHistogram.def(
        "getBinPDF",
        &omnetpp::cAbstractHistogram::getBinPDF,
        R"docstring(
        Returns the estimated value of the Probability Density Function
        within bin k. This method simply divides the number of observations
        in bin k with the bin size and the number of total observations collected.
        )docstring",
        pybind11::arg("k")
    );

    py_cAbstractHistogram.def(
        "getNumUnderflows",
        &omnetpp::cAbstractHistogram::getNumUnderflows,
        R"docstring(
        Returns number of observations that were below the histogram range,
        independent of their weights. This includes the number of collected
        negative infinities as well.
        )docstring"
    );

    py_cAbstractHistogram.def(
        "getNumOverflows",
        &omnetpp::cAbstractHistogram::getNumOverflows,
        R"docstring(
        Returns number of observations that were above the histogram range,
        independent of their weights. This includes the number of collected
        positive infinities as well.
        )docstring"
    );

    py_cAbstractHistogram.def(
        "getUnderflowSumWeights",
        &omnetpp::cAbstractHistogram::getUnderflowSumWeights,
        R"docstring(
        Returns the total weight of the observations that were below the
        histogram range. This includes the weights of the collected negative
        infinities as well.
        )docstring"
    );

    py_cAbstractHistogram.def(
        "getOverflowSumWeights",
        &omnetpp::cAbstractHistogram::getOverflowSumWeights,
        R"docstring(
        Returns the total weight of the observations that were above the
        histogram range. This includes the weights of the collected positive
        infinities as well.
        )docstring"
    );

    py_cAbstractHistogram.def(
        "getNumNegInfs",
        &omnetpp::cAbstractHistogram::getNumNegInfs,
        R"docstring(
        Returns number of observations that were negative infinity, independent of their weights.
        )docstring"
    );

    py_cAbstractHistogram.def(
        "getNumPosInfs",
        &omnetpp::cAbstractHistogram::getNumPosInfs,
        R"docstring(
        Returns number of observations that were positive infinity, independent of their weights.
        )docstring"
    );

    py_cAbstractHistogram.def(
        "getNegInfSumWeights",
        &omnetpp::cAbstractHistogram::getNegInfSumWeights,
        R"docstring(
        Returns the total weight of the observations that were negative infinity.
        )docstring"
    );

    py_cAbstractHistogram.def(
        "getPosInfSumWeights",
        &omnetpp::cAbstractHistogram::getPosInfSumWeights,
        R"docstring(
        Returns the total weight of the observations that were negative infinity.
        )docstring"
    );

    py_cAbstractHistogram.def(
        "getBinInfo",
        &omnetpp::cAbstractHistogram::getBinInfo,
        R"docstring(
        Combines the functionality of getBinEdge(), getBinValue() and getBinPDF() into a
        single call.
        )docstring",
        pybind11::arg("k")
    );

    py_cAbstractHistogram.def(
        "getPDF",
        &omnetpp::cAbstractHistogram::getPDF,
        R"docstring(
        Returns the estimated value of the Probability Density Function at a given x.
        )docstring",
        pybind11::arg("x")
    );

    py_cAbstractHistogram.def(
        "getCDF",
        &omnetpp::cAbstractHistogram::getCDF,
        R"docstring(
        Returns the estimated value of the Cumulative Density Function at a given x.
        )docstring",
        pybind11::arg("x")
    );

    py_cAbstractHistogram.def(
        "draw",
        &omnetpp::cAbstractHistogram::draw,
        R"docstring(
        Returns a random number from the distribution represented by the histogram.
        )docstring"
    );
}
