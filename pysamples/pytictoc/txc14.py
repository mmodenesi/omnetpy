"""
In this step we keep track of how many messages we send and received,
and display it above the icon.
"""

from pyopp import cSimpleModule, cMessage, EV, WATCH


class TicTocMsg14(cMessage):
    def __init__(self, name):
        super().__init__(name)
        self.source = None
        self.destination = None
        self.hopcount = 0


class PyTxc14(cSimpleModule):

    def initialize(self):

        self.numSent = 0
        self.numReceived = 0
        WATCH('numSent')
        WATCH('numReceived')

        # Module 0 sends the first message
        if self.getIndex() == 0:
            # Boot the process scheduling the initial message as a self-message.
            self.numSent += 1
            self.scheduleAt(0.0, self.generateMessage())

    def handleMessage(self, ttmsg):
        assert isinstance(ttmsg, TicTocMsg14)

        if ttmsg.destination == self.getIndex():
            # Message arrived.
            EV << "Message " << ttmsg.getName() << " arrived after " << ttmsg.hopcount << " hops.\n"
            self.numReceived += 1
            self.bubble("ARRIVED, starting new one!")
            self.delete(ttmsg)

            # Generate another one.
            EV << "Generating another message: "
            newmsg = self.generateMessage()
            EV << newmsg.getName() << '\n';
            self.forwardMessage(newmsg)
            self.numSent += 1
        else:
            # We need to forward the message.
            self.forwardMessage(ttmsg)

    def generateMessage(self):
        # Produce source and destination addresses.
        src = self.getIndex()  # our module index
        n = self.getVectorSize()  # module vector size
        dest = self.intuniform(0, n - 2)
        if dest >= src:
            dest += 1
        # Create message object and set source and destination field.
        msg = TicTocMsg14("tic-{}-to-{}".format(src, dest))
        msg.source = src
        msg.destination = dest
        return msg

    def forwardMessage(self, msg):
        # Increment hop count.
        msg.hopcount += 1

        # Same routing as before: random gate.
        n = self.gateSize("gate")
        k = self.intuniform(0, n-1)

        EV << "Forwarding message " << msg << " on gate[" << k << "]\n"
        self.send(msg, "gate$o", k)

    def refreshDisplay(self):
        text = "rcvd: {} sent: {}".format(self.numReceived, self.numSent)
        self.getDisplayString().setTagArg("t", 0, text)
