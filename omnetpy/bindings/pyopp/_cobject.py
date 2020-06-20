from . import _pybind

from ._utils import no_binding_for_method


class cObject(_pybind._cObject):

    @no_binding_for_method
    def getThisPtr(self, *args, **kwargs):
        pass

    @no_binding_for_method
    def dup(self, *args, **kwargs):
        pass

    @no_binding_for_method
    def take(self, *args, **kwargs):
        pass

    @no_binding_for_method
    def drop(self, *args, **kwargs):
        pass

    @no_binding_for_method
    def dropAndDelete(self, *args, **kwargs):
        pass


cObject.__doc__ = _pybind._cObject.__doc__
