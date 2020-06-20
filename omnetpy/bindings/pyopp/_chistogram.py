""" Bindings for cHisgogram."""
from . import _pybind
from ._refstore import _RefStore

cHistogram = _pybind._cHistogram
cDefaultHistogramStrategy = _pybind._cDefaultHistogramStrategy
cFixedRangeHistogramStrategy = _pybind._cFixedRangeHistogramStrategy
cAutoRangeHistogramStrategy = _pybind._cAutoRangeHistogramStrategy


class cDefaultHistogramStrategy(_pybind._cDefaultHistogramStrategy):

    def __init__(self, *args, **kwargs):
        _pybind._cDefaultHistogramStrategy.__init__(self, *args, **kwargs)
        _RefStore.save(self)


class cAutoRangeHistogramStrategy(_pybind._cAutoRangeHistogramStrategy):

    def __init__(self, *args, **kwargs):
        _pybind._cAutoRangeHistogramStrategy.__init__(self, *args, **kwargs)
        _RefStore.save(self)


class cFixedRangeHistogramStrategy(_pybind._cFixedRangeHistogramStrategy):

    def __init__(self, *args, **kwargs):
        _pybind._cFixedRangeHistogramStrategy.__init__(self, *args, **kwargs)
        _RefStore.save(self)
