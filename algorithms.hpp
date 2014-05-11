#include <iostream>
#include <unistd.h>

struct Buffer {
	int availableUnits = 0;
	char units[11] = "----------";
	unsigned int consumersWaiting = 0;
} data;


void producerConsumer()
{
	Mutex *m = new Mutex(1);
	ConditionVariable *cv_c = new ConditionVariable(1);

	m->setDataForReturn((void *) &data, sizeof(data));

	Monitor *monitor = new Monitor();

	/*
	 * producer
	 */
	monitor->lock(m);
	Buffer *b = (Buffer *) m->getData();		

	// Produce one unit of resource
	b->units[b->availableUnits] = b->availableUnits + 48;
	b->availableUnits++;
	monitor->log(INFO,"Producer: count = " + to_string(b->availableUnits) + " buffer = " + b->units);
	monitor->unlock(m);

	/*
	 * consumer
	 */
	
	// Wait for all consumers to appear. 
	monitor->lock(m);
	monitor->log(TRACE,"Here comes the tricky part.");
	((Buffer *) m->getData())->consumersWaiting++;
	while(((Buffer *) m->getData())->consumersWaiting < monitor->communicator->processCount)
	{
		monitor->wait(cv_c, m);
		monitor->log(INFO,"Discovered value: " + to_string(((Buffer *) m->getData())->consumersWaiting));
	}
	monitor->log(TRACE,"Here ends the trick.");



	b->availableUnits--;
	b->units[b->availableUnits] = '-';
	monitor->log(INFO,"Consumer:  count = " + to_string(b->availableUnits) + " buffer = " + b->units);


	monitor->signalAll(cv_c);
	monitor->unlock(m);
	monitor->finalize();
	delete monitor;
	monitor = NULL;

}


void tryPowerOfTwo()
{
	Mutex *m = new Mutex(1);
	Monitor *monitor = new Monitor();

	for(unsigned int i = 0; i < 2 ; i++)
	{
		monitor->lock(m);
		if(m->getDataSize() == 0)
		{
			int x = 2;
			m->setDataForReturn(&x,sizeof(x));
		}
		int *y = (int *) m->getData();

		monitor->log(INFO,"CS: " + to_string(*y));
		(*y) *= 2;
		monitor->unlock(m);
	}

	monitor->finalize();
	delete monitor;
	monitor = NULL;


}


void tryCondition()
{
	Mutex *m = new Mutex(1);
	ConditionVariable *cv = new ConditionVariable(1);
	Monitor *monitor = new Monitor();


	if(monitor->communicator->processId  > 0)
	{
		monitor->lock(m);		
		monitor->wait(cv, m);	
		monitor->log(INFO,"I left wait, now i will signal in 2 seconds.");
		usleep(2000000);
		monitor->signalOne(cv);
		monitor->unlock(m);
	}    
	else
	{
		usleep(5000000);
		monitor->lock(m);		
		monitor->signalOne(cv);
		monitor->unlock(m);		
	}

	monitor->finalize();
	delete monitor;
	monitor = NULL;


}

