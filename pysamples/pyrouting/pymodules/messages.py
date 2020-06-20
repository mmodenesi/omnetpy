from pyopp import cPacket


class Packet(cPacket):
    """Represents a packet in the network."""

    def __init__(self, name, srcAddr, destAddr, kind=0):
        cPacket.__init__(self, name, kind)
        self.srcAddr = srcAddr
        self.destAddr = destAddr
        self.hopCount = 0
