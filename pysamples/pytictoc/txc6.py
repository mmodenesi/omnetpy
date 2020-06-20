from pyopp import cSimpleModule, cMessage, EV, simTime

EV << "---- Loading module {} ----\n".format(__name__)

class PyTxc6(cSimpleModule):

    def initialize(self):
        self.event = cMessage('event')
        self.tictocmsg = None
        if self.getName() == 'tic':
            EV << "Scheduling first send to t=5.0s\n";
            self.tictocmsg = cMessage("tictocmsg")
            self.scheduleAt(5.0, self.event)

    def handleMessage(self, msg):
        if msg is self.event:
            # The self-message arrived, so we can send out tictocmsg and nullptr out
            # its pointer so that it doesn't confuse us later.
            EV << "Wait period is over, sending back message\n"
            self.send(self.tictocmsg, "out")
            self.tictocmsg = None
        else:
            # If the message we received is not our self-message, then it must be the tic-toc
            # message arriving from our partner. We remember its pointer in the tictocMsg variable,
            # then schedule our self-message to come back to us in 1s simulated time.
            EV << "Message arrived, starting to wait 1 sec...\n";
            self.tictocmsg = msg;
            self.scheduleAt(simTime() + 1.0, self.event);

    def __del__(self):
        self.cancelAndDelete(self.event)
        if self.tictocmsg:
            self.delete(self.tictocmsg)
