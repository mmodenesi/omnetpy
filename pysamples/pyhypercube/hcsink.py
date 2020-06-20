from pyopp import (
    cSimpleModule,
    EV
)

from packets import HCPacket

def hammingDistance(a, b):
    return bin(a ^ b).count('1')


class HCSink(cSimpleModule):
    def initialize(self):
        self.endToEndDelaySignal = self.registerSignal("endToEndDelay")
        self.hopCountSignal = self.registerSignal("hopCount")
        self.hopRatioSignal = self.registerSignal("hopRatio")

    def handleMessage(self, pkt):
        # calculate statistics and record them
        eed = pkt.getArrivalTime() - pkt.getTimestamp()
        actualHops = pkt.hops
        minHops = hammingDistance(pkt.srcAddress, pkt.destAddress)

        self.emit(self.endToEndDelaySignal, eed)
        self.emit(self.hopCountSignal, actualHops)
        self.emit(self.hopRatioSignal, actualHops / minHops)

        EV << "Received: " << pkt.getName() << '\n'
        EV << "  - end-to-end delay=" << eed << '\n'
        EV << "  - distance=" << minHops << ", actual hops=" << actualHops << '\n'

        # message no longer needed
        self.delete(pkt)
        pkt = None
