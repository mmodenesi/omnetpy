#include <string>

#include <pybind11/pybind11.h>

#include <omnetpp/clog.h>
#include <omnetpp/cwatch.h>
#include <omnetpp/cobject.h>


/*
 * a custom class to replicate WATCH behavior from python side
 */
class PycReadOnlyWatch : public omnetpp::cWatchBase {
  public:
    pybind11::handle h;  // a python object
    std::string attr;  // the name of the attribute we want to watch

    PycReadOnlyWatch(const char *name, pybind11::handle obj)
        : omnetpp::cWatchBase(name), h(obj), attr(name) {}
    virtual bool supportsAssignment() const override { return false; }
    virtual std::string str() const override { return pybind11::str(h.attr(attr.c_str())); }
};


class PyLogProxy {
  private:
      omnetpp::cObject * thisPtr;
      std::string filename;
      int lineno;
      std::string func;
      omnetpp::LogLevel logLevel;
      std::string category;

  public:
    PyLogProxy(
        omnetpp::cObject * _thisPtr,
        std::string _filename,
        int _lineno,
        std::string _func,
        omnetpp::LogLevel _logLevel)
        : thisPtr(_thisPtr),
          filename(_filename),
          lineno(_lineno),
          func(_func),
          logLevel(_logLevel) {}

    void doLog(std::string msg)
    {
        ((void)0, !(COMPILETIME_LOG_PREDICATE(thisPtr, logLevel, category.c_str()) && \
        omnetpp::cLog::runtimeLogPredicate(thisPtr, logLevel, category.c_str()))) ? \
            omnetpp::cLogProxy::dummyStream : \
            omnetpp::cLogProxy(
                thisPtr,
                logLevel,
                category.c_str(),
                filename.c_str(),
                lineno,
                func.c_str()).getStream()
            << msg;
    }
};


/*
 * bindings for EV macro
 */
void bind_EV(pybind11::module & m) {

    // bind omnetpp::LogLevel enumeration
    pybind11::enum_<omnetpp::LogLevel>(m, "_LogLevel")
        .value("LOGLEVEL_TRACE", omnetpp::LogLevel::LOGLEVEL_TRACE)
        .value("LOGLEVEL_DEBUG", omnetpp::LogLevel::LOGLEVEL_DEBUG)
        .value("LOGLEVEL_DETAIL", omnetpp::LogLevel::LOGLEVEL_DETAIL)
        .value("LOGLEVEL_INFO", omnetpp::LogLevel::LOGLEVEL_INFO)
        .value("LOGLEVEL_WARN", omnetpp::LogLevel::LOGLEVEL_WARN)
        .value("LOGLEVEL_ERROR", omnetpp::LogLevel::LOGLEVEL_ERROR)
        .value("LOGLEVEL_FATAL", omnetpp::LogLevel::LOGLEVEL_FATAL)
        .value("LOGLEVEL_OFF", omnetpp::LogLevel::LOGLEVEL_OFF)
        .export_values();

    pybind11::class_<PyLogProxy>(m, "_PyLogProxy")
        .def(pybind11::init<omnetpp::cObject *, std::string, int, std::string, omnetpp::LogLevel>())
        .def("doLog", &PyLogProxy::doLog);
}


void bind_WATCH(pybind11::module &m) {

    m.def(
        "_createWatch",
        [] (const char *varname, pybind11::handle o)
        {
            new PycReadOnlyWatch(varname, o);
        }
    );
}
