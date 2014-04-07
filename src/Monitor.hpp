#include "message/Message.hpp"
#include <string>
#include <thread>
#include <mpi.h>
#include <sys/types.h>
using namespace std;

#define DEBUG

class Monitor
{
	public:
		int monitorId;
		char *monitorName;
		
		Monitor();
		Monitor(int argc, char **argv);
		~Monitor();
		void log(string text);
		void lockMutex(int mutexId);
		void unlockMutex(int mutexId);

	private:
		thread *communicationThread; 

		void init(int argc, char **argv);
		void communicationLoop();
		void finalize();


};	
