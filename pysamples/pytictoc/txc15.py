"""
This model is exciting enough so that we can collect some statistics.
We'll record in output vectors the hop count of every message upon arrival.
Output vectors are written into the omnetpp.vec file and can be visualized
with the Plove program.

We also collect basic statistics (min, max, mean, std.dev.) and histogram
about the hop count which we'll print out at the end of the simulation.
"""

from pyopp import cSimpleModule, cMessage, WATCH, cHistogram, cOutVector, EV


class TicTocMsg15(cMessage):
    def __init__(self, name, source, destination):
        super().__init__(name)
        self.source = source
        self.destination = destination
        self.hopcount = 0


class PyTxc15(cSimpleModule):

    def initialize(self):
        #  Initialize variables
        self.numSent = 0
        self.numReceived = 0
        WATCH('numSent')
        WATCH('numReceived')

        self.hopCountStats = cHistogram(name="hopCountStats")
        self.hopCountVector = cOutVector(name="HopCount")

        # Module 0 sends the first message
        if (self.getIndex() == 0):
            # Boot the process scheduling the initial message as a self-message.
            self.scheduleAt(0.0, self.generateMessage())

    def handleMessage(self, ttmsg):
        assert isinstance(ttmsg, TicTocMsg15)

        if ttmsg.destination == self.getIndex():
            # Message arrived
            hopcount = ttmsg.hopcount
            EV << "Message " << ttmsg << " arrived after " << hopcount << " hops.\n"
            self.bubble("ARRIVED, starting new one!")

            # update statistics.
            self.numReceived += 1
            self.hopCountVector.record(hopcount)
            self.hopCountStats.collect(hopcount)

            self.delete(ttmsg)

            # Generate another one.
            EV << "Generating another message: "
            newmsg = self.generateMessage()
            EV << newmsg << '\n'
            self.forwardMessage(newmsg)
            self.numSent += 1
        else:
            # We need to forward the message.
            self.forwardMessage(ttmsg)

    def generateMessage(self):
        # Produce source and destination addresses.
        src = self.getIndex()
        n = self.getVectorSize()
        dest = self.intuniform(0, n - 2)
        if dest >= src:
            dest += 1

        return TicTocMsg15('tic-{}-to-{}'.format(src, dest), src, dest)

    def forwardMessage(self, msg):
        # Increment hop count.
        msg.hopcount += 1

        # Same routing as before: random gate.
        n = self.gateSize("gate")
        k = self.intuniform(0, n - 1)

        EV << "Forwarding message " << msg << " on gate[" << k << "]\n"
        self.send(msg, "gate$o", k)

    def finish(self):
        # This function is called by OMNeT++ at the end of the simulation.
        EV << "Sent:     " << self.numSent << '\n'
        EV << "Received: " << self.numReceived << '\n'
        EV << "Hop count, min:    " << self.hopCountStats.getMin() << '\n'
        EV << "Hop count, max:    " << self.hopCountStats.getMax() << '\n'
        EV << "Hop count, mean:   " << self.hopCountStats.getMean() << '\n'
        EV << "Hop count, stddev: " << self.hopCountStats.getStddev() << '\n'

        self.recordScalar("#sent", self.numSent)
        self.recordScalar("#received", self.numReceived)

        self.hopCountStats.recordAs("hop count")
