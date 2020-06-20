from pyopp import cSimpleModule, cMessage, simTime


class PySource(cSimpleModule):
    '''Generates messages or jobs; see NED file for more info.'''

    def __init__(self):
        cSimpleModule.__init__(self)
        self.sendMessageEvent = None

    def initialize(self):
        self.sendMessageEvent = cMessage("sendMessageEvent")
        self.scheduleAt(simTime(), self.sendMessageEvent)

    def handleMessage(self, msg):
        assert msg is self.sendMessageEvent

        job = cMessage("job")
        self.send(job, "out")

        self.scheduleAt(simTime() + self.par("sendIaTime").doubleValue(), self.sendMessageEvent)

    def __del__(self):
        self.cancelAndDelete(self.sendMessageEvent)
        self.sendMessageEvent = None
