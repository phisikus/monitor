#include "Message.hpp"


bool Message:cmp(Message *a, Message *b)
{
	if(a->messageId == b->messageId)
		return (a->monitorId < b->monitorId);
	
	return (a->messageId < b->messageId);	
};
