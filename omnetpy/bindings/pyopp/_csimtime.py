"""Bindings related to simulation time."""

from . import _pybind

SimTimeUnit = _pybind._SimTimeUnit

simTime = _pybind._simTime


SimTime = _pybind._SimTime

SIMTIME_ZERO = SimTime.ZERO
SIMTIME_MAX = SimTime.getMaxTime()
