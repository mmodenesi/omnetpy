"""Bindings for observability related classes and functions."""
import inspect
import os

from . import _pybind


class WATCHException(ValueError):
    """Raised when WATCH is not correctly configured."""


class WATCH:
    def __init__(self, attrname, thisptr=None):
        stack = inspect.stack()
        if not thisptr:
            try:
                thisptr = stack[1].frame.f_locals["self"]  # getThisPtr()
            except KeyError as e:
                msg = 'Unable to identify object to perform WATCH({})'.format(attrname)
                raise WATCHException(msg)

            attrpath = attrname.split('.')
            obj = thisptr
            attrname = attrpath.pop(0)
            while len(attrpath) > 1:
                attrname = attrpath.pop(0)
                if not hasattr(obj, attrname):
                    msg = '{} does not have {} attribute'.format(obj, attrname)
                    raise WATCHException(msg)
                obj = getattr(obj, attrname)

        _pybind._createWatch(attrname, obj)
        # allow GC to sweep this quickly
        stack = None
        thisptr = None
        obj = None


class EV:
    def __init__(self, level):
        self.level = level

    def __lshift__(self, msg):
        stack = inspect.stack()
        thisptr = None
        try:
            thisptr = stack[1].frame.f_locals["self"]  # getThisPtr()
        except KeyError:
            # no self
            pass

        _pybind._PyLogProxy(
            thisptr,
            os.path.basename(stack[1].filename),
            stack[1].lineno,
            stack[1].function,
            self.level
        ).doLog(str(msg))

        # allow GC to sweep this quickly
        thisptr = None
        stack = None

        return self


EV_FATAL = EV(_pybind._LogLevel.LOGLEVEL_FATAL)
EV_ERROR = EV(_pybind._LogLevel.LOGLEVEL_ERROR)
EV_WARN = EV(_pybind._LogLevel.LOGLEVEL_WARN)
EV_INFO = EV(_pybind._LogLevel.LOGLEVEL_INFO)
EV_DETAIL = EV(_pybind._LogLevel.LOGLEVEL_DETAIL)
EV_DEBUG = EV(_pybind._LogLevel.LOGLEVEL_DEBUG)
EV_TRACE = EV(_pybind._LogLevel.LOGLEVEL_TRACE)
EV = EV_INFO
