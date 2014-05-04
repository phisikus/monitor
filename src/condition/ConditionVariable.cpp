#include "ConditionVariable.hpp"

list<pair<int,ConditionVariable *>> * ConditionVariable::existingConditionVariables = new list<pair<int,ConditionVariable *>>();
mutex ConditionVariable::conditionListMutex;


ConditionVariable::ConditionVariable(int id)
{
	
	if(this->getConditionVariable(id) != NULL)
		throw new invalid_argument("ConditionVariable of that ID already exists");

	this->id = id;  

	pair<int, ConditionVariable *> *p = new pair<int, ConditionVariable *>;
	p->first = id;
	p->second = this;    

	conditionListMutex.lock();
	existingConditionVariables->push_back(*p);
	conditionListMutex.unlock();

}


ConditionVariable * ConditionVariable::getConditionVariable(int id)
{
	conditionListMutex.lock();
	for (std::list<pair<int,ConditionVariable *>>::iterator it = existingConditionVariables->begin(); it != existingConditionVariables->end(); ++it)
	{

		if(it->first == id)
		{
			conditionListMutex.unlock();
			return it->second;
		}
	}
	conditionListMutex.unlock();
	return NULL;
}



list<ConditionVariable *> * ConditionVariable::getConditionVariables()
{
	conditionListMutex.lock();
	list<ConditionVariable *> *listOfConditionVariables = new list<ConditionVariable *>();
	for (std::list<pair<int,ConditionVariable *>>::iterator it = existingConditionVariables->begin(); it != existingConditionVariables->end(); ++it)
	{
		listOfConditionVariables->push_back(it->second);
	}
	conditionListMutex.lock();
	return listOfConditionVariables;
}
