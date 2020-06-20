#include <pybind11/pybind11.h>

#include <omnetpp.h>


void bind_cHistogramStrategy(pybind11::module &m)
{
    pybind11::class_<omnetpp::cIHistogramStrategy, omnetpp::cObject> py_cIHistogramStrategy(
        m,
        "_cIHistogramStrategy",
        R"docstring(
        Interface for histogram strategy classes. Histogram strategies
        encapsulate the task of setting up and managing the bins in a cHistogram.
        )docstring"
    );

    py_cIHistogramStrategy.def(
        "setHistogram",
        &omnetpp::cIHistogramStrategy::setHistogram,
        R"docstring(
        Set the cHistogram instance this strategy is associated with.
        This method is called from cHistogram::setStrategy().
        )docstring",
        pybind11::arg("hist")
    );

    py_cIHistogramStrategy.def(
        "getHistogram",
        &omnetpp::cIHistogramStrategy::getHistogram,
        R"docstring(
        Returns the cHistogram instance this strategy is associated with.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cIHistogramStrategy.def(
        "collect",
        &omnetpp::cIHistogramStrategy::collect,
        R"docstring(
        Called from cHistogram's collect() method. Implementations are expected to
        call back cHistogram's collectIntoHistogram(), or if histogram bins are
        not yet set up, store it for later.
        )docstring",
        pybind11::arg("value")
    );

    py_cIHistogramStrategy.def(
        "collectWeighted",
        &omnetpp::cIHistogramStrategy::collectWeighted,
        R"docstring(
        Called from cHistogram's collectWeighted() method. Implementations are
        expected to call back cHistogram's collectIntoHistogram(), or if histogram
        bins are not yet set up, store it for later.
        )docstring",
        pybind11::arg("value"), pybind11::arg("weight")
    );

    py_cIHistogramStrategy.def(
        "setUpBins",
        &omnetpp::cIHistogramStrategy::setUpBins,
        R"docstring(
        cHistogram's setUpBins() method delegates here. Implementations are expected
        to create bins in the associated histogram by calling its setBinEdges()
        or createUniformBins() method, and move possibly existing precollected
        observations into the bins using collectIntoHistogram().
        )docstring"
    );

    py_cIHistogramStrategy.def(
        "clear",
        &omnetpp::cIHistogramStrategy::clear,
        R"docstring(
        Called from cHistogram's clear() method, to give the strategy object an
        opportunity to clear its state.
        )docstring"
    );

    pybind11::class_<
        omnetpp::cFixedRangeHistogramStrategy,
        omnetpp::cIHistogramStrategy,
        omnetpp::cObject> py_cFixedRangeHistogramStrategy(
        m,
        "_cFixedRangeHistogramStrategy",
        R"docstring(
        Histogram strategy that sets up uniform bins over a predetermined
        interval. The number of bins and the histogram mode (integers or reals)
        also need to be configured. This strategy does not use precollection,
        as all input for setting up the bins must be explicitly provided by
        the user.

        Bins are set up when the first value is collected, and never change afterwards.
        If you need a histogram strategy that supports dynamically extending the
        histogram with new bins, consider using cAutoRangeHistogramStrategy.
        )docstring"
    );

    py_cFixedRangeHistogramStrategy.def(pybind11::init<>());
    py_cFixedRangeHistogramStrategy.def(
        pybind11::init<double, double, int, omnetpp::cHistogram::Mode>(),
        pybind11::arg("lo"), pybind11::arg("hi"), pybind11::arg("numBins"), pybind11::arg("mode")
    );

    py_cFixedRangeHistogramStrategy.def(
        "setRange",
        &omnetpp::cFixedRangeHistogramStrategy::setRange,
        pybind11::arg("lo"), pybind11::arg("hi")
    );

    py_cFixedRangeHistogramStrategy.def(
        "getUpperLimit",
        &omnetpp::cFixedRangeHistogramStrategy::getUpperLimit
    );

    py_cFixedRangeHistogramStrategy.def(
        "setUpperLimit",
        &omnetpp::cFixedRangeHistogramStrategy::setUpperLimit,
        pybind11::arg("hi")
    );

    py_cFixedRangeHistogramStrategy.def(
        "getLowerLimit",
        &omnetpp::cFixedRangeHistogramStrategy::getLowerLimit
    );

    py_cFixedRangeHistogramStrategy.def(
        "setLowerLimit",
        &omnetpp::cFixedRangeHistogramStrategy::setLowerLimit,
        pybind11::arg("lo")
    );

    py_cFixedRangeHistogramStrategy.def(
        "getMode",
        &omnetpp::cFixedRangeHistogramStrategy::getMode
    );

    py_cFixedRangeHistogramStrategy.def(
        "setMode",
        &omnetpp::cFixedRangeHistogramStrategy::setMode,
        pybind11::arg("mode")
    );

    py_cFixedRangeHistogramStrategy.def(
        "getBinSize",
        &omnetpp::cFixedRangeHistogramStrategy::getBinSize
    );

    py_cFixedRangeHistogramStrategy.def(
        "setBinSize",
        &omnetpp::cFixedRangeHistogramStrategy::setBinSize,
        pybind11::arg("binSize")
    );

    pybind11::class_<
        omnetpp::cPrecollectionBasedHistogramStrategy,
        omnetpp::cIHistogramStrategy> py_cPrecollectionBasedHistogramStrategy(
        m,
        "_cPrecollectionBasedHistogramStrategy",
        R"docstring(
        Base class for histogram strategies that employ a precollection phase
        in order to gather input for setting up the bins. This class provides
        storage for the precollected values, and also a built-in algorithm for
        deciding when to stop precollection.
        )docstring"
    );

    py_cPrecollectionBasedHistogramStrategy.def(
        "getNumToPrecollect",
        &omnetpp::cPrecollectionBasedHistogramStrategy::getNumToPrecollect
    );

    py_cPrecollectionBasedHistogramStrategy.def(
        "setNumToPrecollect",
        &omnetpp::cPrecollectionBasedHistogramStrategy::setNumToPrecollect,
        pybind11::arg("numToPrecollect")
    );

    py_cPrecollectionBasedHistogramStrategy.def(
        "getNumToCollate",
        &omnetpp::cPrecollectionBasedHistogramStrategy::getNumToCollate
    );

    py_cPrecollectionBasedHistogramStrategy.def(
        "setNumToCollate",
        &omnetpp::cPrecollectionBasedHistogramStrategy::setNumToCollate,
        pybind11::arg("numToCollate")
    );

    py_cPrecollectionBasedHistogramStrategy.def(
        "getRangeUnchangedThreshold",
        &omnetpp::cPrecollectionBasedHistogramStrategy::getRangeUnchangedThreshold
    );

    py_cPrecollectionBasedHistogramStrategy.def(
        "setRangeUnchangedThreshold",
        &omnetpp::cPrecollectionBasedHistogramStrategy::setRangeUnchangedThreshold,
        pybind11::arg("threshold")
    );

    pybind11::class_<
        omnetpp::cDefaultHistogramStrategy,
        omnetpp::cPrecollectionBasedHistogramStrategy> py_cDefaultHistogramStrategy(
        m,
        "_cDefaultHistogramStrategy",
        R"docstring(
        @brief A strategy class used by the default setup of cHistogram. It is
        meant to provide a good quality uniform-bin histogram without requiring
        manual configuration.

        This strategy uses precollection to gather input information about the
        distribution before setting up the bins. Precollection is used to determine
        the initial histogram range and the histogram mode (integers vs. reals).
        In integers mode, bin edges will be whole numbers.

        To keep up with distributions that change over time, this histogram strategy
        can auto-extend the histogram range by adding new bins as needed. It also
        performs bin merging when necessary, to keep the number of bins reasonably low.

        Collected infinities are counted as underflows/overflows, and they do not
        affect the histogram layout in any way.
        )docstring"
    );

    py_cDefaultHistogramStrategy.def(
        pybind11::init<int, omnetpp::cHistogram::Mode>(),
        pybind11::arg("numBinsHint") = -1, pybind11::arg("mode") = omnetpp::cHistogram::MODE_AUTO
    );

    py_cDefaultHistogramStrategy.def(
        "getNumBinsHint",
        &omnetpp::cDefaultHistogramStrategy::getNumBinsHint
    );

    py_cDefaultHistogramStrategy.def(
        "setNumBinsHint",
        &omnetpp::cDefaultHistogramStrategy::setNumBinsHint,
        pybind11::arg("numBins")
    );

    pybind11::class_<
        omnetpp::cAutoRangeHistogramStrategy,
        omnetpp::cPrecollectionBasedHistogramStrategy> py_cAutoRangeHistogramStrategy(
        m,
        "_cAutoRangeHistogramStrategy",
        R"docstring(
        A generic, very configurable histogram strategy that is meant to provide
        a good quality histogram for practical distributions, and creates uniform bins.
        This strategy uses precollection to gather input information about the
        distribution before setting up the bins.

        Several parameters and constraints can be specified for setting up the bins:
        range lower and/or upper endpoint, bin size, number of bins, mode (integers
        or reals), and whether bin size rounding is to be used. If bin size rounding
        is turned on, a bin size of the form {1|2|5}*10^n is chosen, i.e. a power of ten,
        or two or five times a power of ten. When both endpoints of the range are
        left unspecified, the histogram range is derived by taking the range of the
        precollected observations and extending it symmetrically by a range extension
        factor. If one of the endpoints is specified by the user, that value is used
        instead of the computed one. The number of observations to precollect as well
        as the range extension factor can be configured. If the histogram mode
        (integers vs. reals) is left unspecified, it will be determined by examining
        the precollected values. In integers mode, bin edges will be whole numbers.
        If the histogram range and mode are all specified by the user, the
        precollection phase is skipped.

        If there are inconsistent or conflicting settings, e.g. an explicitly given
        histogram range is not multiple of the bin size in integers mode, the bin
        setup algorithm will do "best effort" to set up the histogram instead of
        stopping with an exception.

        This histogram strategy can auto-extend the histogram range by adding new
        bins at either end. One can also set up an upper limit to the number of
        histogram bins to prevent it from growing indefinitely. Bin merging can
        also be enabled: it will cause every two (or N) adjacent bins to be
        merged to reduce the number of bins if their number grows too high.

        Collected infinities are counted as underflows/overflows, and they do not
        affect the histogram layout in any way.
        )docstring"
    );

    py_cAutoRangeHistogramStrategy.def(
        pybind11::init<omnetpp::cHistogram::Mode>(),
        pybind11::arg("mode") = omnetpp::cHistogram::MODE_AUTO
    );

    py_cAutoRangeHistogramStrategy.def(
        pybind11::init<int, omnetpp::cHistogram::Mode>(),
        pybind11::arg("numBins"), pybind11::arg("mode") = omnetpp::cHistogram::MODE_AUTO
    );

    py_cAutoRangeHistogramStrategy.def(
        "setRangeHint",
        &omnetpp::cAutoRangeHistogramStrategy::setRangeHint,
        pybind11::arg("lo"), pybind11::arg("hi")
    );

    py_cAutoRangeHistogramStrategy.def(
        "getUpperLimitHint",
        &omnetpp::cAutoRangeHistogramStrategy::getUpperLimitHint
    );

    py_cAutoRangeHistogramStrategy.def(
        "setUpperLimitHint",
        &omnetpp::cAutoRangeHistogramStrategy::setUpperLimitHint,
        pybind11::arg("hi")
    );

    py_cAutoRangeHistogramStrategy.def(
        "getLowerLimitHint",
        &omnetpp::cAutoRangeHistogramStrategy::getLowerLimitHint
    );

    py_cAutoRangeHistogramStrategy.def(
        "setLowerLimitHint",
        &omnetpp::cAutoRangeHistogramStrategy::setLowerLimitHint,
        pybind11::arg("lo")
    );

    py_cAutoRangeHistogramStrategy.def(
        "getRangeExtensionFactor",
        &omnetpp::cAutoRangeHistogramStrategy::getRangeExtensionFactor
    );

    py_cAutoRangeHistogramStrategy.def(
        "setRangeExtensionFactor",
        &omnetpp::cAutoRangeHistogramStrategy::setRangeExtensionFactor,
        pybind11::arg("rangeExtensionFactor")
    );

    py_cAutoRangeHistogramStrategy.def(
        "getMode",
        &omnetpp::cAutoRangeHistogramStrategy::getMode
    );

    py_cAutoRangeHistogramStrategy.def(
        "setMode",
        &omnetpp::cAutoRangeHistogramStrategy::setMode,
        pybind11::arg("mode")
    );

    py_cAutoRangeHistogramStrategy.def(
        "getBinSizeHint",
        &omnetpp::cAutoRangeHistogramStrategy::getBinSizeHint
    );

    py_cAutoRangeHistogramStrategy.def(
        "setBinSizeHint",
        &omnetpp::cAutoRangeHistogramStrategy::setBinSizeHint,
        pybind11::arg("binSize")
    );

    py_cAutoRangeHistogramStrategy.def(
        "getBinSizeRounding",
        &omnetpp::cAutoRangeHistogramStrategy::getBinSizeRounding
    );

    py_cAutoRangeHistogramStrategy.def(
        "setBinSizeRounding",
        &omnetpp::cAutoRangeHistogramStrategy::setBinSizeRounding,
        pybind11::arg("binSizeRounding")
    );

    py_cAutoRangeHistogramStrategy.def(
        "getNumBinsHint",
        &omnetpp::cAutoRangeHistogramStrategy::getNumBinsHint
    );

    py_cAutoRangeHistogramStrategy.def(
        "setNumBinsHint",
        &omnetpp::cAutoRangeHistogramStrategy::setNumBinsHint,
        pybind11::arg("numBins")
    );

    py_cAutoRangeHistogramStrategy.def(
        "setAutoExtend",
        &omnetpp::cAutoRangeHistogramStrategy::setAutoExtend,
        pybind11::arg("enable")
    );

    py_cAutoRangeHistogramStrategy.def(
        "getAutoExtend",
        &omnetpp::cAutoRangeHistogramStrategy::getAutoExtend
    );

    py_cAutoRangeHistogramStrategy.def(
        "setBinMerging",
        &omnetpp::cAutoRangeHistogramStrategy::setBinMerging,
        pybind11::arg("enable")
    );

    py_cAutoRangeHistogramStrategy.def(
        "getBinMerging",
        &omnetpp::cAutoRangeHistogramStrategy::getBinMerging
    );

    py_cAutoRangeHistogramStrategy.def(
        "getMaxNumBins",
        &omnetpp::cAutoRangeHistogramStrategy::getMaxNumBins
    );

    py_cAutoRangeHistogramStrategy.def(
        "setMaxNumBins",
        &omnetpp::cAutoRangeHistogramStrategy::setMaxNumBins,
        pybind11::arg("numBins")
    );
}
