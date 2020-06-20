"""Bindings related to cDatarateChannel."""
from . import _pybind
from ._refstore import _RefStore


class cDatarateChannel(_pybind._cDatarateChannel):

    @staticmethod
    def create(*args, **kwargs):
        instance = _pybind._cDatarateChannel.create(*args, **kwargs)
        _RefStore.save(instance)
        return instance

    def __init__(self, *args, **kwarg):
        _pybind._cDatarateChannel.__init__(self, *args, **kwargs)
        _RefStore.save(self)

    def __del__(self):
        pass
        # print('__del__', self.getName())


cDatarateChannel.__doc__ = _pybind._cDatarateChannel.__doc__
cDatarateChannel.create.__doc__ = _pybind._cDatarateChannel.create.__doc__
