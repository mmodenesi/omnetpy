from pyopp import cSimpleModule, cMessage, simTime, SimTime, EV

SLOT_TIME = 1.0
PROPDEL = SimTime(0.99 * SLOT_TIME)
STACKSIZE = 16384


def deflectionRouting(my_address, dim, rte_dest, num_rte, rte_port, usr_dest, num_usr, usr_port):
    """ This function implements a simple, fast, but very suboptimal, unfair, unbalanced etc.
    deflection scheme."""
    i = 0

    port_mask = (1 << dim) - 1

    # used_ports: bits set to 1 will denote ports already assigned
    used_ports = 0

    # assign transit cells to ports
    for i in range(num_rte):
        optimal = my_address ^ rte_dest[i]
        usable = optimal & ~used_ports & port_mask

        if usable:
            # route through first usable port (with smallest index)
            k = 0
            while (usable & (1 << k)) == 0:
                k += 1
            rte_port[i] = k
            used_ports |= 1 << k
        else:
            # deflect through first free port (with smallest index)
            k = 0
            while used_ports & (1 << k):
                k += 1
            rte_port[i] = k
            used_ports |= 1 << k

    # assign user cells to remaining ports
    for i in range(num_usr):
        optimal = my_address ^ usr_dest[i]
        usable = optimal & ~used_ports & port_mask

        if usable:
            # route through first usable port (with smallest index)
            k = 0
            while (usable & (1<<k)) == 0:
                    k += 1
            usr_port[i] = k
            used_ports |= 1 << k
        elif used_ports != port_mask:
            # deflect through first free port (with smallest index)
            k = 0
            while used_ports & (1 << k):
                k += 1

            usr_port[i] = k
            used_ports |= 1 << k
        else:
            # all ports used, cell will be discarded
            usr_port[i] = -1


class HCRouter(cSimpleModule):

    """
    def __init__(self):
        cSimpleModule.__init__(self, STACKSIZE)
    """

    def initialize(self):
        self.myAddress = self.par("address").intValue()
        self.dim = self.par("dim").intValue()
        self.fromUserGateId = self.gate("fromGen").getId()
        self.total_usr = 0
        self.discarded_usr = 0

        self.event = cMessage('event')
        self.endOfSlot = cMessage("endOfSlot");

        self.scheduleAt(simTime(), self.event)

    def finish(self):
        if not self.endOfSlot.isScheduled():
            print(self, 'deleting', self.endOfSlot)
            self.delete(self.endOfSlot)
            self.endOfSlot = None
        if not self.event.isScheduled():
            print(self, 'deleting', self.event)
            self.delete(self.event)
            self.event = None

        # delete transit cells
        for i in range(self.num_rte):
            if not self.rte_cell[i].isScheduled():
                self.delete(self.rte_cell[i])
                self.rte_cell[i] = None

        # delete user cells
        for i in range(self.num_usr):
            if not self.usr_cell[i].isScheduled():
                self.delete(self.usr_cell[i])
                self.usr_cell[i] = None

    def handleMessage(self, msg):
        if msg == self.event:
            # buffers for transit cells (rte) and for cells from local user (usr)
            self.rte_cell = [None] * 32
            self.num_rte = 0;

            self.usr_cell = [None] * 32
            self.num_usr = 0

            # collect cells; user cells go into separate buffer
            self.scheduleAt(simTime() + SLOT_TIME, self.endOfSlot)

        elif msg is self.endOfSlot:
            # prepare arrays used in routing
            rte_dest = [-1] * 32  # destinations
            rte_port = [-1] * 32  # output ports
            usr_dest = [-1] * 32
            usr_port = [-1] * 32

            for i in range(self.num_rte):
                rte_dest[i] = self.rte_cell[i].destAddress
            for i in range(self.num_usr):
                usr_dest[i] = self.usr_cell[i].destAddress

            # make routing decision (function fills rte_port[] and usr_port[])
            deflectionRouting(
                self.myAddress, self.dim,
                rte_dest, self.num_rte, rte_port,
                usr_dest, self.num_usr, usr_port);

            # send out transit cells
            for i in range(self.num_rte):
                self.rte_cell[i].hops = self.rte_cell[i].hops + 1
                self.sendDelayed(self.rte_cell[i], PROPDEL, "out", rte_port[i])

            # send out user cells
            for i in range(self.num_usr):
                if usr_port[i] < 0:
                    self.discarded_usr += 1
                    self.delete(self.usr_cell[i])
                    self.usr_cell[i] = None
                else:
                    self.usr_cell[i].hops = self.usr_cell[i].hops + 1
                    self.sendDelayed(self.usr_cell[i], PROPDEL, "out", usr_port[i])
            EV << "rte[" << self.myAddress << "]: Discarded " \
               << self.discarded_usr << " out of " << self.total_usr << "\n"

            # to begin again
            self.scheduleAt(simTime(), self.event)
        else:
            # it is a packet
            pkt = msg
            if pkt.getArrivalGateId() != self.fromUserGateId:
                if pkt.destAddress != self.myAddress:
                    self.rte_cell[self.num_rte] = pkt
                    self.num_rte += 1
                else:
                    self.send(pkt, "toSink")
            else:
                self.total_usr += 1
                if self.num_usr < 32:
                    self.usr_cell[self.num_usr] = pkt
                    self.num_usr += 1
                else:
                    self.discarded_usr += 1
                    self.delete(pkt)
