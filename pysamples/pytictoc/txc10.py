"""
Let's make it more interesting by using several (n) `tic' modules,
and connecting every module to every other. For now, let's keep it
simple what they do: module 0 generates a message, and the others
keep tossing it around in random directions until it arrives at
module 2.
"""

from pyopp import cSimpleModule, cMessage, EV


class PyTxc10(cSimpleModule):
    def initialize(self):
        if self.getIndex() == 0:
            # Boot the process scheduling the initial message as a self-message.
            self.scheduleAt(0.0, cMessage('tic-0'))

    def handleMessage(self, msg):
        if self.getIndex() == 3:
            # Message arrived.
            EV << "Message " << msg.getName() << " arrived.\n";
        else:
            # We need to forward the message.
            self.forwardMessage(msg)

    def forwardMessage(self, msg):
        # In this example, we just pick a random gate to send it on.
        # We draw a random number between 0 and the size of gate `out[]'.
        n = self.gateSize("out")
        k = self.intuniform(0, n-1);

        EV << "Forwarding message " << msg.getName() << " on port out[" << k << "]\n"
        self.send(msg, "out", k)
