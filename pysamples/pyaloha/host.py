import math

from pyopp import cSimpleModule
from pyopp import cMessage, cPacket
from pyopp import WATCH, EV
from pyopp import simTime
from pyopp import cFigure, cRingFigure, cOvalFigure


class PyHost(cSimpleModule):

    # speed of light in m/s
    PROPAGATIONSPEED = 299792458.0

    # state constants
    IDLE = 0
    TRANSMIT = 1

    def __init__(self, *args, **kwargs):
        cSimpleModule.__init__(self, *args, **kwargs)

        # parameters
        self.radioDelay = None  # simtime_t
        self.txRate = None  # double
        self.iaTime = None # cPar *
        self.pkLenBits = None # cPar *
        self.slotTime = None # simtime_t
        self.isSlotted = None  # bool

        # state variables, event pointers etc
        self.server = None
        self.endTxEvent = None
        self.state = None
        self.stateSignal = None
        self.pkCounter = None

        # position on the canvas, unit is m
        self.x = None
        self.y = None

        # animation parameters
        self.ringMaxRadius = 2000 # in m
        self.circlesMaxRadius = 1000 # in m
        self.idleAnimationSpeed = None
        self.transmissionEdgeAnimationSpeed = None
        self.midtransmissionAnimationSpeed = None

        # figures and animation state
        self.lastPacket = None # a copy of the last sent message, needed for animation
        self.transmissionRing = None # shows the last packet
        self.transmissionCircles = []

    def __del__(self):
        self.delete(self.lastPacket)
        self.cancelAndDelete(self.endTxEvent)

    def initialize(self):
        self.stateSignal = self.registerSignal("state")
        self.server = self.getModuleByPath("server")
        if not self.server:
            raise RuntimeError("server not found")

        self.txRate = self.par("txRate").doubleValue()

        # TODO
        self.iaTime = self.par("iaTime")
        self.pkLenBits = self.par("pkLenBits")

        self.slotTime = self.par("slotTime").doubleValue()
        self.isSlotted = self.slotTime > 0
        WATCH('slotTime')
        WATCH('isSlotted')

        self.endTxEvent = cMessage("send/endTx")
        self.state = self.IDLE
        self.emit(self.stateSignal, self.state)
        self.pkCounter = 0
        WATCH('state')
        WATCH('pkCounter')

        self.x = self.par("x").doubleValue()
        self.y = self.par("y").doubleValue()

        serverX = self.server.par("x").doubleValue()
        serverY = self.server.par("y").doubleValue()

        self.idleAnimationSpeed = self.par("idleAnimationSpeed").doubleValue()
        self.transmissionEdgeAnimationSpeed = self.par("transmissionEdgeAnimationSpeed").doubleValue()
        self.midtransmissionAnimationSpeed = self.par("midTransmissionAnimationSpeed").doubleValue()

        dist = math.sqrt((self.x - serverX) ** 2  + (self.y - serverY) ** 2)
        self.radioDelay = dist / self.PROPAGATIONSPEED

        self.getDisplayString().setTagArg("p", 0, self.x)
        self.getDisplayString().setTagArg("p", 1, self.y)

        self.scheduleAt(self.getNextTransmissionTime(), self.endTxEvent)

    def handleMessage(self, msg):
        EV << self.str() << " handleMessage\n"
        assert msg == self.endTxEvent

        self.getParentModule().getCanvas().setAnimationSpeed(
            self.transmissionEdgeAnimationSpeed, self)

        if self.state == self.IDLE:
            # generate packet and schedule timer when it ends
            self.pkCounter += 1
            pkname = "pk-%d-#%d" % (self.getId(), self.pkCounter)
            EV << "generating packet " << pkname << '\n'

            self.state = self.TRANSMIT
            self.emit(self.stateSignal, self.state)

            pk = cPacket(pkname)
            pk.setBitLength(self.pkLenBits.intValue())
            duration = pk.getBitLength() / self.txRate
            self.sendDirect(pk, self.radioDelay, duration, self.server.gate("in"))
            self.scheduleAt(simTime() + duration, self.endTxEvent)

            # let visualization code know about the new packet
            if self.transmissionRing is not None:
                self.delete(self.lastPacket)
                self.lastPacket = pk.dup()
                self.transmissionRing.setAssociatedObject(None)
                for c in self.transmissionCircles:
                    c.setAssociatedObject(None)
        elif self.state == self.TRANSMIT:
            # endTxEvent indicates end of transmission
            self.state = self.IDLE
            self.emit(self.stateSignal, self.state)

            # schedule next sending
            self.scheduleAt(self.getNextTransmissionTime(), self.endTxEvent)
        else:
            raise RuntimeError("invalid state")

    def getNextTransmissionTime(self):
        t = simTime() + self.iaTime.doubleValue()
        if not self.isSlotted:
            return t
        else:
            # align time of next transmission to a slot boundary
            return self.slotTime * math.ceil(t / self.slotTime)

    def refreshDisplay(self):
        canvas = self.getParentModule().getCanvas()
        numCircles = 20
        circleLineWidth = 10

        # create figures on our first invocation
        if not self.transmissionRing:
            color = cFigure.GOOD_DARK_COLORS[self.getId() % cFigure.NUM_GOOD_DARK_COLORS]

            self.transmissionRing = cRingFigure("Host %d Ring" % self.getIndex())
            self.transmissionRing.setOutlined(False)
            self.transmissionRing.setFillColor(color)
            self.transmissionRing.setFillOpacity(0.25)
            self.transmissionRing.setFilled(True)
            self.transmissionRing.setVisible(False)
            self.transmissionRing.setZIndex(-1)
            canvas.addFigure(self.transmissionRing)

            for i in range(numCircles):
                circle = cOvalFigure("Host %d Circle" % self.getIndex())
                circle.setFilled(False)
                circle.setLineColor(color)
                circle.setLineOpacity(0.75)
                circle.setLineWidth(circleLineWidth)
                circle.setZoomLineWidth(True)
                circle.setVisible(False)
                circle.setZIndex(-0.5)
                self.transmissionCircles.append(circle)
                canvas.addFigure(circle)

        if self.lastPacket:
            # update transmission ring and circles
            if self.transmissionRing.getAssociatedObject() != self.lastPacket:
                self.transmissionRing.setAssociatedObject(self.lastPacket)
                for c in self.transmissionCircles:
                    c.setAssociatedObject(self.lastPacket)

            now = simTime()
            frontTravelTime = now - self.lastPacket.getSendingTime()
            backTravelTime = now - (self.lastPacket.getSendingTime() + self.lastPacket.getDuration())

            # conversion from time to distance in m using speed
            frontRadius = min(self.ringMaxRadius, frontTravelTime.dbl() * self.PROPAGATIONSPEED)
            backRadius = backTravelTime.dbl() * self.PROPAGATIONSPEED
            circleRadiusIncrement = self.circlesMaxRadius / numCircles

            # update transmission ring geometry and visibility/opacity
            opacity = 1.0
            if backRadius > self.ringMaxRadius:
                self.transmissionRing.setVisible(False)
                self.transmissionRing.setAssociatedObject(None)
            else:
                self.transmissionRing.setVisible(True)
                self.transmissionRing.setBounds(
                    cFigure.Rectangle(
                        self.x - frontRadius,
                        self.y - frontRadius,
                        2*frontRadius,
                        2*frontRadius))
                self.transmissionRing.setInnerRadius(max(0.0, min(self.ringMaxRadius, backRadius)))
                if backRadius > 0:
                    opacity = max(0.0, 1.0 - backRadius / self.circlesMaxRadius)

            self.transmissionRing.setLineOpacity(opacity)
            self.transmissionRing.setFillOpacity(opacity / 5)

            # update transmission circles geometry and visibility/opacity
            radius0 = (frontTravelTime.dbl() * self.PROPAGATIONSPEED) % circleRadiusIncrement
            for i, circle in enumerate(self.transmissionCircles):
                circleRadius = min(self.ringMaxRadius, radius0 + i * circleRadiusIncrement)
                if circleRadius < frontRadius - circleRadiusIncrement / 2 and circleRadius > backRadius + circleLineWidth / 2:
                    circle.setVisible(True)
                    circle.setBounds(cFigure.Rectangle(self.x - circleRadius, self.y - circleRadius, 2*circleRadius, 2*circleRadius))
                    circle.setLineOpacity(max(0.0, 0.2 - 0.2 * (circleRadius / self.circlesMaxRadius)))
                else:
                    circle.setVisible(False)

            #/ compute animation speed
            animSpeed = self.idleAnimationSpeed
            if (frontRadius >= 0 and frontRadius < self.circlesMaxRadius) or (backRadius >= 0 and backRadius < self.circlesMaxRadius):
                animSpeed = self.transmissionEdgeAnimationSpeed
            if frontRadius > self.circlesMaxRadius and backRadius < 0:
                animSpeed = self.midtransmissionAnimationSpeed
            canvas.setAnimationSpeed(animSpeed, self)
        else:
            # hide transmission rings, update animation speed
            if self.transmissionRing.getAssociatedObject() is not None:
                self.transmissionRing.setVisible(False)
                self.transmissionRing.setAssociatedObject(None)

                for c in self.transmissionCircles:
                    c.setVisible(False)
                    c.setAssociatedObject(None)
                canvas.setAnimationSpeed(self.idleAnimationSpeed, self)

        # update host appearance (color and text)
        self.getDisplayString().setTagArg("t", 2, "#808000")
        if self.state == self.IDLE:
            self.getDisplayString().setTagArg("i", 1, "")
            self.getDisplayString().setTagArg("t", 0, "")
        elif self.state == self.TRANSMIT:
            self.getDisplayString().setTagArg("i", 1, "yellow")
            self.getDisplayString().setTagArg("t", 0, "TRANSMIT")
