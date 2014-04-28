#include "Mutex.hpp"

list<int> * Mutex::existingMutexes;

Mutex::Mutex(int id)
{		
	if(existingMutexes == NULL)
		existingMutexes = new list<int>();
		
	if(std::find(existingMutexes->begin(), existingMutexes->end(), id) == existingMutexes->end())
		throw new invalid_argument("Mutex of that ID already exists");
	
	this->id = id;
	existingMutexes->push_back(id);
}
