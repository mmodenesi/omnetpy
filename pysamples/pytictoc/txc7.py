from pyopp import cSimpleModule, cMessage, EV, WATCH, simTime


EV << "---- Loading module {} ----\n".format(__name__)

class PyTxc7(cSimpleModule):

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
            # "Lose" the message with 0.1 probability:
            if self.uniform(0, 1) < 0.1:
                EV << '"Losing" message\n';
                self.delete(msg)
            else:
                # The "delayTime" module parameter can be set to values like
                # "exponential(5)" (tictoc7.ned, omnetpp.ini), and then here
                # we'll get a different delay every time.
                delay = self.par("delayTime").doubleValue()

                EV << "Message arrived, starting to wait " << delay << " secs...\n";
                self.tictocmsg = msg;
                self.scheduleAt(simTime() + delay, self.event);

    def __del__(self):
        self.cancelAndDelete(self.event)
        if self.tictocmsg:
            self.delete(self.tictocmsg)
