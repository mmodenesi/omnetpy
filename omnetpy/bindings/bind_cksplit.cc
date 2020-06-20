#include <pybind11/pybind11.h>

#include <omnetpp.h>


void bind_cKSplit(pybind11::module &m)
{
    pybind11::class_<
        omnetpp::cKSplit,
        // omnetpp::cPrecollectionBasedDensityEst,
        omnetpp::cAbstractHistogram,
        omnetpp::cStdDev,
        omnetpp::cStatistic,
        omnetpp::cOwnedObject,
        omnetpp::cNamedObject,
        omnetpp::cObject> py_cKSplit(
        m,
        "_cKSplit",
        R"docstring(
        Implements k-split, an adaptive histogram-like density estimation
        algorithm. During result collection, k-split will dynamically subdivide
        "busy" bins (ones that collect a large number of observations), thereby
        refining the resolution of the histogram where needed.

        The histogram produced by k-split will be an approximation, because the
        algorithm has no information on how to divide up observations collected
        into a bin before it was split. Nevertheless, for stationary distributions,
        k-split usually produces a superior estimate of the distribution than
        uniform-bin histograms. If the distribution changes over time, and
        especially if it shows a distinct (increasing or decreasing) trend,
        k-split's estimate often contains artifacts that are not part of the input
        distribution.
        )docstring"
    );

    py_cKSplit.def(
        pybind11::init<const char *, bool>(),
        pybind11::arg("name") = nullptr, pybind11::arg("weighted") = false
    );

    py_cKSplit.def(
        "dup",
        &omnetpp::cKSplit::dup,
        R"docstring(
        Creates and returns an exact copy of this object.
        See cObject for more details.
        )docstring"
    );

    py_cKSplit.def(
        "setUpBins",
        &omnetpp::cKSplit::setUpBins,
        R"docstring(
        Transforms the table of pre-collected values into the k-split data structure.
        )docstring"
    );

    py_cKSplit.def(
        "getNumBins",
        &omnetpp::cKSplit::getNumBins,
        R"docstring(
        Returns the number of histogram cells used.
        )docstring"
    );

    py_cKSplit.def(
        "getBinEdge",
        &omnetpp::cKSplit::getBinEdge,
        R"docstring(
        Returns the number of histogram cells used.
        )docstring",
        pybind11::arg("k")
    );

    py_cKSplit.def(
        "getBinValue",
        &omnetpp::cKSplit::getBinValue,
        R"docstring(
        Returns the number of observations that fell into the kth histogram cell.
        )docstring",
        pybind11::arg("k")
    );

    py_cKSplit.def(
        "merge",
        &omnetpp::cKSplit::merge,
        R"docstring(
        Merging is not supported by this class. This method throws an error.
        )docstring"
    );

    py_cKSplit.def(
        "clear",
        &omnetpp::cKSplit::clear,
        R"docstring(
        Clears the results collected so far.
        )docstring"
    );

    py_cKSplit.def(
        "saveToFile",
        &omnetpp::cKSplit::saveToFile,
        R"docstring(
        Writes the contents of the object into a text file.
        )docstring",
        pybind11::arg("fh")
    );

    py_cKSplit.def(
        "loadFromFile",
        &omnetpp::cKSplit::loadFromFile,
        R"docstring(
        Reads the object data from a file, in the format written out by saveToFile().
        )docstring",
        pybind11::arg("fh")
    );

    py_cKSplit.def(
        "setRangeExtension",
        &omnetpp::cKSplit::setRangeExtension,
        R"docstring(
        Enables/disables range extension. If range extension is enabled,
        a new observation that falls outside the k-split range (i.e. outside
        the root grid) will cause the range to be expanded (i.e. new
        root getGrid(s) to be placed above the current root grid).
        If range extension is disabled, such observations will simply be
        counted as underflows or overflows.
        )docstring",
        pybind11::arg("enabled")
    );

    py_cKSplit.def(
        "getTreeDepth",
        pybind11::overload_cast<>(&omnetpp::cKSplit::getTreeDepth, pybind11::const_),
        R"docstring(
        Returns the depth of the k-split tree.
        )docstring"
    );

    py_cKSplit.def(
        "printGrids",
        &omnetpp::cKSplit::printGrids,
        R"docstring(
        Dumps the contents of the k-split data structure to EV.
        )docstring"
    );

    py_cKSplit.def(
        "collect",
        pybind11::overload_cast<double>(&omnetpp::cKSplit::collect),
        R"docstring(
        )docstring",
        pybind11::arg("value")
    );
}
