#include <iostream>
#include <mpi.h>
#include <vector>
#include <mutex>
#include "Message.hpp"
using namespace std;

#define DEBUG

enum LogLevel { TRACE, INFO, ERROR };

class Communicator
{
	public:	
		int processId;
		char *processName;
		int processCount;
		vector<bool> activePeers;
		long clock = 0;

		bool initialized = false;
		void init(int argc, char **argv);
		void sendMessage(Message *msg);
		void sendBroadcast(Message *msg);
		Message* recvMessage();
		void log(LogLevel level, string text);
		void close();
		
		mutex * getCommunicationMutex();
	private: 
		mutex communicationMutex;				
};
