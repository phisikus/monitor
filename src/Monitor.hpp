#include "message/Message.hpp"
#include <pthread.h>
#include <mpi.h>
#include <sys/types.h>
#define DEBUG
class Monitor
{
	public:
		Monitor();
		Monitor(int argc, char **argv);
		~Monitor();
		void lockMutex(int mutexId);
		void unlockMutex(int mutexId);

	private:
		int monitorId;
		char *monitorName;
		pthread_t *communicationThread; 

		void init(int argc, char **argv);
		void communication_loop();
		void finalize();


};	
