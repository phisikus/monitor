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
	Message *msg = new Message();
	msg->type = START;
	msg->data = this->communicator->processName;
	msg->dataSize = strlen(this->communicator->processName);
	communicator->sendBroadcast(msg);
	
	this->communicationThread = new thread(&Monitor::communicationLoop,this);	
		
	this->log("Monitor: Communication loop started.");
	this->log("Monitor: initialized. ");

}

void Monitor::finalize()
{		
	Message *msg = new Message();
	msg->type = QUIT;
	communicator->sendBroadcast(msg);
	
	communicator->close();
	this->log("Monitor: MPI finalized. ");
}

void Monitor::communicationLoop()
{
	Message *msg;
	while(true)
	{			
		msg = communicator->recvMessage();		
		
		if(msg == NULL)
			break;
			
		this->log("Received: " + toString(msg->type) + " from: " + to_string(msg->senderId));							
		
		switch(msg->type)
		{
			case QUIT:				
				communicator->activePeers[msg->senderId] = false;				
				break;
							
				
			default:
				break;
			
		}
		
	}
	
}

void Monitor::lockMutex(Mutex mutexId) 
{
}

void Monitor::unlockMutex(Mutex mutexId)
{
}
