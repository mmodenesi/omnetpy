from pyopp import cSimpleModule, cOutVector, cQueue, simTime, cMessage, EV



class AbstractQueue(cSimpleModule):
    """
    Abstract base class for single-server queues. Subclasses are
    expected to redefine the arrival(), startService() and endService()
    methods.
    """

    def __init__(self, *args, **kwargs):
        cSimpleModule.__init__(self, *args, **kwargs)
        self.priority = None
        self.msgServiced = None
        self.endServiceMsg = None
        self.queue = None
        self.queueLength = None

    def initialize(self):
        self.priority = self.par("priority").intValue()
        self.endServiceMsg = cMessage("end-service")
        self.queue = cQueue("queue")
        self.queueLength = cOutVector("queueLength")

    def handleMessage(self, msg):
        if msg == self.endServiceMsg:
            self.endService(self.msgServiced)
            if self.queue.isEmpty():
                self.msgServiced = None
            else:
                self.msgServiced = self.queue.pop()
                self.queueLength.record(self.queue.getLength())
                serviceTime = self.startService(self.msgServiced)
                self.endServiceMsg.setSchedulingPriority(self.priority)
                self.scheduleAt(simTime() + serviceTime, self.endServiceMsg)
        elif self.msgServiced is None:
            self.arrival(msg)
            self.msgServiced = msg
            serviceTime = self.startService(self.msgServiced)
            self.endServiceMsg.setSchedulingPriority(self.priority)
            self.scheduleAt(simTime() + serviceTime, self.endServiceMsg)
        else:
            self.arrival(msg)
            self.queue.insert(msg)
            self.queueLength.record(self.queue.getLength())

    def refreshDisplay(self):
        self.getDisplayString().setTagArg("i2", 0, "status/execute" if self.msgServiced else "")

    def arrival(self, msg):
        pass



class PyQueue(AbstractQueue):
    """Queue model, with service time as parameter; see NED file for more info."""

    def initialize(self):
        super().initialize()
        numInitialJobs = self.par("numInitialJobs").intValue()
        for i in range(numInitialJobs):
            job = cMessage("job")
            self.queue.insert(job);
            self.queueLength.record(self.queue.getLength())

        if not self.queue.isEmpty():
            self.msgServiced = self.queue.pop()
            self.queueLength.record(self.queue.getLength())
            serviceTime = self.startService(self.msgServiced)
            self.scheduleAt(simTime() + serviceTime, self.endServiceMsg)

    def startService(self, msg):
        EV << "Starting service of " << msg.getName() << '\n'
        return self.par("serviceTime").doubleValue()

    def endService(self, msg):
        EV << "Completed service of " << msg.getName() << '\n'
        msg.setSchedulingPriority(self.priority)
        self.send(msg, "out")

    def __del__(self):
        print('__del__', self.getFullPath())
        self.delete(self.msgServiced)
        self.cancelAndDelete(self.endServiceMsg)

