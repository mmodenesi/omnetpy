from pyopp import (
    EV,
    cAutoRangeHistogramStrategy,
    cDefaultHistogramStrategy,
    cFixedRangeHistogramStrategy,
    cHistogram,
    cKSplit,
    cMessage,
    cOutVector,
    cPSquare,
    cSimpleModule,
    simTime,
)


class PyHistograms(cSimpleModule):
    """Exercise histogram classes; see NED file for more info."""

    def __init__(self):
        cSimpleModule.__init__(self)

        # int
        self.count = None

        # bool
        self.weighted = False

        # list of cAbstractHistogram
        self.statisticObjects = []

        self.valuesVector = cOutVector()
        self.weightsVector = cOutVector()

        # simsignal_t
        self.unweightedValueSignal = None

    def addHistogram(self, name, strategy):
        self.statisticObjects.append(cHistogram(name, strategy, self.weighted))

    def createStatisticObjects(self):
        if not self.weighted:
            self.statisticObjects.append(cPSquare("psquare-default"))
            self.statisticObjects.append(cPSquare("psquare bins=100", 100))

        self.statisticObjects.append(cKSplit("ksplit-default", self.weighted))

        self.addHistogram("Default", cDefaultHistogramStrategy())
        self.addHistogram("Default bins=5", cDefaultHistogramStrategy(5))

        self.addHistogram("FixedRange [-10,10), bins=40, reals",
                          cFixedRangeHistogramStrategy(-10, 10, 40, cHistogram.MODE_REALS))

        self.addHistogram("AutoRange default", cAutoRangeHistogramStrategy())
        self.addHistogram("AutoRange bins=100, integers", cAutoRangeHistogramStrategy(100, cHistogram.MODE_INTEGERS))

        rangeStrategy = cAutoRangeHistogramStrategy()
        rangeStrategy.setRangeHint(5, float('nan'))
        self.addHistogram("AutoRange [5,auto)", rangeStrategy)

        binSizeStrategy = cAutoRangeHistogramStrategy()
        binSizeStrategy.setBinSizeHint(2)
        binSizeStrategy.setBinSizeRounding(False)
        self.addHistogram("AutoRange binSize=2", binSizeStrategy)

        fixedBinsStrategy = cAutoRangeHistogramStrategy()
        fixedBinsStrategy.setRangeHint(5, 15)
        fixedBinsStrategy.setNumBinsHint(5)
        fixedBinsStrategy.setAutoExtend(False)
        self.addHistogram("AutoRange [5,15) numBins=5 autoExtend=false)", fixedBinsStrategy)

    def initialize(self):
        self.valuesVector.setName("values")
        self.weightsVector.setName("weights")
        self.count = self.par("count").intValue()
        self.weighted = self.par("weighted").boolValue()
        self.createStatisticObjects()

        self.unweightedValueSignal = self.registerSignal("unweightedValue")

        EV << "Created " << len(self.statisticObjects) << " statistics objects.\n\n"

        EV << ("If Qtenv is used (it is the default), you may click the Children mode\n"
               "button of the object inspector, and double-click the items to open\n"
               "graphical inspectors for the distributions.\n\n")

        EV << "Collecting variables from: " << self.par("variable").str() << "\n"

        self.scheduleAt(0, cMessage())

    def handleMessage(self, msg):
        if (self.statisticObjects[0].getCount() == self.count):
            EV << "done" << '\n'
            self.delete(msg)
            msg = None
            return

        if not self.weighted:
            value = self.par("variable").doubleValue()
            EV << "collecting value=" << value << '\n'

            self.valuesVector.record(value);
            self.emit(self.unweightedValueSignal, value)

            for statistic in self.statisticObjects:
                statistic.collect(value)
        else:
            value = par("variable").doubleValue()
            weight = par("weight").doubleValue()
            EV << "collecting value=" << value << " with weight=" << weight << '\n'

            self.valuesVector.record(value)
            self.weightsVector.record(weight)
            self.emit(self.unweightedValueSignal, value)

            for statistic in self.statisticObjects:
                statistic.collectWeighted(value, weight)

        self.scheduleAt(simTime() + 0.001, msg)

    def finish(self):
        stat = self.statisticObjects[0]
        EV << "Bins in " << stat.getName() << ":\n"
        for i in range(stat.getNumBins()):
            EV << " bin " << i << ":"
            EV << " [" << stat.getBinEdge(i) << "," << stat.getBinEdge(i + 1) << ") "
            EV << "  w=" << stat.getBinValue(i) << " pdf=" << stat.getBinPDF(i) << '\n'

        EV << "Writing snapshot file...\n"
        self.snapshot(self)

        EV << "Recording histograms...\n"
        for statistic in self.statisticObjects:
            statistic.record()

    def __del__(self):
        """
        for (auto stat : statisticObjects)
            delete stat;
        """
