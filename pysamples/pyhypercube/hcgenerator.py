from pyopp import (
    cSimpleModule,
    EV,
    cMessage,
    simTime,
)

from packets import HCPacket


class HCGenerator(cSimpleModule):

    def initialize(self):
        self.iaTime = self.par("iaTime")
        self.numStations = self.par("numStations").intValue()
        self.myAddress = self.par("address").intValue()
        self.event = cMessage('event')

        # send self message to start activity
        self.scheduleAt(simTime() + self.iaTime.doubleValue(), self.event)

    def finish(self):
        if not self.event.isScheduled():
            self.delete(self.event)
            self.event = None

    def handleMessage(self, msg):
        assert msg is self.event

        dest = self.intrand(self.numStations - 1);
        if dest >= self.myAddress:
            dest += 1

        # create packet
        pkt = HCPacket('{}-->{}'.format(self.myAddress, dest))
        pkt.srcAddress = self.myAddress
        pkt.destAddress = dest
        pkt.hops = 0
        pkt.setTimestamp()
        EV << "Generated " << pkt.getName() << '\n';

        self.send(pkt, "out");

        # wait between messages
        #
        # Note that iaTime is a reference to the module parameter "iaTime"
        # that will be evaluated here. The module parameter can also take
        # a random value (for example: truncnormal(0.5,0.1) ).
        # send self message to start activity
        self.scheduleAt(simTime() + self.iaTime.doubleValue(), self.event)
