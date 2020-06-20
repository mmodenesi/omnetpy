import math

from pyopp import cSimpleModule, cMessage, cFigure, WATCH, simTime, SimTime, cPolygonFigure


class PyCarAnimator(cSimpleModule):

    def __init__(self, *args, **kwargs):
        """Initialize all attributes as None."""
        cSimpleModule.__init__(self, *args, **kwargs)
        self.timeStep  = None  # simtime_t
        self.lastStep  = simTime()  # simtime_t
        self.loc = None  # Point
        self.speed = None  # float
        self.heading = None  # float
        self.angularSpeed = None  # float
        self.targetPointIndex = None  # int
        self.distanceTravelled = None  # float
        self.road = None  # cPolygonFigure
        self.car = None  # cImageFigure
        self.antenna = None  # cFigure
        self.trail = None  # cPolylineFigure
        self.distanceDisplay = None  # cTextFigure
        self.headingDisplay = None  # cTextFigure

    def initialize(self):
        self.timeStep = SimTime(1)
        self.speed = 2
        self.heading = 0
        self.angularSpeed = 0
        self.targetPointIndex = 0
        self.distanceTravelled = 0

        canvas = self.getParentModule().getCanvas()

        canvas.setAnimationSpeed(50.0, self)

        self.road = canvas.getFigure("road")
        self.trail = canvas.getFigure("trail").asPolylineFigure()
        self.car = canvas.getFigure("car")
        self.antenna = canvas.getFigureByPath("car.antenna")
        self.distanceDisplay = canvas.getFigureByPath("status.distance")
        self.headingDisplay = canvas.getFigureByPath("status.heading")

        self.loc = self.road.getPoint(self.targetPointIndex)

        WATCH('timeStep')
        WATCH('loc.x')
        WATCH('loc.y')
        WATCH('speed')
        WATCH('heading')
        WATCH('angularSpeed')
        WATCH('targetPointIndex')
        WATCH('distanceTravelled')

        self.refreshDisplay()
        self.scheduleAt(simTime(), cMessage())

    def refreshDisplay(self):
        t = (simTime() - self.lastStep) / self.timeStep

        assert 0 <= t <= 1

        carTr = cFigure.Transform()
        carTr.rotate(self.heading + self.angularSpeed * t)

        distance = self.speed * t
        carTr.translate(self.loc.x + distance * math.cos(self.heading),
                        self.loc.y + distance * math.sin(self.heading))
        self.car.setTransform(carTr)

        antTr = cFigure.Transform()
        antTr.rotate(-2 * simTime().dbl() * math.pi / 180)
        self.antenna.setTransform(antTr)

        self.distanceDisplay.setText("{:.0f}".format(self.distanceTravelled))

        degrees = - int(self.heading * 180 / math.pi)
        degrees = degrees - 360 * int(math.floor(degrees / 360))
        self.headingDisplay.setText('{:.2f}°'.format(degrees));


    def handleMessage(self, msg):
        # move
        distance = self.speed * self.timeStep.dbl()
        self.loc.x += distance * math.cos(self.heading)
        self.loc.y += distance * math.sin(self.heading)

        target = self.road.getPoint(self.targetPointIndex)
        vectorToTarget = target - self.loc
        if vectorToTarget.getLength() < 50:  # reached
            self.targetPointIndex = (self.targetPointIndex + 1) % self.road.getNumPoints()

        targetDirection = math.atan2(vectorToTarget.y, vectorToTarget.x)
        diff = targetDirection - self.heading
        while diff < - math.pi:
            diff += 2 * math.pi
        while diff > math.pi:
            diff -= 2 * math.pi;

        # steer
        self.heading += self.angularSpeed * self.timeStep.dbl()

        self.angularSpeed = diff / 30

        self.distanceTravelled += distance
        self.refreshDisplay()

        self.trail.addPoint(self.loc)
        if self.trail.getNumPoints() > 500:
            self.trail.removePoint(0)

        self.lastStep = simTime()
        self.scheduleAt(simTime() + self.timeStep, msg);
