#include <pybind11/pybind11.h>

#include <omnetpp.h>
#include <omnetpp/chistogram.h>

void bind_cHistogram(pybind11::module &m)
{
    pybind11::class_<
        omnetpp::cHistogram,
        omnetpp::cAbstractHistogram,
        omnetpp::cStdDev,
        omnetpp::cStatistic,
        omnetpp::cNamedObject,
        omnetpp::cOwnedObject,
        omnetpp::cObject> py_cHistogram(
        m,
        "_cHistogram",
        R"docstring(
        Generic histogram class, capable of representing both unweighted and
        weighted distributions. Histogram data are stored as n+1 bin edges and
        n bin values, both being double-precision floating point values. Upper and
        lower outliers (as well as positive and negative infinities) are kept as counts
        (for unweighted statistics) or as sum of weights (for weighted statistics).

        cHistogram is able to generate random numbers from the stored distribution,
        and also supports save/load of the histogram data in a file.

        Bins can be set up directly using methods such as setBinEdges() or
        createUniformBins(), but it is often more practical to automate it by letting
        a <i>histogram strategy</i> do it. Histogram strategies are objects that
        encapsulate the job of setting up and managing histogram bins. For example,
        histogram strategies may employ a precollection phase to get an estimate of
        the distribution for laying out the bins. Strategies are also capable of
        extending the histogram range later by adding new bins as needed, or merging
        adjacent bins to reduce their count. Strategies that create non-uniform
        (e.g. equi-probable) bins are also possible to implement. Histogram
        strategies subclass from cIHistogramStrategy.

        The default constructor of cHistogram installs a default histogram strategy
        which was designed to provide a good quality histogram for arbitrary
        distributions, without manual configuration. It employs precollection
        and also auto-extends the histogram at runtime, merging neighbouring groups
        of bins if there would be too many of them after extension.

        Custom behavior can be achieved by setting up and installing an appropriate
        strategy class, such as cFixedRangeHistogramStrategy or cAutoRangeHistogramStrategy.
        There are also convenience methods such as setRange() and setNumBins()
        that internally use cAutoRangeHistogramStrategy.

        Examples:

        Automatic mode:

        .. code::
            cHistogram histogram("histogram");

        Setting up a 50-bin histogram on the range [0,100):

        .. code::
            cAutoRangeHistogramStrategy *strategy = new cAutoRangeHistogramStrategy();
            strategy->setRange(0, 100);
            strategy->setNumBins(50);
            strategy->setMode(cHistogram::MODE_INTEGERS);
            cHistogram histogram("histogram", strategy);

        The same effect using convenience methods:

        .. code::
            cHistogram histogram("histogram");
            histogram.setRange(0, 100);
            histogram.setNumBins(50);
            histogram.setMode(cHistogram::MODE_INTEGERS);
        )docstring"
    );

    pybind11::enum_<omnetpp::cHistogram::Mode>(
        py_cHistogram,
        "Mode",
        R"docstring(
        Histogram mode. In INTEGERS mode, bin edges are whole numbers; in REALS mode
        they can be real numbers.
        )docstring")
        .value("MODE_AUTO", omnetpp::cHistogram::Mode::MODE_AUTO)
        .value("MODE_INTEGERS", omnetpp::cHistogram::Mode::MODE_INTEGERS)
        .value("MODE_REALS", omnetpp::cHistogram::Mode::MODE_REALS)
        .export_values();

    py_cHistogram.def(
        pybind11::init<const char *, bool >(),
        R"docstring(
        This constructor installs a cDefaultHistogramStrategy on the histogram.
        To create a histogram without a strategy object, use the constructor
        that takes a 'strategy' parameter and pass nullptr for it.
        )docstring",
        pybind11::arg("name"), pybind11::arg("weighted") = false
    );

    py_cHistogram.def(
        pybind11::init<const char *, int, bool >(),
        R"docstring(
        This convenience constructor installs a cDefaultHistogramStrategy
        with the desired number of bins on the histogram.
        )docstring",
        pybind11::arg("name"), pybind11::arg("numBinsHint"), pybind11::arg("weighted") = false
    );

    py_cHistogram.def(
        pybind11::init<const char *, omnetpp::cIHistogramStrategy *, bool >(),
        R"docstring(
        Constructor that allows one to install a histogram strategy on the
        histogram. It is also legal to pass None as strategy.
        )docstring",
        pybind11::arg("name"), pybind11::arg("strategy"), pybind11::arg("weighted") = false
    );

    py_cHistogram.def(
        "collect",
        pybind11::overload_cast<double>(&omnetpp::cHistogram::collect),
        R"docstring(
        Collects one observation with a given weight. The weight must not be
        negative. (Zero-weight observations are allowed, but will not affect
        mean and stddev, nor the bin values.) This methods delegates to the strategy
        object if there is one installed.
        )docstring",
        pybind11::arg("value")
    );

    py_cHistogram.def(
        "collectWeighted",
        pybind11::overload_cast<double, double>(&omnetpp::cHistogram::collectWeighted),
        R"docstring(
        Collects one observation with a given weight. The weight must not be
        negative. (Zero-weight observations are allowed, but will not affect
        mean and stddev, nor the bin values.) This methods delegates to the strategy
        object if there is one installed.
        )docstring",
        pybind11::arg("value"), pybind11::arg("weight")
    );

    py_cHistogram.def(
        "setStrategy",
        &omnetpp::cHistogram::setStrategy,
        R"docstring(
        Installs a new histogram strategy, replacing the current one, taking ownership
        of 'setupStrategy'. Can only be called while the histogram is empty.
        )docstring",
        pybind11::arg("strategy")
    );

    py_cHistogram.def(
        "getStrategy",
        &omnetpp::cHistogram::getStrategy,
        R"docstring(
        Returns a pointer to the currently used histogram strategy.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cHistogram.def(
        "binsAlreadySetUp",
        &omnetpp::cHistogram::binsAlreadySetUp,
        R"docstring(
        Returns true if histogram bins are already available. (Bins are not yet
        available in the precollection phase.) See setUpBins().
        )docstring"
    );

    py_cHistogram.def(
        "setUpBins",
        &omnetpp::cHistogram::setUpBins,
        R"docstring(
        Sets up histogram bins using the installed strategy (see cIHistogramStrategy).
        The histogram strategy class may use precollection to gather information
        for laying out the bins.
        )docstring"
    );

    py_cHistogram.def(
        "setBinEdges",
        &omnetpp::cHistogram::setBinEdges,
        R"docstring(
        Configures a histogram with bins defined by 'edges'.
        Can only be called once, and only when there are no bins defined.
        edges' must contain at least two values, and it must be strictly increasing.
        )docstring",
        pybind11::arg("edges")
    );

    py_cHistogram.def(
        "createUniformBins",
        &omnetpp::cHistogram::createUniformBins,
        R"docstring(
        Sets the histogram up to have bins covering the range from 'lo' to 'hi',
        each bin being 'step' wide. Can only be called on a histogram without bins.
        'lo' will always be added as an edge, all bins will be 'step' wide, and
        the last bin edge will be at or above 'hi'.
        )docstring",
        pybind11::arg("lo"), pybind11::arg("hi"), pybind11::arg("step")
    );

    py_cHistogram.def(
        "prependBins",
        &omnetpp::cHistogram::prependBins,
        R"docstring(
        Extends the histogram to the left with some bins, as delimited by 'edges'.
        This can only be used if there is at least one bin already, and there are
        no underflows. 'edges' must not be empty, it must be strictly increasing,
        and its last value must be less than the first already existing bin edge.
        )docstring",
        pybind11::arg("edges")
    );

    py_cHistogram.def(
        "appendBins",
        &omnetpp::cHistogram::appendBins,
        R"docstring(
        Extends the histogram to the right with some bins, as delimited by 'edges'.
        This can only be used if there is at least one bin already, and there are
        no overflows. 'edges' must not be empty, it must be strictly increasing,
        and its first value must be greater than the last already existing bin edge.
        )docstring",
        pybind11::arg("edges")
    );

    py_cHistogram.def(
        "extendBinsTo",
        &omnetpp::cHistogram::extendBinsTo,
        R"docstring(
        Makes sure that 'value' will fall in the range covered by the bins, by
        potentially extending the histogram with some bins of width 'step'.
        Creation of new bins stops when either the total number of bins reaches
        'maxNumBins', or 'value' is covered by the histogram. If 'value' is already
        * in the range of the existing bins, the function does nothing.
        This method may only be called when there are no over- or underflows.
        'step' must be positive.
        )docstring",
        pybind11::arg("value"), pybind11::arg("step"), pybind11::arg("maxNumBins") = INT_MAX
    );

    py_cHistogram.def(
        "mergeBins",
        &omnetpp::cHistogram::mergeBins,
        R"docstring(
        Reduces the number of bins by merging each 'groupSize' consecutive bins into one.
        The number of bins must be a multiple of 'groupSize'. If that is not the case,
        extendBinsTo() or similar methods may be used to create extra empty bins
        before calling this function.
        )docstring",
        pybind11::arg("groupSize")
    );

    py_cHistogram.def(
        "getBinEdges",
        &omnetpp::cHistogram::getBinEdges,
        R"docstring(
        Returns the bin edges of the histogram. There is always one more edge than bin,
        except when the histogram has not been set up yet, in which case both are zero.
        )docstring",
        pybind11::return_value_policy::reference
    );

    py_cHistogram.def(
        "getBinValues",
        &omnetpp::cHistogram::getBinValues,
        R"docstring(
        Returns the bin values of the histogram. There is always one less bin than edge,
        except when the histogram has not been set up yet, in which case both are zero.
        )docstring"
    );

    py_cHistogram.def(
        "getNumBins",
        &omnetpp::cHistogram::getNumBins,
        R"docstring(
        Returns the number of bins in the histogram.
        )docstring"
    );

    py_cHistogram.def(
        "getBinEdge",
        &omnetpp::cHistogram::getBinEdge,
        R"docstring(
        Returns the k'th bin edge of the histogram. The k'th bin is delimited by the edge k and k+1.
        )docstring",
        pybind11::arg("k")
    );

    py_cHistogram.def(
        "getBinValue",
        &omnetpp::cHistogram::getBinValue,
        R"docstring(
        Returns the total weight of the observations in the k'th bin of the histogram.
        (In the unweighted case, every observation is regarded as having the weight 1.0.)
        )docstring",
        pybind11::arg("k")
    );

    py_cHistogram.def(
        "getUnderflowSumWeights",
        &omnetpp::cHistogram::getUnderflowSumWeights,
        R"docstring(
        Returns the total weight of the observations that were under the histogram range.
        )docstring"
    );

    py_cHistogram.def(
        "getOverflowSumWeights",
        &omnetpp::cHistogram::getOverflowSumWeights,
        R"docstring(
        Returns the total weight of the observations that were above the histogram range.
        )docstring"
    );

    py_cHistogram.def(
        "getNumUnderflows",
        &omnetpp::cHistogram::getNumUnderflows,
        R"docstring(
        Returns the number of observations that were under the histogram range.
        This value is only collected for unweighted statistics, i.e. it is an
        error to call this method on a weighted histogram.
        )docstring"
    );

    py_cHistogram.def(
        "getNumOverflows",
        &omnetpp::cHistogram::getNumOverflows,
        R"docstring(
        Returns the number of observations that were under the histogram range.
        This value is only collected for unweighted statistics, i.e. it is an
        error to call this method on a weighted histogram.
        )docstring"
    );

    py_cHistogram.def(
        "getNumNegInfs",
        &omnetpp::cHistogram::getNumNegInfs,
        R"docstring(
        Returns number of observations that were negative infinity, independent of their weights.
        This value is only collected for unweighted statistics, i.e. it is an
        error to call this method on a weighted histogram.
        )docstring"
    );

    py_cHistogram.def(
        "getNumPosInfs",
        &omnetpp::cHistogram::getNumPosInfs,
        R"docstring(
        Returns number of observations that were positive infinity, independent of their weights.
        This value is only collected for unweighted statistics, i.e. it is an
        error to call this method on a weighted histogram.
        )docstring"
    );

    py_cHistogram.def(
        "getNegInfSumWeights",
        &omnetpp::cHistogram::getNegInfSumWeights,
        R"docstring(
        Returns the total weight of the observations that were negative infinity.
        )docstring"
    );

    py_cHistogram.def(
        "getPosInfSumWeights",
        &omnetpp::cHistogram::getPosInfSumWeights,
        R"docstring(
        Returns the total weight of the observations that were above the histogram range.
        )docstring"
    );

    py_cHistogram.def(
        "setMode",
        &omnetpp::cHistogram::setMode,
        R"docstring(
        Sets the histogram mode: MODE_AUTO, MODE_INTEGERS or MODE_DOUBLES.
        Cannot be called when the bins have been set up already.
        This method internally installs (or uses an already installed)
        cAutoRangeHistogramStrategy on the histogram, and configures it accordingly.
        )docstring",
        pybind11::arg("mode")
    );

    py_cHistogram.def(
        "setRange",
        &omnetpp::cHistogram::setRange,
        R"docstring(
        Sets the histogram range explicitly to [lower, upper). Use NAN to leave
        either or both values unspecified. This method internally installs
        (or uses an already installed) cAutoRangeHistogramStrategy on the
        histogram, and configures it accordingly.
        )docstring",
        pybind11::arg("lower"), pybind11::arg("upper")
    );

    py_cHistogram.def(
        "setNumPrecollectedValues",
        &omnetpp::cHistogram::setNumPrecollectedValues,
        R"docstring(
        Sets the number of observations to collect before setting up histogram bins.
        This method internally installs (or uses an already installed)
        cAutoRangeHistogramStrategy on the histogram, and configures it accordingly.
        )docstring",
        pybind11::arg("numPrecollect")
    );

    py_cHistogram.def(
        "setRangeExtensionFactor",
        &omnetpp::cHistogram::setRangeExtensionFactor,
        R"docstring(
        Sets the factor by which the range of the precollected observations
        is multiplied for determining the range of the histogram bins.
        This method internally installs (or uses an already installed)
        cAutoRangeHistogramStrategy on the histogram, and configures it accordingly.
        )docstring",
        pybind11::arg("rangeExtensionFactor")
    );

    py_cHistogram.def(
        "setAutoExtend",
        &omnetpp::cHistogram::setAutoExtend,
        R"docstring(
        When set to true, observations that fall outside of the histogram range
        will cause the histogram to be extended with new bins, instead of being
        collected as outliers.
        This method internally installs (or uses an already installed)
        cAutoRangeHistogramStrategy on the histogram, and configures it accordingly.
        )docstring",
        pybind11::arg("autoExtend")
    );

    py_cHistogram.def(
        "setNumBinsHint",
        &omnetpp::cHistogram::setNumBinsHint,
        R"docstring(
        Sets the preferred number of bins. Cannot be called when the bins have been
        set up already. This method internally installs (or uses an already installed)
        cAutoRangeHistogramStrategy on the histogram, and configures it accordingly.
        )docstring",
        pybind11::arg("numCells")
    );

    py_cHistogram.def(
        "setBinSizeHint",
        &omnetpp::cHistogram::setBinSizeHint,
        R"docstring(
        Sets the preferred bin size. Cannot be called when the bins have been
        set up already. This method internally installs (or uses an already installed)
        cAutoRangeHistogramStrategy on the histogram, and configures it accordingly.
        )docstring",
        pybind11::arg("d")
    );
}
