from pyopp import cSimpleModule, cMessage


class PyTxc1(cSimpleModule):

    def initialize(self):
        if self.getName() == 'tic':
            msg = cMessage("pymessage")
            self.send(msg, "out")

    def handleMessage(self, msg):
        self.send(msg, "out")
