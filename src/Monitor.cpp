#include <iostream>
#include "Monitor.hpp"
#include <signal.h>
using namespace std;


Monitor::Monitor()
{
#ifdef DEBUG
	cerr << "Monitor: object created. " << endl;
#endif	
	this->init(0, NULL);

}

Monitor::Monitor(int argc, char **argv)
{
#ifdef DEBUG
	cerr << "Monitor: object created. " << endl;
#endif	
	this->init(argc, argv);
}

Monitor::~Monitor()
{
	this->finalize();
#ifdef DEBUG	
	cerr << "Monitor: object destroyed." << endl;
#endif	
}

void Monitor::init(int argc, char **argv)
{
#ifdef DEBUG
	cerr << "Monitor: initializing..." << endl;
#endif

	// Initialize MPI enviroment
	this->monitorName = new char[MPI_MAX_PROCESSOR_NAME];
	MPI::Init_thread(argc, argv, MPI_THREAD_MULTIPLE);
	MPI_Comm_rank(MPI_COMM_WORLD, &(this->monitorId));
	MPI_Get_processor_name(this->monitorName, new int);
	this->communicationThread = (pthread_t) NULL;


#ifdef DEBUG
	cerr << "Monitor: initialized. hostname = " << this->monitorName << endl;
#endif

}

void Monitor::finalize()
{
	MPI::Finalize();
	if(this->communicationThread != (pthread_t) NULL)
		pthread_kill(*this->communicationThread,15); // SIGTERM

#ifdef DEBUG
	cerr << "Monitor: MPI finalized. " << endl;
#endif

}

void Monitor::communication_loop()
{
}

void Monitor::lockMutex(int mutexId) 
{
}

void Monitor::unlockMutex(int mutexId)
{
}
