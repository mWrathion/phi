SDSL_DIR=/home/wrathiontbp/magister
INCLUDES=-I$(SDSL_DIR)/include/
LIB=$(SDSL_DIR)/lib/libsdsl.a $(SDSL_DIR)/lib/libdivsufsort.a $(SDSL_DIR)/lib/libdivsufsort64.a

all:
	nvcc -arch=sm_75 -c kernel.cu -o kernel.o
	g++ -std=c++11 -fopenmp $(INCLUDES) -c main.cpp -o main.o 
	g++ -std=c++11 -fopenmp $(INCLUDES) kernel.o main.o -o exe -L/opt/cuda/lib64 -lcudart $(LIB)


