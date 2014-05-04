#ifndef INCLUDE_MESSAGE_HPP
#define INCLUDE_MESSAGE_HPP
#include "MessageType.hpp"

struct MessageDTO
{
	long clock;
	int senderId;
	int recipientId;
	MessageType type;
	long referenceId;
	bool hasData;
	long dataSize;
	char data[1]; // fake size of array
};


class Message
{
	public:
		long clock;
		int senderId;
		int recipientId;
		MessageType type;
		long referenceId;
		bool hasData = false;
		long dataSize = 0;
		char *data = NULL;

		bool operator<(Message& b);
		bool cmp(Message *a, Message *b);
		void* getArray();
		long getArraySize();

		Message();
		Message(Message *msg);
		Message(MessageDTO *dto);
};


#endif
