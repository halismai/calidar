CXX = c++
CXX_VERSION = $(strip $(shell g++ -dumpversion | cut -f1,2 -d.))

#
# edit this to point to point to your matlab installation directory if needed
#

MATLAB_EXE  = matlab
MATLAB_ROOT = $(shell $(MATLAB_EXE) -e | grep MATLAB= | cut -f2 -d=)

###### the rest should work without modifications ######
MEX 	      = mex
MEX_SUFFIX  = $(shell mexext)
MEX_FLAGS   = -cxx CC='$(CXX)' CXX='$(CXX)' LD='$(CXX)'
MEX_OUT_DIR = bin


CXX_FLAGS = -I. -I$(MATLAB_ROOT)/extern/include       	      \
            -I/usr/local/include/                             \
            -DMEXMATWITH_EIGEN  -DMEXMAT_WITH_NANOFLANN

# path to use installation of mexmat
CXX_FLAGS += -I/home/halismai/code/mexmat/
CXX_FLAGS += -Wall -fPIC -fopenmp -mtune=native -O3 -std=c++11
CXX_FLAGS += -malign-double -ftree-vectorize
CXX_FLAGS += -I/usr/include/mpi/
CXX_FLAGS += `pkg-config --cflags eigen3`
CXX_FLAGS += `pkg-config --cflags nanoflann`
CXX_FLAGS += -Wno-unused-local-typedefs # for gcc-4.8

LD_FLAGS  += -lm -lgomp -L/usr/local/lib -Lmex/

# comment out this if you do not have tbb
CXX_FLAGS += -DMEXMAT_WITH_TBB
LD_FLAGS  += `pkg-config --libs tbb`

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

