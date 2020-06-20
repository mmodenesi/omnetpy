#ifndef __INTERPRETER_H
#define __INTERPRETER_H

#include <omnetpp.h>
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>


/*
 * Publicist to increase visibility of some methods
 */
class cSimpleModulePublicist : public omnetpp::cSimpleModule {

public:

    using omnetpp::cSimpleModule::activity;
    using omnetpp::cSimpleModule::handleMessage;
    using omnetpp::cSimpleModule::send;
    using omnetpp::cSimpleModule::sendDelayed;
    using omnetpp::cSimpleModule::sendDirect;
};


/*
 * Trampoline class to let python implement virtual methods
 */
class PycSimpleModule : public omnetpp::cSimpleModule
{

public:

    using omnetpp::cSimpleModule::cSimpleModule;

    void initialize() override
    {
        PYBIND11_OVERLOAD(void, omnetpp::cSimpleModule, initialize, );
    }

    void handleMessage(omnetpp::cMessage *msg) override
    {
        PYBIND11_OVERLOAD(void, omnetpp::cSimpleModule, handleMessage, msg);
    }

    void activity() override
    {
        PYBIND11_OVERLOAD(void, omnetpp::cSimpleModule, activity, );
    }

    void refreshDisplay() const override
    {
        PYBIND11_OVERLOAD(void, omnetpp::cSimpleModule, refreshDisplay, );
    }

    void finish() override
    {
        PYBIND11_OVERLOAD(void, omnetpp::cSimpleModule, finish, );
    }

    ~PycSimpleModule()
    {
        // decrease undelying python object to let python free its memory
        pybind11::handle h = pybind11::cast(this);
        h.dec_ref();
        // INFO("Destructing %s, underlying python object ref_count is %d", this->getFullPath().c_str(), h.ref_count());
    }
};

class InterpreterManager {
  private:
    static void* interpreter;
  public:
    static void ensureInterpreter();
};


// Define_Python_Module
#define Define_Python_Module(PYTHON_MODULE, CLASSNAME) \
    __REGISTER_PYTHON_CLASS(PYTHON_MODULE, CLASSNAME, omnetpp::cModule, "module")

//TODO review caster
#define __REGISTER_PYTHON_CLASS(PYTHON_MODULE, CLASSNAME, BASECLASS, DESC) \
    namespace {\
    static omnetpp::cObject *MAKE_UNIQUE_WITHIN_FILE(__factoryfunc_)() { auto py_module = pybind11::module::import(PYTHON_MODULE); pybind11::object obj = py_module.attr(CLASSNAME)(); obj.inc_ref(); return obj.cast<omnetpp::cObject *>(); }\
    static void *MAKE_UNIQUE_WITHIN_FILE(__castfunc_)(omnetpp::cObject *obj) {return (void*)dynamic_cast<PycSimpleModule*>(obj);}\
    EXECUTE_ON_STARTUP(InterpreterManager::ensureInterpreter(); omnetpp::classes.getInstance()->add(new omnetpp::cObjectFactory(CLASSNAME, MAKE_UNIQUE_WITHIN_FILE(__factoryfunc_), MAKE_UNIQUE_WITHIN_FILE(__castfunc_), DESC)); /* nothing */; )\
    }

#endif // __INTERPRETER_H
