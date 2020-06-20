from pyopp import cSimpleModule


class PySwitch(cSimpleModule):
    """Sends received packets to one of the outputs; see NED file for more info."""

    def initialize(self):
        self.priority = self.par("priority").intValue()
        self.numGates = self.gateSize("out")

    def handleMessage(self, msg):
        gate = self.intuniform(0, self.numGates - 1)
        msg.setSchedulingPriority(self.priority)
        self.send(msg, "out", gate)

    def __del__(self):
        print('__del__', self.__class__.__name__, self)
