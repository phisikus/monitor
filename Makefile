all: clean compile
build: clean compile
clean:
	rm -fr ./bin/*
	rm -fr ./src/*.gch

compile:
	mpic++  src/condition/ConditionVariable.* src/mutex/Mutex.* src/message/Message.* src/message/Communicator.* src/Monitor.* main.cpp -o ./bin/monitor -lpthread -Wall -std=c++0x

run:
	mpirun -n 15 ./bin/monitor	
