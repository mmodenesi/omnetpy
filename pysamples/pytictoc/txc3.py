from pyopp import cSimpleModule, cMessage, EV, WATCH

EV << "---- Loading module {} ----\n".format(__name__)

class PyTxc3(cSimpleModule):

    def initialize(self):
        self.counter = 10
        WATCH('counter', self)
        if self.getName() == 'tic':
            EV << 'Sending initial message\n'
            msg = cMessage("pymessage")
            self.send(msg, "out")

    def handleMessage(self, msg):
        self.counter -= 1
        if self.counter == 0:
            EV << self.getName() << "'s counter reached zero, end of simulation\n";
            self.delete(msg)
        else:
            EV << self.getName() << "'s counter is " << self.counter << ", sending back message\n";
            self.send(msg, "out")
