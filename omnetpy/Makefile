# makefile for omnetpy tiny libary and the pyopp python package

INC_OMNETPP= -I$(OMNETPP_ROOT)/include
LIB_OMNETPP_DIR= $(OMNETPP_ROOT)/lib

INC_OMNETPY= -I$(OMNETPY_ROOT)/include
LIB_OMNETPY_DIR= $(OMNETPY_ROOT)/lib

INC_PYTHON=$(shell python3 -m pybind11 --includes)

TGT_LIB=$(LIB_OMNETPY_DIR)/libomnetpy.so


CXX=g++
CXXFLAGS=-std=c++14 -O3 -fPIC
LDFLAGS= -shared -fPIC -Wl,-rpath,$(LIB_OMNETPP_DIR) -Wl,-rpath,/lib -Wl,-rpath,. -Wl,--export-dynamic -L$(LIB_OMNETPP_DIR)

O=out
SOURCES = $(wildcard src/*.cc)
OBJS = $(SOURCES:%.cc=$(O)/%.o)


all: $(TGT_LIB) bindings


bindings:
	make -C bindings


$(TGT_LIB): $(O)/omnetpy.o $(LIB_OMNETPY_DIR)
	@echo "Creating $@"
	@$(CXX) $(LDFLAGS) -o $(TGT_LIB) $(O)/omnetpy.o


$(O)/%.o: $(O) $(SOURCES)
	@echo "Compiling omnetpy.cc"
	@$(CXX) -c $(SOURCES) $(CXXFLAGS) $(INC_OMNETPP) $(INC_PYTHON) $(INC_OMNETPY) -o $(O)/omnetpy.o


$(SOURCES):


$(O):
	@mkdir -p $(O)


$(LIB_OMNETPY_DIR):
	@mkdir -p $(LIB_OMNETPY_DIR)

clean:
	@echo "Cleaning omnetpy"
	@rm -rf $(LIB_OMNETPY_DIR)
	@rm -rf $(O)


.PHONY: clean bindings
