all: clean compile
build: clean compile
clean:
	rm -fr ./bin/*
	rm -fr ./src/*.gch

compile:
	mpic++ src/message/Message.* src/message/Communicator.* src/Monitor.* main.cpp -o ./bin/monitor -std=c++11 -lpthread -Wall	

run:
	mpirun -n 10 ./bin/monitor	
