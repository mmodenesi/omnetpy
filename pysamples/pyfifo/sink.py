from pyopp import cSimpleModule, simTime, EV


class PySink(cSimpleModule):
    '''Packet sink; see NED file for more info.'''

    def __init__(self):
        cSimpleModule.__init__(self)
        self.lifetimeSignal = None

    def initialize(self):
        self.lifetimeSignal = self.registerSignal("lifetime")

    def handleMessage(self, msg):
        lifetime = simTime() - msg.getCreationTime()
        EV << "Received " << msg.getName() << ", lifetime: " << lifetime << "s" << '\n'
        self.emit(self.lifetimeSignal, lifetime)
        self.delete(msg)
        msg = None
