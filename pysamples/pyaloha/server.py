from pyopp import cSimpleModule, cMessage
from pyopp import WATCH, EV
from pyopp import simTime, cTimestampedValue


class PyServer(cSimpleModule):

    IDLE = 0
    TRANSMISSION = 1
    COLLISION = 2

    def __init__(self, *args, **kwargs):
        cSimpleModule.__init__(self, *args, **kwargs)
        self.endRxEvent = None

    def __del__(self):
        self.cancelAndDelete(self.endRxEvent)

    def initialize(self):
        self.channelStateSignal = self.registerSignal("channelState")
        self.endRxEvent = cMessage("end-reception")
        self.channelBusy = False
        self.emit(self.channelStateSignal, self.IDLE)

        self.gate("in").setDeliverOnReceptionStart(True)

        self.currentCollisionNumFrames = 0
        self.receiveCounter = 0
        WATCH('currentCollisionNumFrames')

        self.receiveBeginSignal = self.registerSignal("receiveBegin")
        self.receiveSignal = self.registerSignal("receive")
        self.collisionSignal = self.registerSignal("collision")
        self.collisionLengthSignal = self.registerSignal("collisionLength")

        self.emit(self.receiveSignal, 0);
        self.emit(self.receiveBeginSignal, 0);

        self.getDisplayString().setTagArg("p", 0, self.par("x").doubleValue())
        self.getDisplayString().setTagArg("p", 1, self.par("y").doubleValue())

    def handleMessage(self, msg):
        if msg == self.endRxEvent:
            EV << "reception finished\n"
            self.channelBusy = False
            self.emit(self.channelStateSignal, self.IDLE)

            # update statistics
            if self.currentCollisionNumFrames == 0:
                # start of reception at recvStartTime
                tmp = cTimestampedValue(self.recvStartTime, 1)
                self.emit(self.receiveSignal, tmp)
                # end of reception now
                self.emit(self.receiveSignal, 0)
            else:
                # start of collision at recvStartTime
                tmp = cTimestampedValue(self.recvStartTime, self.currentCollisionNumFrames)
                self.emit(self.collisionSignal, tmp)

                dt = simTime() - self.recvStartTime
                self.emit(self.collisionLengthSignal, dt)

            self.currentCollisionNumFrames = 0;
            self.receiveCounter = 0;
            self.emit(self.receiveBeginSignal, self.receiveCounter)
        else:

            pkt = msg.as_cPacket()
            endReceptionTime = simTime() + pkt.getDuration()

            self.receiveCounter += 1
            self.emit(self.receiveBeginSignal, self.receiveCounter)

            if not self.channelBusy:
                EV << "started receiving\n"
                self.recvStartTime = simTime()
                self.channelBusy = True
                self.emit(self.channelStateSignal, self.TRANSMISSION)
                self.scheduleAt(endReceptionTime, self.endRxEvent)
            else:
                EV << "another frame arrived while receiving -- collision!\n"
                self.emit(self.channelStateSignal, self.COLLISION)

                if self.currentCollisionNumFrames == 0:
                    self.currentCollisionNumFrames = 2
                else:
                    self.currentCollisionNumFrames += 1

                if endReceptionTime > self.endRxEvent.getArrivalTime():
                    self.cancelEvent(self.endRxEvent)
                    self.scheduleAt(endReceptionTime, self.endRxEvent)

                # update network graphics
                if self.hasGUI():
                    self.bubble("Collision! (%d frames)" % self.currentCollisionNumFrames)
                    self.getParentModule().getCanvas().holdSimulationFor(
                        self.par("animationHoldTimeOnCollision").doubleValue())
            self.channelBusy = True
            self.delete(pkt)

    def refreshDisplay(self):
        if not self.channelBusy:
            self.getDisplayString().setTagArg("i2", 0, "status/off")
            self.getDisplayString().setTagArg("t", 0, "")
        elif self.currentCollisionNumFrames == 0:
            self.getDisplayString().setTagArg("i2", 0, "status/yellow")
            self.getDisplayString().setTagArg("t", 0, "RECEIVE")
            self.getDisplayString().setTagArg("t", 2, "#808000")
        else:
            self.getDisplayString().setTagArg("i2", 0, "status/red")
            self.getDisplayString().setTagArg("t", 0, "COLLISION")
            self.getDisplayString().setTagArg("t", 2, "#800000")

    def finish(self):
        EV << "duration: " << simTime().dbl() << '\n'
        self.recordScalar("duration", simTime())
