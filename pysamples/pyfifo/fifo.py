from pyopp import cSimpleModule, EV, cMessage, simTime, SIMTIME_ZERO, cQueue


class AbstractFifo(cSimpleModule):
    def __init__(self):
        cSimpleModule.__init__(self)

        # cMessages
        self.msgServiced = None
        self.endServiceMsg = None

        # cQueue
        self.queue = cQueue()

        # signals
        self.qlenSignal = None
        self.busySignal = None
        self.queueingTimeSignal = None

    def arrival(self, msg):
        pass

    def startService(self, msg):
        pass

    def endService(self, msg):
        pass

    def __del__(self):
        self.delete(self.msgServiced)
        self.cancelAndDelete(self.endServiceMsg)
        self.msgServiced = self.endServiceMsg = None

    def initialize(self):
        self.endServiceMsg = cMessage("end-service")
        self.queue.setName("queue");

        self.qlenSignal = self.registerSignal("qlen")
        self.busySignal = self.registerSignal("busy")
        self.queueingTimeSignal = self.registerSignal("queueingTime")
        self.emit(self.qlenSignal, self.queue.getLength())
        self.emit(self.busySignal, False);

    def handleMessage(self, msg):
        if msg is self.endServiceMsg:
            self.endService(self.msgServiced)
            if self.queue.isEmpty():
                self.msgServiced = None;
                self.emit(self.busySignal, False)
            else:
                self.msgServiced = self.queue.pop()
                self.emit(self.qlenSignal, self.queue.getLength())
                self.emit(self.queueingTimeSignal, simTime() - self.msgServiced.getTimestamp())
                serviceTime = self.startService(self.msgServiced)
                self.scheduleAt(simTime() + serviceTime, self.endServiceMsg)
        elif not self.msgServiced:
            self.arrival(msg)
            self.msgServiced = msg;
            self.emit(self.queueingTimeSignal, SIMTIME_ZERO)
            serviceTime = self.startService(self.msgServiced)
            self.scheduleAt(simTime() + serviceTime, self.endServiceMsg)
            self.emit(self.busySignal, True)
        else:
            self.arrival(msg)
            self.queue.insert(msg)
            msg.setTimestamp()
            self.emit(self.qlenSignal, self.queue.getLength())


class PyFifo(AbstractFifo):

    def startService(self, msg):
        EV << "Starting service of " << msg.getName() << '\n'
        return self.par("serviceTime").doubleValue()

    def endService(self, msg):
        EV << "Completed service of " << msg.getName() << '\n'
        self.send(msg, "out")
