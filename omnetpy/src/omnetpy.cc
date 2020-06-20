#include <omnetpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>

void * InterpreterManager::interpreter;

void InterpreterManager::ensureInterpreter() {
  if (!interpreter)
      interpreter = new pybind11::scoped_interpreter();
}
