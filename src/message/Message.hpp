#ifndef INCLUDE_MESSAGE_HPP
#define INCLUDE_MESSAGE_HPP
#include "MessageType.hpp"

class Message
{	
	public:
		long messageId;
		int senderId;
		int recipientId;
		MessageType type;
		long referenceId;
		bool hasData;
		long dataSize;
		void *data;		
		
		bool cmp(Message *a, Message *b);
		char* getArray();
		long getArraySize();
				
		Message(char *sourceArray);		
};

#endif
