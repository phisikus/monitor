#include "mutex/Mutex.hpp"
#include "message/Message.hpp"
#include "message/Communicator.hpp"
#include <string>
#include <thread>
#include <mutex>
#include <vector>
#include <sys/types.h>
using namespace std;


class Monitor
{
	public:
		Communicator *communicator;

		Monitor();
		Monitor(int argc, char **argv);
		~Monitor();
		void log(LogLevel level, string text);
		void lock(Mutex *mutex);
		void unlock(Mutex *mutex);
		void finalize();

	private:
		thread *communicationThread; 

		void init(int argc, char **argv);
		void communicationLoop();
		void enterCriticalSection(Mutex *m);
		
		


};	
