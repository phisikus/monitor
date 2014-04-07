#include <iostream>
#include "Monitor.hpp"
#include <signal.h>
using namespace std;


Monitor::Monitor()
{
	this->log("Monitor: object created. ");
	this->init(0, NULL);

}

Monitor::Monitor(int argc, char **argv)
{
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
	#ifdef DEBUG
	string message = "";
	if(this->monitorName != NULL) 
	{
		message += "[";
		message += this->monitorName;
		message += " " + std::to_string(this->monitorId) + "] ";
	}
	message += text;
	cout << message << endl;		
		
	#endif
};


void Monitor::init(int argc, char **argv)
{
	this->log("Monitor: initializing...");

	// Initialize MPI enviroment
	this->monitorName = new char[MPI_MAX_PROCESSOR_NAME];
	this->communicationThread = (pthread_t) NULL;
	MPI::Init_thread(argc, argv, MPI_THREAD_MULTIPLE);
	MPI_Comm_rank(MPI_COMM_WORLD, &(this->monitorId));
	MPI_Get_processor_name(this->monitorName, new int);	
	this->communicationThread = new thread(&Monitor::communicationLoop,this);

	this->log("Monitor: initialized. ");

}

void Monitor::finalize()
{
	MPI::Finalize();	
	this->log("Monitor: MPI finalized. ");

}

void Monitor::communicationLoop()
{
	this->log("Monitor: Communication loop started.");
}

void Monitor::lockMutex(int mutexId) 
{
}

void Monitor::unlockMutex(int mutexId)
{
}
