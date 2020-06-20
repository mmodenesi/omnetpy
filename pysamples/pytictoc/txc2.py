from pyopp import cSimpleModule, cMessage, EV

EV << "---- Loading module {} ----\n".format(__name__)

class PyTxc2(cSimpleModule):

    def initialize(self):
        if self.getName() == 'tic':
            EV << 'Sending initial message\n'
            msg = cMessage("pymessage")
            self.send(msg, "out")

    def handleMessage(self, msg):
        EV << 'Received msg {}, sending it out again\n'.format(msg.getName())
        self.send(msg, "out")
