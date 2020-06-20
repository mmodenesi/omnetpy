"""
Let us take a step back, and remove random delaying from the code.
We'll leave in, however, losing the packet with a small probability.
And, we'll we do something very common in telecommunication networks:
if the packet doesn't arrive within a certain period, we'll assume it
was lost and create another one. The timeout will be handled using
(what else?) a self-message.
"""

from pyopp import cSimpleModule, cMessage, EV, simTime


class PyTic8(cSimpleModule):

    def initialize(self):
        self.timeout = 1.0
        self.timeoutEvent = cMessage("timeoutEvent")

        # Generate and send initial message
        EV << "Sending initial message\n"
        msg = cMessage("tictocMsg");
        self.send(msg, "out")
        self.scheduleAt(simTime() + self.timeout, self.timeoutEvent);

    def handleMessage(self, msg):
        if msg is self.timeoutEvent:
            # If we receive the timeout event, that means the packet hasn't
            # arrived in time and we have to re-send it.
            EV << "Timeout expired, resending message and restarting timer\n"
            newMsg = cMessage("tictocMsg")
            self.send(newMsg, "out")
            self.scheduleAt(simTime() + self.timeout, self.timeoutEvent)
        else:
            # message arrived
            # Acknowledgement received -- delete the received message and cancel
            # the timeout event.
            self.cancelEvent(self.timeoutEvent)
            EV << "Timer cancelled.\n"
            self.delete(msg)

            # Ready to send another one.
            newMsg = cMessage("tictocMsg")
            self.send(newMsg, "out");
            self.scheduleAt(simTime() + self.timeout, self.timeoutEvent)

    def __del__(self):
        self.cancelAndDelete(self.timeoutEvent)


class PyToc8(cSimpleModule):
    """Sends back an acknowledgement -- or not."""
    def handleMessage(self, msg):
        if self.uniform(0, 1) < 0.5:
            EV << '"Losing" message.\n';
            self.bubble("message lost");  # making animation more informative...
            self.delete(msg)
        else:
            EV << "Sending back same message as acknowledgement.\n";
            self.send(msg, "out");
