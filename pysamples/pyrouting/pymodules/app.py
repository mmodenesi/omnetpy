import random

from pyopp import (
    EV,
    cSimpleModule,
    cMessage,
    WATCH,
    simTime,
)

from .messages import Packet


class App(cSimpleModule):
    '''Generates traffic for the network.'''

    def __init__(self):
        cSimpleModule.__init__(self)
        # configuration
        self.myAddress = None
        self.destAddresses = list()
        self.sendIATime = None
        self.packetLengthBytes = None

        # state
        self.generatePacket = None
        self.pkCounter = 0

        # signals
        self.endToEndDelaySignal = None
        self.hopCountSignal = None
        self.sourceAddressSignal = None

    def __del__(self):
        self.cancelAndDelete(self.generatePacket)

    def initialize(self):
        self.myAddress = self.par('address').intValue()
        self.packetLengthBytes = self.par('packetLength')
        self.sendIATime = self.par('sendIaTime')  # volatile parameter

        WATCH('pkCounter')
        WATCH('myAddress')

        self.destAddresses = [
            int(elem) for elem in self.par('destAddresses').stringValue().split()]

        if len(self.destAddresses) == 0:
            raise RuntimeError(
                'At least one address must be specified in the destAddresses parameter!')

        self.generatePacket = cMessage('nextPacket')
        self.scheduleAt(self.sendIATime.doubleValue(), self.generatePacket)

        self.endToEndDelaySignal = self.registerSignal('endToEndDelay')
        self.hopCountSignal = self.registerSignal('hopCount')
        self.sourceAddressSignal = self.registerSignal('sourceAddress')

    def handleMessage(self, msg):
        if msg is self.generatePacket:
            self.sendPacket()
        else:
            self.handleIncomingMessage(msg)

    def sendPacket(self):
        # Sending packet
        destAddress = random.choice(self.destAddresses)
        self.pkCounter += 1
        pk = Packet(
            name='pk-{}-to-{}-#{}'.format(self. myAddress, destAddress, self.pkCounter),
            srcAddr=self.myAddress,
            destAddr=destAddress,
            kind=random.randint(0, 7))
        pk.setByteLength(self.packetLengthBytes.intValue())

        EV << 'sending packet ' << pk << '\n'

        self.send(pk, 'out')

        self.scheduleAt(simTime() + self.sendIATime.doubleValue(), self.generatePacket)
        if self.hasGUI():
            self.getParentModule().bubble('Generating packet...')

    def handleIncomingMessage(self, pk):
        EV << 'received packet ' << pk.getName() << ' after ' << pk.hopCount << 'hops\n'
        self.emit(self.endToEndDelaySignal, simTime() - pk.getCreationTime())
        self.emit(self.hopCountSignal, pk.hopCount)
        self.emit(self.sourceAddressSignal, pk.srcAddr)
        self.delete(pk)
        pk = None
        if self.hasGUI():
            self.getParentModule().bubble('Arrived!')
