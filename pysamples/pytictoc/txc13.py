"""
In this step the destination address is no longer node 2 -- we draw a
random destination, and we'll add the destination address to the message.

The best way is to subclass cMessage and add destination as a data member.

To make the model execute longer, after a message arrives to its destination
the destination node will generate another message with a random destination
address, and so forth.
"""

from pyopp import cSimpleModule, cMessage, EV


class TicTocMsg13(cMessage):
    def __init__(self, name):
        super().__init__(name)
        self.source = None
        self.destination = None
        self.hopcount = 0


class PyTxc13(cSimpleModule):

    def initialize(self):
        # Module 0 sends the first message
        if self.getIndex() == 0:
            # Boot the process scheduling the initial message as a self-message.
            self.scheduleAt(0.0, self.generateMessage())

    def handleMessage(self, ttmsg):
        assert isinstance(ttmsg, TicTocMsg13)

        if ttmsg.destination == self.getIndex():
            # Message arrived.
            EV << "Message " << ttmsg.getName() << " arrived after " << ttmsg.hopcount << " hops.\n"
            self.bubble("ARRIVED, starting new one!")
            self.delete(ttmsg)

            # Generate another one.
            EV << "Generating another message: "
            newmsg = self.generateMessage()
            EV << newmsg.getName() << '\n';
            self.forwardMessage(newmsg)
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
        msg = TicTocMsg13("tic-{}-to-{}".format(src, dest))
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
