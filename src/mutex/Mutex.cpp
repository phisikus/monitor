#include "Mutex.hpp"

list<pair<int,Mutex *>> * Mutex::existingMutexes = new list<pair<int,Mutex *>>();
mutex Mutex::mutexListMutex;


Mutex::Mutex(int id)
{

	if(this->getMutex(id) != NULL)
		throw new invalid_argument("Mutex of that ID already exists");

	this->id = id;
	this->agreeVector = NULL;

	pair<int, Mutex *> *p = new pair<int, Mutex *>;
	p->first = id;
	p->second = this;    

	mutexListMutex.lock();
	existingMutexes->push_back(*p);
	mutexListMutex.unlock();

}


Mutex * Mutex::getMutex(int id)
{
	mutexListMutex.lock();
	for (std::list<pair<int,Mutex *>>::iterator it = existingMutexes->begin(); it != existingMutexes->end(); ++it)
	{

		if(it->first == id)
		{

			mutexListMutex.unlock();
			return it->second;
		}
	}
	mutexListMutex.unlock();
	return NULL;
}


void * Mutex::getData()
{
	if((this->previousReturn != NULL) && (this->previousReturn->hasData))
	{
		return this->previousReturn->data;
	}

	return NULL;
}


long Mutex::getDataSize()
{
	if((this->previousReturn != NULL) && (this->previousReturn->hasData))
	{
		return this->previousReturn->dataSize;
	}

	return 0;
}


void Mutex::setDataForReturn(void *data, long size)
{
	if(this->previousReturn != NULL)
	{
		delete this->previousReturn;
	}

	Message *m = new Message();
	m->type = DATA;
	m->referenceId = this->id;
	m->hasData = true;
	m->dataSize = size;
	m->data = (char*) data;
	this->previousReturn = m;

};


list<Mutex *> * Mutex::getMutexes()
{
	mutexListMutex.lock();
	list<Mutex *> *listOfMutexes = new list<Mutex *>();
	for (std::list<pair<int,Mutex *>>::iterator it = existingMutexes->begin(); it != existingMutexes->end(); ++it)
	{
		listOfMutexes->push_back(it->second);
	}
	mutexListMutex.unlock();
	return listOfMutexes;
}

bool Mutex::agreeVectorTrue()
{
	if(this->agreeVector == NULL)
		return false;

	for(unsigned int i = 0; i < this->agreeVector->size(); i++)
	{
		if((*this->agreeVector)[i] != true)
			return false;
	}

	return true;
}

