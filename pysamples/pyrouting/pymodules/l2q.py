from pyopp import (
    cSimpleModule,
    cMessage,
    cQueue,
    EV,
    simTime,
    SIMTIME_ZERO,
)


class L2Queue(cSimpleModule):
    '''
    Point-to-point interface module. While one frame is transmitted,
    additional frames get queued up; see NED file for more info.
    '''

    def __init__(self):
        cSimpleModule.__init__(self)

        self.frameCapacity = None

        self.queue = cQueue()
        self.endTransmissionEvent = None
        self.isBusy = False

        # signals
        self.qlenSignal = None
        self.busySignal = None
        self.queueingTimeSignal = None
        self.dropSignal = None
        self.txBytesSignal = None
        self.rxBytesSignal = None

    def __del__(self):
        self.cancelAndDelete(self.endTransmissionEvent)

    def initialize(self):
        self.queue.setName('queue')
        self.endTransmissionEvent = cMessage('endTxEvent')

        if self.par('useCutThroughSwitching').boolValue():
            self.gate('line$i').setDeliverOnReceptionStart(True)

        self.frameCapacity = self.par('frameCapacity').intValue()

        self.qlenSignal = self.registerSignal('qlen')
        self.busySignal = self.registerSignal('busy')
        self.queueingTimeSignal = self.registerSignal('queueingTime')
        self.dropSignal = self.registerSignal('drop')
        self.txBytesSignal = self.registerSignal('txBytes')
        self.rxBytesSignal = self.registerSignal('rxBytes')

        self.emit(self.qlenSignal, self.queue.getLength())
        self.emit(self.busySignal, False)

    def startTransmitting(self, msg):
        EV << 'Starting transmission of ' << msg << '\n'
        self.isBusy = True

        self.send(msg, 'line$o')
        self.emit(self.txBytesSignal, msg.getByteLength())

        # Schedule an event for the time when last bit will leave the gate.
        endTransmission = self.gate('line$o').getTransmissionChannel().getTransmissionFinishTime()
        self.scheduleAt(endTransmission, self.endTransmissionEvent)

    def handleMessage(self, msg):
        if msg is self.endTransmissionEvent:
            # Transmission finished, we can start next one.
            EV << 'Transmission finished.\n'
            self.isBusy = False
            if self.queue.isEmpty():
                self.emit(self.busySignal, False)
            else:
                msg = self.queue.pop()
                self.emit(self.queueingTimeSignal, simTime() - msg.getTimestamp())
                self.emit(self.qlenSignal, self.queue.getLength())
                self.startTransmitting(msg)
        elif msg.arrivedOn('line$i'):
            # pass up
            self.emit(self.rxBytesSignal, msg.getByteLength())
            self.send(msg, 'out')
        else:  #/ arrived on gate 'in'
            if self.endTransmissionEvent.isScheduled():
                # We are currently busy, so just queue up the packet.
                if self.frameCapacity and self.queue.getLength() >= self.frameCapacity:
                    EV << 'Received ' << msg \
                       << ' but transmitter busy and queue full: discarding\n'
                    self.emit(self.dropSignal, msg.getByteLength())
                    self.delete(msg)
                    msg = None
                else:
                    EV << 'Received ' << msg << ' but transmitter busy: queueing up\n'
                    msg.setTimestamp()
                    self.queue.insert(msg)
                    self.emit(self.qlenSignal, self.queue.getLength())
            else:
                # We are idle, so we can start transmitting right away.
                EV << 'Received ' << msg << '\n'
                self.emit(self.queueingTimeSignal, SIMTIME_ZERO)
                self.startTransmitting(msg)
                self.emit(self.busySignal, True)

    def refreshDisplay(self):
        self.getDisplayString().setTagArg('t', 0, 'transmitting' if self.isBusy else 'idle')
        self.getDisplayString().setTagArg(
            'i', 1, ('red' if self.queue.getLength() >= 3 else 'yellow') if self.isBusy else '')
