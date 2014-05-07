#include <iostream>
#include "Monitor.hpp"
#include <signal.h>
#include <unistd.h>
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

	this->communicationThread = new thread(&Monitor::communicationLoop,this);

	this->log(TRACE, "Monitor: Communication loop started.");
	this->log(INFO, "Monitor: initialized. ");

}

void Monitor::finalize()
{
	communicator->barrier();
	Message *q = new Message();
	q->type = QUIT;
	q->recipientId = communicator->processId;
	communicator->sendMessage(q);
	delete q;
	this->log(TRACE, "Waiting for communication thread to join parent.");
	this->communicationThread->join();	
	this->log(TRACE, "Communication thread joined parent.");
	communicator->barrier();
	communicator->close();
	this->log(TRACE, "Monitor: MPI finalized. ");
}

void Monitor::communicationLoop()
{

	Message *msg;
	while(true)
	{
		communicator->waitForMessage();
		msg = communicator->recvMessage();

		if(msg == NULL)
			break;

		switch(msg->type)
		{

			case REQUEST:
				{
					Mutex *m = Mutex::getMutex(msg->referenceId);
					if(m != NULL)
					{
						m->operationMutex.lock();
						if(m->requesting)
						{
							// If REQUEST has earlier time than ours -> AGREE
							// If time is the same but we have higher ID -> AGREE
							if(m->requestClock < msg->clock)
							{
								// Add to queue
								m->heldUpRequests.push_back(msg->senderId);
							}
							else
							{
								if((m->requestClock == msg->clock) && (communicator->processId < msg->senderId))
								{
									m->heldUpRequests.push_back(msg->senderId);
								}
								else
								{
									Message *agreeReply = new Message();
									agreeReply->type = AGREE;
									agreeReply->referenceId = msg->referenceId;
									agreeReply->recipientId = msg->senderId;
									communicator->sendMessage(agreeReply);
									delete agreeReply;
								}
							}

						}
						else
						{
							if(!m->locked)
							{
								Message *agreeReply = new Message();
								agreeReply->type = AGREE;
								agreeReply->referenceId = msg->referenceId;
								agreeReply->recipientId = msg->senderId;
								communicator->sendMessage(agreeReply);
								delete agreeReply;
							}
							else
							{
								m->heldUpRequests.push_back(msg->senderId);
							}

						}
						m->operationMutex.unlock();
					}
					delete msg;
				}
				break;

			case RETURN:
				{
					Mutex *m = Mutex::getMutex(msg->referenceId);
					if(m != NULL)
					{
						m->operationMutex.lock();
						(*(m->agreeVector))[msg->senderId] = true;

						if(m->previousReturn != NULL)
						{
							delete m->previousReturn;
						}

						m->previousReturn = msg;
						m->operationMutex.unlock();
						enterCriticalSection(m);
					}
				}
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
							Message *dataMessage = new Message(m->previousReturn);
							dataMessage->recipientId = msg->senderId;
							communicator->sendMessage(dataMessage);
							delete dataMessage;
						}
						m->operationMutex.unlock();
					}
					delete msg;
				}
				break;
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
					// check if conditions to enter C.S. were met and signal conditional variable.
					enterCriticalSection(m);

				}
				break;

			case AGREE:
				{
					Mutex *m = Mutex::getMutex(msg->referenceId);
					m->operationMutex.lock();
					if((m != NULL) && (m->requesting))
					{
						// Note that sender agreed.
						(*(m->agreeVector))[msg->senderId] = true;
						m->operationMutex.unlock();

						// check if conditions to enter C.S. were met and signal conditional variable.
						enterCriticalSection(m);

					}
					else
					{
						m->operationMutex.unlock();
					}

					delete msg;

				}

				break;

			case WAIT:
				{
					ConditionVariable *cv = ConditionVariable::getConditionVariable(msg->referenceId);
					unique_lock<mutex> l(cv->operationMutex);
					cv->waitingProcesses.push_back(msg->senderId);					
				}
				break;

			case WAIT_RETURN:
				{
					ConditionVariable *cv = ConditionVariable::getConditionVariable(msg->referenceId);
					unique_lock<mutex> l(cv->operationMutex);
					cv->waitingProcesses.remove(msg->senderId);			
				}
				break;

			case SIGNAL:
				{
					ConditionVariable *cv = ConditionVariable::getConditionVariable(msg->referenceId);
					unique_lock<mutex> l(cv->operationMutex);
					cv->waiting = false;
					cv->cv.notify_one();					
				}
				break;					

			case QUIT:
				{
					return;
					break;
				}


			default:
				break;

		}

	}

}

