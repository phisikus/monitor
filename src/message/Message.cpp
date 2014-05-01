#include "Message.hpp"
#include <string.h>
#include <iostream>
using namespace std;

Message::Message()
{
    // nothing to do here.
};

Message::Message(MessageDTO *dto)
{
    this->clock = dto->clock;
    this->senderId = dto->senderId;
    this->recipientId = dto->recipientId;
    this->type = dto->type;
    this->referenceId = dto->referenceId;
    this->hasData = dto->hasData;
    this->dataSize = dto->dataSize;
    this->data = (char*) malloc(this->dataSize);
    memcpy(this->data, dto->data, this->dataSize);
};

Message::Message(Message *msg)
{
    this->clock = msg->clock;
    this->senderId = msg->senderId;
    this->recipientId = msg->recipientId;
    this->type = msg->type;
    this->referenceId = msg->referenceId;
    this->hasData = msg->hasData;
    this->dataSize = msg->dataSize;
    this->data = (char*) malloc(this->dataSize);
    memcpy(this->data, msg->data, this->dataSize);
};

void* Message::getArray()
{
    MessageDTO *m = (MessageDTO*) malloc(this->getArraySize());
    m->clock = this->clock;
    m->senderId = this->senderId;
    m->recipientId = this->recipientId;
    m->type = this->type;
    m->referenceId = this->referenceId;
    m->hasData = this->hasData;
    m->dataSize = this->dataSize;
    for(unsigned int i=0; i < this->dataSize; i++)
    {
        m->data[i] = (char) this->data[i];
    }
    return m;
}

long Message::getArraySize()
{
    return sizeof(MessageDTO) + this->dataSize - 1;
}

bool Message::operator<(Message& b) {
    return this->cmp(this, &b);
}


bool Message::cmp(Message *a, Message *b)
{
    if(a->clock == b->clock)
        return (a->senderId < b->senderId);

    return (a->clock < b->clock);
}
