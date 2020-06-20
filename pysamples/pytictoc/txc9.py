"""
In the previous model we just created another packet if we needed to
retransmit. This is OK because the packet didn't contain much, but
in real life it's usually more practical to keep a copy of the original
packet so that we can re-send it without the need to build it again.
"""
from pyopp import cSimpleModule, cMessage, EV, simTime

class PyTic9(cSimpleModule):

    def initialize(self):
        # Initialize variables.
        self.timeout = 1.0
        self.timeoutEvent = cMessage("timoutEvent")  # timeout self-message
        self.seq = 0  # message sequence number
        self.message = None  # message that has to be re-sent on timeout

        # Generate and send initial message.
        EV << "Sending initial message\n"
        self.message = self.generateNewMessage()
        self.sendCopyOf(self.message)
        self.scheduleAt(simTime() + self.timeout, self.timeoutEvent)

    def handleMessage(self, msg):
        if msg is self.timeoutEvent:
            # If we receive the timeout event, that means the packet hasn't
            # arrived in time and we have to re-send it.
            EV << "Timeout expired, resending message and restarting timer\n"
            self.sendCopyOf(self.message)
            self.scheduleAt(simTime() + self.timeout, self.timeoutEvent)
        else:
            # message arrived
            # Acknowledgement received!
            EV << "Received: " << msg.getName() << "\n"
            self.delete(msg)

            # Also delete the stored message and cancel the timeout event.
            EV << "Timer cancelled.\n";
            self.cancelEvent(self.timeoutEvent)
            self.delete(self.message)
            self.message = None

            # Ready to send another one.
            self.message = self.generateNewMessage()
            self.sendCopyOf(self.message)
            self.scheduleAt(simTime() + self.timeout, self.timeoutEvent)

    def generateNewMessage(self):
        # Generate a message with a different name every time.
        self.seq += 1
        return cMessage('tic-{:02d}'.format(self.seq))

    def sendCopyOf(self, msg):
        # Duplicate message and send the copy.
        self.send(msg.dup(), "out")

    def finish(self):
        if self.message and not self.message.isScheduled():
            self.delete(self.message)

        if self.timeoutEvent and not self.timeoutEvent.isScheduled():
            self.delete(self.timeoutEvent)



class PyToc9(cSimpleModule):
    """Sends back an acknowledgement -- or not."""
    def handleMessage(self, msg):
        if self.uniform(0, 1) < 0.1:
            EV << '"Losing" message ' << msg.getName() << '\n';
            self.bubble("message lost");
            self.delete(msg)
        else:
            EV << msg.getName() << " received, sending back an acknowledgement.\n";
            self.delete(msg)
            self.send(cMessage("ack"), "out");
