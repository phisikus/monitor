#include "Communicator.hpp"

void Communicator::init(int argc, char **argv)
{
	this->processName = new char[MPI_MAX_PROCESSOR_NAME];
	MPI::Init_thread(argc, argv, MPI_THREAD_MULTIPLE);
	MPI_Comm_rank(MPI_COMM_WORLD, &this->processId);
	MPI_Comm_size(MPI_COMM_WORLD, &this->processCount);
	MPI_Get_processor_name(this->processName, new int);
	this->activePeers.resize(this->processCount, true);
	this->activePeers[this->processId] = false;
	this->initialized = true;
};

void Communicator::close()
{
	if(this->initialized)
	{
		communicationMutex.lock();
		this->initialized = false;
		MPI::Finalize();
	}
}

void Communicator::log(LogLevel level, string text)
{
#ifndef DEBUG
	if(level != TRACE)
#endif
	{
		string message = "";
		if(this->processName != NULL)
		{
			message += "[";
			message += this->processName;
			message += " " + to_string(this->processId);
			message += " ; clock = " + to_string(this->clock) + " ";
			message += "] ";
		}
		message += text;
		cout << message << endl;
	}


}

void Communicator::sendMessage(Message *msg)
{
	if(!initialized) return;

	communicationMutex.lock();
	if((msg != NULL) && (this->activePeers[msg->recipientId]))
	{
		this->clock++;
		msg->senderId = this->processId;
		msg->clock = this->clock;
		this->log(TRACE,"Sending message " + toString(msg->type) + " to " + to_string(msg->recipientId) + " (size = " + to_string(msg->getArraySize()) + ", clock = " + to_string(msg->clock) + " )");
		MPI_Isend(msg->getArray(), msg->getArraySize(), MPI_CHAR, msg->recipientId, 0, MPI_COMM_WORLD, new MPI_Request());
	}
	communicationMutex.unlock();
}

void Communicator::sendBroadcast(Message *msg)
{

	if((!initialized) && (msg == NULL)) return;

	communicationMutex.lock();
	this->clock++;
	msg->clock = this->clock;
	msg->senderId = this->processId;


	for(unsigned int i = 0; i < activePeers.size(); i++)
	{

		msg->recipientId = i;

		if(msg->recipientId == this->processId)
			continue;

		this->log(TRACE,"Sending message " + toString(msg->type) + " to " + to_string(msg->recipientId) + " (size = " + to_string(msg->getArraySize()) + ", clock = " + to_string(msg->clock) + " )");
		MPI_Isend(msg->getArray(), msg->getArraySize(), MPI_CHAR, msg->recipientId, 0, MPI_COMM_WORLD, new MPI_Request());

	}
	communicationMutex.unlock();
}

void Communicator::waitForMessage()
{
	if(!initialized) return;

	MPI_Status status;
	MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
}


Message* Communicator::recvMessage()
{
	if(!initialized) return NULL;

	communicationMutex.lock();
	MPI_Status status;
	int packetSize;

	MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	MPI_Get_count(&status, MPI_CHAR, &packetSize);
	MessageDTO *packet = (MessageDTO *) malloc(packetSize);
	MPI_Recv(packet, packetSize, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

	Message *msg = new Message(packet);
	this->clock = max(this->clock, msg->clock + 1);
	communicationMutex.unlock();
	this->log(TRACE, "Received: " + toString(msg->type) + " from " + to_string(msg->senderId) + " (size = " + to_string(msg->getArraySize()) + ", clock = " + to_string(msg->clock) + " )");

	return msg;
}

mutex * Communicator::getCommunicationMutex()
{
	return &this->communicationMutex;
}
