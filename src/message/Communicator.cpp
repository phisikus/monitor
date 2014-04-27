#include "Communicator.hpp"

void Communicator::init(int argc, char **argv) 
{
	this->processName = new char[MPI_MAX_PROCESSOR_NAME];
	MPI::Init_thread(argc, argv, MPI_THREAD_MULTIPLE);
	MPI_Comm_rank(MPI_COMM_WORLD, &this->processId);
	MPI_Get_processor_name(this->processName, new int);	
	this->initialized = true;
};

void Communicator::close()
{
	if(this->initialized)
	{
		this->initialized = false;
		MPI::Finalize();			
	}
}

void Communicator::log(string text)
{
	#ifdef DEBUG
	string message = "";
	if(this->processName != NULL) 
	{
		message += "[";
		message += this->processName;
		message += " " + to_string(this->processId) + "] ";
	}
	message += text;
	cout << message << endl;		
		
	#endif
}

void Communicator::sendMessage(Message *msg)
{
	if(msg != NULL)
	{			
		this->log("Sending message " + toString(msg->type) + "  to " + to_string(msg->recipientId));		
		MPI_Isend(msg->getArray(), msg->getArraySize(), MPI_INT, msg->senderId, 0, MPI_COMM_WORLD, new MPI_Request());
		
	}
}


Message* Communicator::recvMessage()
{
/*    Message *msg = new Message;
    int data[6];
    MPI_Status status;
    MPI_Recv(data, msg->getArraySize(), MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    msg->fromArray(data);
    me->clock = max(me->clock, msg->clock + 1);
    return msg;*/	
}

