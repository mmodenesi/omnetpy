"""
The main problem with the previous step is that we must modify the model's
code if we want to change what statistics are gathered. Statistic calculation
is woven deeply into the model code which is hard to modify and understand.

OMNeT++ 4.1 provides a different mechanism called 'signals' that we can use
to gather statistics. First we have to identify the events where the state
of the model changes. We can emit signals at these points that carry the value
of chosen state variables. This way the C++ code only emits signals, but how those
signals are processed are determined only by the listeners that are attached to them.

The signals the model emits and the listeners that process them can be defined in
the NED file using the 'signal' and 'statistic' property.

We will gather the same statistics as in the previous step, but notice that we will not need
any private member variables to calculate these values. We will use only a single signal that
is emitted when a message arrives and carries the hopcount in the message.
"""

from pyopp import cSimpleModule, cMessage, EV


class TicTocMsg16(cMessage):
    def __init__(self, name, source, destination):
        super().__init__(name)
        self.source = source
        self.destination = destination
        self.hopcount = 0


class PyTxc16(cSimpleModule):
    def initialize(self):
        self.arrivalSignal = self.registerSignal("arrival")
        # Module 0 sends the first message
        if self.getIndex() == 0:
            # Boot the process scheduling the initial message as a self-message.
            self.scheduleAt(0.0, self.generateMessage())

    def handleMessage(self, ttmsg):
        assert isinstance(ttmsg, TicTocMsg16)

        if ttmsg.destination == self.getIndex():
            # Message arrived
            hopcount = ttmsg.hopcount
            # send a signal
            self.emit(self.arrivalSignal, hopcount)

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
        msg = TicTocMsg16("tic-%d-to-%d" % ( src, dest), src, dest)
        return msg

    def forwardMessage(self, msg):
        # Increment hop count.
        msg.hopcount += 1

        # Same routing as before: random gate.
        n = self.gateSize("gate")
        k = self.intuniform(0, n - 1)

        EV << "Forwarding message " << msg << " on gate[" << k << "]\n"
        self.send(msg, "gate$o", k)
