from pyopp import cPacket


class HCPacket(cPacket):

    def __init__(self, name, kind=0):
        cPacket.__init__(self, name, kind)
        self.srcAddress = None
        self.destAddress = None
        self.hops = 0
