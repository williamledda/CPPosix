INCLUDE_PATH=../include
LD_PATH=../Debug
LDFLAGS=-lcpposix

#CXX=g++
CXXFLAGS = -I${INCLUDE_PATH} -L${LD_PATH} ${LDFLAGS} -g

all: bin/threadp_test bin/mutexp_test

clean:
	-rm -f bin/*
	
bin/threadp_test: threadp_test.cpp
	LD_RUN_PATH=${LD_PATH} ${CXX} -o ../bin/threadp_test threadp_test.cpp $(CXXFLAGS)

bin/mutexp_test: mutexp_test.cpp
	LD_RUN_PATH=${LD_PATH} ${CXX} -o ../bin/mutexp_test mutexp_test.cpp $(CXXFLAGS)