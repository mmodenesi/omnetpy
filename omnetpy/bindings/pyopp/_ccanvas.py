"""Bindings related to cFigure."""
from . import _pybind
from ._utils import no_binding_for_method

cRingFigure = _pybind._cRingFigure
cOvalFigure = _pybind._cOvalFigure
cPolygonFigure = _pybind._cPolygonFigure
cFigure = _pybind._cFigure


class _ColorArray:
    def __init__(self, f):
        self._f = f

    def __getitem__(self, index):
        return self._f(index)


cFigure.GOOD_LIGHT_COLORS = _ColorArray(_pybind._getGoodLightColors)
cFigure.GOOD_DARK_COLORS = _ColorArray(_pybind._getGoodDarkColors)


cFigure.__doc__ = _pybind._cFigure.__doc__
cFigure.Point.buffer = no_binding_for_method(lambda self: None)