// Called when packets received: RETURN, DATA, AGREE
void Monitor::enterCriticalSection(Mutex *m)
{
	if(m == NULL) return;

	m->operationMutex.lock();
	if((m->requesting) && (m->agreeVectorTrue()))
	{
		// conditions to enter C.S. were met. We should check if it is necessary to request for data...
		if(m->previousReturn == NULL)
		{
			// we are the first ones entering this section...
			m->requesting = false;
			fill(m->agreeVector->begin(), m->agreeVector->end(), false);
			{
				unique_lock<std::mutex> cLock(m->criticalSectionConditionMutex);            
				m->criticalSectionCondition.notify_one();
			}                                   
			m->operationMutex.unlock();
			return;
		}
		else
		{

			if(m->previousReturn->type == RETURN)
			{


				if(m->previousReturn->hasData)
				{
					// Ask for data from previous process that entered.
					Message *rd = new Message();
					rd->type = REQUEST_DATA;
					rd->recipientId = m->previousReturn->senderId;
					rd->referenceId = m->previousReturn->referenceId;
					communicator->sendMessage(rd);
					delete rd;

				}
				else
				{
					m->requesting = false;
					fill(m->agreeVector->begin(), m->agreeVector->end(), false);                    
					{
						// !!! this lock will only exist in this bracket scope and will be unlocked after that
						unique_lock<std::mutex> cLock(m->criticalSectionConditionMutex); 
						m->criticalSectionCondition.notify_one();
					}                                   
					m->operationMutex.unlock();
					return;
				}
			}

			if(m->previousReturn->type == DATA)
			{

				m->requesting = false;
				fill(m->agreeVector->begin(), m->agreeVector->end(), false);                
				{
					unique_lock<std::mutex> cLock(m->criticalSectionConditionMutex);            
					m->criticalSectionCondition.notify_one();
				}
				m->operationMutex.unlock();
				return;
			}
		}



	}
	m->operationMutex.unlock();

}

void Monitor::lock(Mutex *mutex)
{
	// local mutex for thread safety
	mutex->localMutex.lock();

	// Set mutex to "requesting" state.
	mutex->operationMutex.lock();
	mutex->requesting = true;

	// Determine which other processes should agree before we can get in (all except those who sent QUIT)
	if(mutex->agreeVector != NULL)
	{
		delete mutex->agreeVector;
		mutex->agreeVector = NULL;
	}

	communicator->getCommunicationMutex()->lock();
	mutex->agreeVector = new vector<bool>(communicator->processCount, false);
	int	processCount = communicator->processCount;
	for(int i = 0; i < processCount; i++)
	{
		(* mutex->agreeVector)[i] = (i == communicator->processId);
	}
	communicator->getCommunicationMutex()->unlock();

	// Send Request.
	Message *rm = new Message();
	rm->type = REQUEST;
	rm->referenceId = mutex->id;
	communicator->sendBroadcast(rm);

	// save the time of request
	mutex->requestClock = rm->clock;
	delete rm;

	// Set status.
	mutex->locked = true;


	// Wait for conditions to be met.
	{
		unique_lock<std::mutex> cLock(mutex->criticalSectionConditionMutex);

		mutex->operationMutex.unlock();

		while(mutex->requesting)
			mutex->criticalSectionCondition.wait(cLock);				
	}

	this->log(INFO,"(" + to_string(mutex->id) + ") Locked.");

}

void Monitor::unlock(Mutex *mutex)
{
	mutex->operationMutex.lock();

	if(!mutex->locked)
	{
		mutex->operationMutex.unlock();
		return;
	}

	Message *retMessage = new Message();
	retMessage->type = RETURN;
	retMessage->referenceId = mutex->id;
	retMessage->hasData = false;

	if(mutex->previousReturn != NULL)
	{
		retMessage->hasData = mutex->previousReturn->hasData;
	}

	// send Messages to all held up processes
	for (std::list<int>::const_iterator it = mutex->heldUpRequests.begin(), end = mutex->heldUpRequests.end(); it != end; ++it)
	{
		retMessage->recipientId = (*it);
		communicator->sendMessage(retMessage);
	}
	mutex->heldUpRequests.clear();

	delete retMessage;
	mutex->requesting = false;
	mutex->locked = false;


	// Create new agree vector.
	communicator->getCommunicationMutex()->lock();
	mutex->agreeVector = new vector<bool>(communicator->processCount, false);
	int processCount = communicator->processCount;
	for(int i = 0; i < processCount; i++)
	{
		(* mutex->agreeVector)[i] = (i == communicator->processId);
	}
	communicator->getCommunicationMutex()->unlock();

	mutex->operationMutex.unlock();

	this->log(INFO,"(" + to_string(mutex->id) + ") Unlocked.");

	mutex->localMutex.unlock();


}


void Monitor::wait(ConditionVariable *cv, Mutex *m)
{

	unique_lock<mutex> l(cv->operationMutex);
	cv->waiting = true;

	Message *ret = new Message();
	ret->referenceId = cv->id;
	ret->type =	WAIT;
	communicator->sendBroadcast(ret);	
	delete ret;


	this->log(INFO,"(" + to_string(cv->id) + ") Waiting... ");
	while(cv->waiting)
	{
		this->unlock(m);
		cv->cv.wait(l);
		this->lock(m);
	}

	this->log(INFO,"(" + to_string(cv->id) + ") Received signal - no more waiting.");

	ret = new Message();
	ret->referenceId = cv->id;
	ret->type =	WAIT_RETURN;
	communicator->sendBroadcast(ret);	
	delete ret;

}



void Monitor::signalAll(ConditionVariable *cv)
{
	unique_lock<mutex> l(cv->operationMutex);
	Message *sgn = new Message();
	sgn->referenceId = cv->id;
	sgn->type =	SIGNAL;
	for(int i : cv->waitingProcesses)
	{			
		sgn->recipientId = i;
		communicator->sendMessage(sgn);	
	}
	delete sgn;
}


void Monitor::signalOne(ConditionVariable *cv)
{
	unique_lock<mutex> l(cv->operationMutex);
	Message *sgn = new Message();
	sgn->referenceId = cv->id;
	sgn->type =	SIGNAL;		
	if(cv->waitingProcesses.size() > 0)
	{
		sgn->recipientId = cv->waitingProcesses.front();
		communicator->sendMessage(sgn);	
	}
	delete sgn;
}
