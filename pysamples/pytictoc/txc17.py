from pyopp import cSimpleModule, cMessage, EV


class TicTocMsg17(cMessage):
    def __init__(self, name, source, destination):
        super().__init__(name)
        self.source = source
        self.destination = destination
        self.hopcount = 0


class PyTxc17(cSimpleModule):

    def initialize(self):
        self.arrivalSignal = self.registerSignal("arrival")
        # Module 0 sends the first message
        if self.getIndex() == 0:
            # Boot the process scheduling the initial message as a self-message.
            self.scheduleAt(0.0, self.generateMessage())

    def handleMessage(self, ttmsg):
        assert isinstance(ttmsg, TicTocMsg17)

        if ttmsg.destination == self.getIndex():
            # Message arrived
            hopcount = ttmsg.hopcount
            # send a signal
            self.emit(self.arrivalSignal, hopcount)

            if self.hasGUI():
                # Write last hop count to string
                label = "last hopCount = {}".format(hopcount)
                # Get pointer to figure
                canvas = self.getParentModule().getCanvas()
                textFigure = canvas.getFigure("lasthopcount")
                # Update figure text
                # textFigure.as_cTextFigure().setText(label)
                textFigure.setText(label)

            EV << "Message " << ttmsg << " arrived after " << hopcount << " hops.\n"
            self.bubble("ARRIVED, starting new one!")

            self.delete(ttmsg)

            # Generate another one.
            EV << "Generating another message: "
            newmsg = self.generateMessage()
            EV << newmsg << '\n'
            self.forwardMessage(newmsg)
        else:
            # We need to forward the message.
            self.forwardMessage(ttmsg)

    def generateMessage(self):
        # Produce source and destination addresses.
        src = self.getIndex()
        n = self.getVectorSize()
        dest = self.intuniform(0, n - 2)
        if dest >= src:
            dest += 1

        # Create message object and set source and destination field.
        msg = TicTocMsg17("tic-%d-to-%d" % (src, dest), src, dest)
        return msg

    def forwardMessage(self, msg):
        # Increment hop count.
        msg.hopcount += 1

        # Same routing as before: random gate.
        n = self.gateSize("gate")
        k = self.intuniform(0, n - 1)

        EV << "Forwarding message " << msg << " on gate[" << k << "]\n"
        self.send(msg, "gate$o", k)
