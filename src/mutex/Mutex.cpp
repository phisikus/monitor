#include "Mutex.hpp"

list<pair<int,Mutex *>> * Mutex::existingMutexes;

Mutex::Mutex(int id)
{		
	if(existingMutexes == NULL)
		existingMutexes = new list<pair<int,Mutex *>>();
	
	if(this->getMutex(id) != NULL)
		throw new invalid_argument("Mutex of that ID already exists");
	
	this->id = id;	
	
	pair<int, Mutex *> *p = new pair<int, Mutex *>;
	p->first = id;
	p->second = this;
	existingMutexes->push_back(*p);
	
}


Mutex * Mutex::getMutex(int id)
{
	for (std::list<pair<int,Mutex *>>::iterator it = existingMutexes->begin(); it != existingMutexes->end(); ++it)
	{
				
		if(it->first == id)
			return it->second;
	}
	return NULL;
}
