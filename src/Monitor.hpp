#include "message/Message.hpp"
#include "message/Communicator.hpp"
#include <string>
#include <thread>
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
		void log(string text);
		void lockMutex(int mutexId);
		void unlockMutex(int mutexId);
		void finalize();

	private:
		thread *communicationThread; 
		
		void init(int argc, char **argv);
		void communicationLoop();
		void sendMessage(Message msg);
		Message* recvMessage();
		


};	
