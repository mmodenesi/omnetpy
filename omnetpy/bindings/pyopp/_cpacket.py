"""Binding for cPacket."""
from . import _pybind
from ._refstore import _RefStore


class cPacket(_pybind._cPacket):

    def __init__(self, name=None, kind=0):
        _pybind._cPacket.__init__(self, name, kind)
        _RefStore.save(self)

    def __del__(self):
        pass
        # print('__del__', self.getName())

    def dup(self):
        copy = super().dup()
        _RefStore.save(copy)
        return copy


cPacket.__doc__ = _pybind._cPacket.__doc__
cPacket.dup.__doc__ = _pybind._cPacket.dup.__doc__
