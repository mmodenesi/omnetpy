"""Bindings for cComponentType."""
import weakref

from . import _pybind

def createScheduleInit(self, modname, who):
   return weakref.ref(_pybind._cModuleType._createScheduleInit(self, modname, who))()


createScheduleInit.__doc__ = _pybind._cModuleType.__doc__


cModuleType = _pybind._cModuleType
cModuleType.createScheduleInit = createScheduleInit
