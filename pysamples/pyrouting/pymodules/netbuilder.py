from pyopp import (
    cSimpleModule,
    cMessage,
    cDatarateChannel,
    cModuleType,
    EV,
)


class NetBuilder(cSimpleModule):
    """
    Builds a network dynamically, with the topology coming from a
    text file.
    """
    def initialize(self):
        # build the network in event 1, because it is undefined whether the simkernel
        # will implicitly initialize modules created *during* initialization, or this needs
        # to be done manually.
        self.scheduleAt(0, cMessage())

    def handleMessage(self, msg):
        assert msg.isSelfMessage(), "This module does not process messages."
        self.delete(msg)
        msg = None
        self.buildNetwork(self.getParentModule())

    def connect(self, src, dest, delay, ber, datarate):
        channel = None
        if delay > 0 or ber > 0 or datarate > 0:
            channel = cDatarateChannel.create("channel")
            if delay > 0:
                channel.setDelay(delay)
            if ber > 0:
                channel.setBitErrorRate(ber)
            if datarate > 0:
                channel.setDatarate(datarate)
        src.connectTo(dest, channel)

    def buildNetwork(self, parent):
        nodeid2mod = {}
        line = None

        with open(self.par("nodesFile").stringValue(), 'r') as fh:
            for line in fh.readlines():
                line = line.strip()
                if not line or line.startswith('#'):
                    continue

                nodeid, name, modtypename = line.split()
                nodeid = int(nodeid)

                EV << "NODE id=" << nodeid << " name=" << name << " type=" << modtypename << "\n"

                # create module
                modtype = cModuleType.find(modtypename)
                if modtype is None:
                    msg = "module type `%s' for node `%s' not found" % (modtypename, name)
                    raise RuntimeError(msg)

                mod = modtype.create(name, parent)
                nodeid2mod[nodeid] = mod

                # read params from the ini file, etc
                mod.finalizeParameters()

        #/ read and create connections
        with open(self.par("connectionsFile").stringValue(), 'r') as fh:
            for line in fh.readlines():
                line = line.strip()
                if not line or line.startswith('#'):
                    continue

                srcnodeid, destnodeid, delay, error, datarate = line.split()
                srcnodeid = int(srcnodeid)
                destnodeid = int(destnodeid)
                delay = float(delay) if delay != '-' else -1
                error = float(error) if error != '-' else -1
                datarate = float(datarate) if datarate != '-' else -1

                try:
                    srcmod = nodeid2mod[srcnodeid]
                    destmod = nodeid2mod[destnodeid]
                except KeyError as e:
                    msg = "wrong line in connections file: node with id=%ld not found" % str(e)
                    raise RuntimeError(msg) from e

                # cGate
                srcIn, srcOut = srcmod.getOrCreateFirstUnconnectedGatePair("port", False, True)
                destIn, destOut = destmod.getOrCreateFirstUnconnectedGatePair("port", False, True)

                # connect
                self.connect(srcOut, destIn, delay, error, datarate)
                self.connect(destOut, srcIn, delay, error, datarate)

        # final touches: buildinside, initialize()
        for mod in nodeid2mod.values():
            mod.buildInside()

        # multi-stage init
        more = True
        stage = 0
        while more:
            for mod in nodeid2mod.values():
                more = mod.callInitialize(stage)
            stage += 1
