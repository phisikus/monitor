#include <iostream>
#include <mpi.h>
#include "Message.hpp"
using namespace std;

#define DEBUG

class Communicator
{
	public:	
		int processId;
		char *processName;

		bool initialized = false;
		void init(int argc, char **argv);
		void sendMessage(Message *msg);
		Message* recvMessage();
		void log(string text);
		void close();				
};
