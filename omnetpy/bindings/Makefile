HIDDEN_MODULE_NAME = _pybind
PUBLIC_MODULE_NAME = pyopp
TARGET = $(HIDDEN_MODULE_NAME)$(shell python3-config --extension-suffix)
INCLUDES = -I$(OMNETPP_ROOT)/include -I$(OMNETPY_ROOT)/include `python3 -m pybind11 --includes`
LIBFLAGS = -L$(OMNETPP_ROOT)/lib
CXXFLAGS = -O3 -Wall -fvisibility=hidden -shared -std=c++14 -fPIC
SOURCES = $(wildcard *.cc)
O = ./out

OBJS = $(SOURCES:%.cc=$(O)/%.o)

$(O)/$(TARGET): $(O) $(OBJS)
	@echo creating python module $@
	@g++ $(CXXFLAGS) $(LIBFLAGS) $(OBJS) -o $(O)/$(TARGET) -loppsim
	@echo copying $@
	-$(Q)cp  $(O)/$(TARGET) $(PUBLIC_MODULE_NAME)/$(TARGET)
	@echo
	@echo python module pyopp successfuly created
	@echo you can type "python3 -c 'import pyopp; print(dir(pyopp))'" to validate

$(O):
	@mkdir -p $(O)

$(O)/%.o: %.cc
	@echo Compiling $@
	@g++ $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@echo "Cleaning python bindings"
	@rm -rf $(PUBLIC_MODULE_NAME)/$(TARGET)
	@rm -rf $(O)

.PHONY: clean
