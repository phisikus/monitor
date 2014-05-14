#include <iostream>
#include <mpi.h>
#include <vector>
#include <mutex>
#include "Message.hpp"
using namespace std;

//#define DEBUG

enum LogLevel { TRACE, INFO, ERROR };

class Communicator
{
	public:
		int processId;
		char *processName;
		unsigned int processCount;
		long clock = 0;

		bool initialized = false;
		void init(int argc, char **argv);
		void sendMessage(Message *msg);
		void sendBroadcast(Message *msg);
		void waitForMessage();
		Message* recvMessage();
		void log(LogLevel level, string text);
		void barrier();
		void close();

		mutex * getCommunicationMutex();
	private:
		mutex communicationMutex;
};
