#include <iostream>
#include "Monitor.hpp"
#include <signal.h>
using namespace std;


Monitor::Monitor()
{
	this->communicator = new Communicator();	
	this->log(TRACE, "Monitor: object created. ");
	this->init(0, NULL);

}

Monitor::Monitor(int argc, char **argv)
{
	this->communicator = new Communicator();	
	this->log(TRACE, "Monitor: object created. ");
	this->init(argc, argv);
}

Monitor::~Monitor()
{	
	this->finalize();
	this->log(TRACE, "Monitor: object destroyed.");
}

void Monitor::log(LogLevel level, string text)
{
	this->communicator->log(level, text);
};


void Monitor::init(int argc, char **argv)
{
	this->log(TRACE, "Monitor: initializing...");
	this->communicationThread = NULL;
	this->communicator->init(argc, argv);
	
	// send START
	Message *msg = new Message();
	msg->type = START;
	msg->data = this->communicator->processName;
	msg->dataSize = strlen(this->communicator->processName);
	communicator->sendBroadcast(msg);
	delete msg;
	
	this->communicationThread = new thread(&Monitor::communicationLoop,this);	
		
	this->log(TRACE, "Monitor: Communication loop started.");
	this->log(INFO, "Monitor: initialized. ");

}

void Monitor::finalize()
{		
	Message *msg = new Message();
	msg->type = QUIT;
	communicator->sendBroadcast(msg);
	
	communicator->close();
	this->log(TRACE, "Monitor: MPI finalized. ");
}

void Monitor::communicationLoop()
{
	Message *msg;
	while(true)
	{			
		msg = communicator->recvMessage();		
		
		if(msg == NULL)
			break;
				
		switch(msg->type)
		{
			case START:
				// Nothing to do here...
				delete msg;
				break;
				
			case REQUEST:
			
				// 1. Find Mutex of given id. ALL MUTEXES Should exist before that point!
				// 2. If Mutex has requesting = false - reply with AGREE and break;
				// 3. If Mutex has requesting = true
				//		if requesting process has better packet -> send AGREE
				//			else -> add id of this process to heldUpRequests in Mutex,					
				//		
				break;
				
			case QUIT:				
				communicator->getCommunicationMutex()->lock();
				communicator->activePeers[msg->senderId] = false;				
				communicator->getCommunicationMutex()->unlock();
				for (std::list<Mutex*>::iterator it = Mutex::getMutexes()->begin(); it != Mutex::getMutexes()->end(); ++it)
				{					
					(*it)->operationMutex.lock();
					if((*it)->requesting)
						(*(*it)->agreeVector)[msg->senderId] = true;
					(*it)->operationMutex.unlock();														
				}
				delete msg;
				
				// check if there are any winners...								
				break;
				
			case RETURN:				
				// 1. Save in mutex this packet. It doesn't matter if we're requesting. It might be possible that none will and after a while we will just get AGREE from all processes.
				// 2. Do everything like in AGREE packet.
				break;
			
			case REQUEST_DATA:
				{
					Mutex *m = Mutex::getMutex(msg->referenceId);
					if(m != NULL)
					{
						m->operationMutex.lock();
						if((m->previousReturn != NULL) && (m->previousReturn->type == DATA))
						{
							// Copy data packet from Mutex, received earlier and send it further.
							void *packet = new char[m->previousReturn->getArraySize()];						
							Message *dataMessage = new Message((MessageDTO *) packet);
							dataMessage->recipientId = msg->senderId;
							communicator->sendMessage(dataMessage);
							delete dataMessage;
						}
						m->operationMutex.unlock();
					}					
					delete msg;
					break;
				}	
			case DATA:
				{
					Mutex *m = Mutex::getMutex(msg->referenceId);
					if(m != NULL)
					{						
						m->operationMutex.lock();
						m->previousReturn = msg; // Save message with data to Mutex property.
						msg = NULL;
						m->operationMutex.unlock();
					}
					
				}			
				// 2. Call conditional variable TODO
				break;
				
			case AGREE:
					{
						Mutex *m = Mutex::getMutex(msg->referenceId);
						m->operationMutex.lock();							
						if((m != NULL) && (m->requesting))
						{						
							(*(m->agreeVector))[msg->senderId] = true;
							// If there is no RETURN package in previousReturn - it is beginning of program and we are first to go - we should create fake return with no data.
							// If there is RETURN package in previousReturn and there is data - send REQUEST_DATA.
							// Check if winner....
							
							
						}
						m->operationMutex.unlock();
						delete msg;
						
					}		
				
				break;
				
			default:
				break;
			
		}
		
	}
	
}

void Monitor::lock(Mutex *mutex) 
{
	// Tutaj osobny lock na zmienną warunkową, bo nie chcemy zablokować możliwości przychodzenia AGREE
	/*
	 * 1. Lock local mutex related to Mutex's conditional variable
	 * 2. Send REQUEST + set requesting in mutex + clock
	 * 3. Lock mutex for communication to check activePeers
	 * 4. Wait on conditional variable
	 * 5. Set inCs, Unlock local conditional mutex
	 * 6. exit
	 */
}

void Monitor::unlock(Mutex *mutex)
{
	/*
	 * 0. Lock local mutex
	 * 1. Build RETURN msg with hasData = true (if there was any in Mutex)
	 * 2. Send it
	 * 3. Clear Mutex structures related to this C.S. 
	 * 4. Send AGREE to hosts waiting (list in mutex)
	 * 5. unlock local mutex, exit
	 * */
}
