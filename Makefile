CXX=g++

MATLAB_ROOT = $(shell $(MEX) -v 2>&1 | sed -n 's/.*MATLAB *= *\(.*\)/\1/gp')
MEX = mex
MEX_SUFFIX = $(shell mexext)
MEX_FLAGS  = -cxx CC='$(CXX)' CXX='$(CXX)' LD='$(CXX)' -largeArrayDims -O
MEX_OUT_DIR = bin

CXX_FLAGS = -I. -I$(MATLAB_ROOT)/extern/include       	      \
            -I/usr/local/include/                             \
            -DMEXMAT_WITH_TBB -DMEXMATWITH_EIGEN  -DMEXMAT_WITH_NANOFLANN
CXX_FLAGS += -Wall -fPIC -fopenmp -mtune=native -mavx -O3 -std=c++11
CXX_FLAGS += -malign-double -ftree-vectorize
CXX_FLAGS += -I/usr/include/mpi/
CXX_FLAGS += `pkg-config --cflags eigen3`

LD_FLAGS  += -lm -lgomp -L/usr/local/lib -lmexmat -Lmex/
LD_FLAGS  += -lboost_system -lboost_filesystem `pkg-config --libs tbb`

MEX_SRC := $(shell find . -name "*_mex.cc")
LIB_SRC := $(filter-out $(MEX_SRC), $(shell find . -name "*.cc"))

all: $(patsubst %.cc,%.$(MEX_SUFFIX), $(MEX_SRC))

%.$(MEX_SUFFIX):%.o
	$(MEX) $(MEX_FLAGS) $(LD_FLAGS) $^ -output $@

%.o: %.cc
	$(CXX) $(CXX_FLAGS) -c $< -o $@

.PHONY: clean
clean:
	@$(shell find . -name "*.o" -o -name  "*.$(MEX_SUFFIX)" -exec rm -rf {} \;)

