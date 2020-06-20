"""Bindings related to cMessage."""
from . import _pybind
from ._refstore import _RefStore
from ._utils import no_binding_for_method


class cMessage(_pybind._cMessage):

    def __init__(self, name=None, kind=0):
        _pybind._cMessage.__init__(self, name, kind)
        _RefStore.save(self)

    def __del__(self):
        print('__del__', self.__class__.__name__, self.getName())

    def dup(self):
        copy = super().dup()
        _RefStore.save(copy)
        return copy

    @no_binding_for_method
    def setContextPointer(self, *args, **kwargs):
        pass

    @no_binding_for_method
    def getContextPointer(self, *args, **kwargs):
        pass

    @no_binding_for_method
    def getParList(self, *args, **kwargs):
        pass

    @no_binding_for_method
    def addPar(self, *args, **kwargs):
        pass

    @no_binding_for_method
    def findPar(self, *args, **kwargs):
        pass

    @no_binding_for_method
    def hasPar(self, *args, **kwargs):
        pass

    @no_binding_for_method
    def addObject(self, *args, **kwargs):
        pass

    @no_binding_for_method
    def getObject(self, *args, **kwargs):
        pass

    @no_binding_for_method
    def hasObject(self, *args, **kwargs):
        pass

    @no_binding_for_method
    def removeObject(self, *args, **kwargs):
        pass


cMessage.__doc__ = _pybind._cMessage.__doc__
cMessage.dup.__doc__ = _pybind._cMessage.dup.__doc__
