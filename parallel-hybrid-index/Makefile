#bin/bash
SDSL_DIR=/home/wrathiontbp/Parallel-Locate/sdsl-lite
RMQ_DIR=/home/wrathiontbp/Parallel-Locate/rmq

CPP=g++
CPPFLAGS=-std=c++11 -O3 -DNDEBUG -march=native -fopenmp
INCLUDES=-I$(SDSL_DIR)/include/ -I$(RMQ_DIR)/includes/ 
BUILD = -c src/build/src/*.cpp -c src/build/entrypoint.cpp
LOAD = -c src/load/src/*.cpp -c src/load/entrypoint.cpp
LOCATE = -c src/locate/src/*.cpp -c src/locate/entrypoint.cpp
UTILS = -c src/utils/*.cpp
LIB=$(SDSL_DIR)/lib/libsdsl.a $(SDSL_DIR)/lib/libdivsufsort.a $(SDSL_DIR)/lib/libdivsufsort64.a $(RMQ_DIR)/rmqrmmBP.a

LOCATE_OBJECTS = bins/utils/*.o bins/load/*.o bins/locate/*.o
BUILD_OBJECTS = bins/utils/*.o bins/build/*.o

NVCC = nvcc -arch=sm_50
CUDA_LIB = -L/opt/cuda/lib64 -lcudart
all:
	$(CPP) $(CPPFLAGS) $(INCLUDES) $(UTILS) $(BUILD) -c main_build.cpp -o build $(LIB)
	$(CPP) $(CPPFLAGS) $(INCLUDES) $(UTILS) $(LOAD) $(LOCATE) main_locate.cpp -o locate $(LIB)
	
locate_binary:
	$(CPP) $(CPPFLAGS) $(INCLUDES) -c main_locate.cpp -o bins/locate/main_locate.o
	$(CPP) $(CPPFLAGS) $(INCLUDES) $(LOCATE_OBJECTS) -o locate $(CUDA_LIB) $(LIB) 

build_binary:
	$(CPP) $(CPPFLAGS) $(INCLUDES) -c main_build.cpp -o bins/build/main_build.o
	$(CPP) $(CPPFLAGS) $(INCLUDES) $(BUILD_OBJECTS) -o build $(LIB)

locate_objects:
	$(CPP) $(CPPFLAGS) $(INCLUDES) $(LOCATE)
	mv *.o bins/locate

build_objects:
	$(CPP) $(CPPFLAGS) $(INCLUDES) $(BUILD)
	mv *.o bins/build

load_objects:
	$(NVCC) -c src/load/src/prepare_gpu.cu
	$(CPP) $(CPPFLAGS) $(INCLUDES) $(LOAD)
	mv *.o bins/load

utils_objects:
	$(CPP) $(CPPFLAGS) $(INCLUDES) $(UTILS)
	mv *.o bins/utils	

objects:
	$(CPP) $(CPPFLAGS) $(INCLUDES) $(LOAD)
	mv *.o bins/load
	$(CPP) $(CPPFLAGS) $(INCLUDES) $(BUILD)
	mv *.o bins/build
	$(CPP) $(CPPFLAGS) $(INCLUDES) $(LOCATE)
	mv *.o bins/locate
	$(CPP) $(CPPFLAGS) $(INCLUDES) $(UTILS)
	mv *.o bins/utils
