from pyopp import (
    EV,
    cSimpleModule,
    cTopology
)


class Routing(cSimpleModule):
    """Demonstrates static routing, utilizing the cTopology class."""

    def __init__(self):
        cSimpleModule.__init__(self)
        self.myAddress = None

        # destaddr -> gateindex
        self.rtable = {}

        # signals
        self.dropSignal = None
        self.outputIfSignal = None

    def initialize(self):
        self.myAddress = self.getParentModule().par("address").intValue()
        self.dropSignal = self.registerSignal("drop")
        self.outputIfSignal = self.registerSignal("outputIf")

        # Brute force approach -- every node does topology discovery on its own,
        # and finds routes to all other nodes independently, at the beginning
        # of the simulation. This could be improved: (1) central routing database,
        # (2) on-demand route calculation

        topo = cTopology("topo")
        nodeNames = [self.getParentModule().getNedTypeName()]
        topo.extractByNedTypeName(nodeNames)
        EV << "cTopology found " << topo.getNumNodes() << " nodes\n"

        thisNode = topo.getNodeFor(self.getParentModule())

        # find and store next hops
        for i in range(topo.getNumNodes()):
            if topo.getNode(i) == thisNode:
                continue  # skip ourselves
            topo.calculateUnweightedSingleShortestPathsTo(topo.getNode(i))

            if thisNode.getNumPaths() == 0:
                continue  # not connected

            parentModuleGate = thisNode.getPath(0).getLocalGate()
            gateIndex = parentModuleGate.getIndex()
            address = topo.getNode(i).getModule().par("address").intValue()
            self.rtable[address] = gateIndex
            EV << "  towards address " << address << " gateIndex is " << gateIndex << '\n'

    def handleMessage(self, pk):
        destAddr = pk.destAddr

        if destAddr == self.myAddress:
            EV << "local delivery of packet " << pk.getName() << '\n'
            self.send(pk, "localOut")
            self.emit(self.outputIfSignal, -1)  # -1: local
            return

        outGateIndex = self.rtable.get(destAddr)

        if outGateIndex is None:
            EV << "address {}  unreachable, discarding packet {}\n'".format(destAddr, pk.getName())
            self.emit(self.dropSignal, pk.getByteLength())
            self.delete(pk)
            pk = None
            return

        EV << "forwarding packet {} on gate index {}\n".format(pk.getName(), outGateIndex)
        pk.hopCount += 1
        self.emit(self.outputIfSignal, outGateIndex)

        self.send(pk, "out", outGateIndex)
