#include <iostream>
#include "Monitor.hpp"
#include <signal.h>
using namespace std;


Monitor::Monitor()
{
	this->communicator = new Communicator();	
	this->log("Monitor: object created. ");
	this->init(0, NULL);

}

Monitor::Monitor(int argc, char **argv)
{
	this->communicator = new Communicator();	
	this->log("Monitor: object created. ");
	this->init(argc, argv);
}

Monitor::~Monitor()
{
	this->finalize();
	this->log("Monitor: object destroyed.");
}

void Monitor::log(string text)
{
	this->communicator->log(text);
};


void Monitor::init(int argc, char **argv)
{
	this->log("Monitor: initializing...");
	this->communicationThread = NULL;
	this->communicator->init(argc, argv);
	
	// send START
	this->communicationMutex.lock();
	
	this->communicationMutex.unlock();
	
	this->communicationThread = new thread(&Monitor::communicationLoop,this);	
	this->log("Monitor: Communication loop started.");
	this->log("Monitor: initialized. ");

}

void Monitor::finalize()
{
	communicator->close();
	this->log("Monitor: MPI finalized. ");
}

void Monitor::communicationLoop()
{
	
}

void Monitor::lockMutex(int mutexId) 
{
}

void Monitor::unlockMutex(int mutexId)
{
}
