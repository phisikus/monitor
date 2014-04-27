#include "Message.hpp"

Message::Message(char *fromArray)
{
	// todo - fill object with data from array;
};

char* Message::getArray()
{
	// todo - return array with object data;
}

long Message::getArraySize()
{
	// todo - implement
	return 0;
}

bool Message::cmp(Message *a, Message *b)
{
	if(a->messageId == b->messageId)
		return (a->senderId < b->senderId);
	
	return (a->messageId < b->messageId);	
};
