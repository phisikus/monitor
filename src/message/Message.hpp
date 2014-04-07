#include "MessageType.hpp"

class Message
{
	public:
		long messageId;
		int senderId;
		MessageType type;
		long referenceId;
		bool hasData;
		long dataSize;
		void *data;		
		bool cmp(Message *a, Message *b);

	private:
};
