all: clean compile
build: clean compile
clean:
	rm -fr ./bin/*
	rm -fr ./src/*.gch

compile:
	mpic++ src/mutex/Mutex.* src/message/Message.* src/condition/ConditionVariable.* src/message/Communicator.* src/Monitor.* main.cpp -o ./bin/monitor -lpthread -Wall -std=c++0x

run:
	mpirun -n 15 ./bin/monitor	
