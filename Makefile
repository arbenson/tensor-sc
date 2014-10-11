CXX = /usr/local/Cellar/llvm/3.3/bin/clang++
SNAP_DIR = /Users/benson/Desktop/Snap-2.3


SNAP_INCLUDES = -I$(SNAP_DIR)/snap-core -I$(SNAP_DIR)/snap-adv -I$(SNAP_DIR)/snap-exp -I$(SNAP_DIR)/glib-core
SNAP_OBJ = $(SNAP_DIR)/snap-core/Snap.o
INCLUDES := -I. -Iinclude $(SNAP_INCLUDES)
#DEFINES := -DNDEBUG
DEFINES +=  -D_VERBOSE_

#DEBUG := -g
OPT := -O3
CFLAGS := $(OPT) $(DEBUG) $(INCLUDES) $(DEFINES) -std=c++11 $(DEFINES)

LDFLAGS := -O3
LDLIBS := $(BLAS_LAPACK_LIB)

vpath %.cpp src

SRC = common.cpp \
      file_io.cpp \
      snap_tools.cpp \
      community.cpp \
      network.cpp \
      tensor_ops.cpp \
      laplacian.cpp \
      cut_tools.cpp

OBJECTS = $(SRC:.cpp=.o)

.PHONY : default
default : all

.PHONY : all

d3c_test: $(OBJECTS) d3c_test.cpp
	$(CXX) $(CFLAGS) d3c_test.cpp $(LDFLAGS) $(SNAP_OBJ) $(OBJECTS) $(LDLIBS) -o d3c_test

d3c_cocluster: $(OBJECTS) d3c_cocluster.cpp
	$(CXX) $(CFLAGS) d3c_cocluster.cpp $(LDFLAGS) $(SNAP_OBJ) $(OBJECTS) $(LDLIBS) -o d3c_cocluster

d3c_scores: $(OBJECTS) d3c_scores.cpp
	$(CXX) $(CFLAGS) d3c_scores.cpp $(LDFLAGS) $(SNAP_OBJ) $(OBJECTS) $(LDLIBS) -o d3c_scores

small_flow: $(OBJECTS) small_flow.cpp
	$(CXX) $(CFLAGS) small_flow.cpp $(LDFLAGS) $(SNAP_OBJ) $(OBJECTS) $(LDLIBS) -o small_flow

layered_flow: $(OBJECTS) layered_flow.cpp
	$(CXX) $(CFLAGS) layered_flow.cpp $(LDFLAGS) $(SNAP_OBJ) $(OBJECTS) $(LDLIBS) -o layered_flow

tripartite_flow: $(OBJECTS) tripartite_flow.cpp
	$(CXX) $(CFLAGS) tripartite_flow.cpp $(LDFLAGS) $(SNAP_OBJ) $(OBJECTS) $(LDLIBS) -o tripartite_flow

anomaly: $(OBJECTS) anomaly.cpp
	$(CXX) $(CFLAGS) anomaly.cpp $(LDFLAGS) $(SNAP_OBJ) $(OBJECTS) $(LDLIBS) -o anomaly

all: d3c_test d3c_cocluster d3c_scores small_flow layered_flow tripartite_flow anomaly

%.o: %.cpp
	$(CXX) -c $(CFLAGS) $<

.PHONY : clean
clean :
	rm -rf *.o *~ d3c_test d3c_cocluster d3c_scores small_flow layered_flow tripartite_flow anomaly
