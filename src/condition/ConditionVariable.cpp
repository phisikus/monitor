#include "ConditionVariable.hpp"

list<pair<int,ConditionVariable *>> * ConditionVariable::existingConditionVariables;

ConditionVariable::ConditionVariable(int id)
{
    if(existingConditionVariables == NULL)
        existingConditionVariables = new list<pair<int,ConditionVariable *>>();

    if(this->getConditionVariable(id) != NULL)
        throw new invalid_argument("ConditionVariable of that ID already exists");

    this->id = id;  

    pair<int, ConditionVariable *> *p = new pair<int, ConditionVariable *>;
    p->first = id;
    p->second = this;    

    existingConditionVariables->push_back(*p);

}


ConditionVariable * ConditionVariable::getConditionVariable(int id)
{
    for (std::list<pair<int,ConditionVariable *>>::iterator it = existingConditionVariables->begin(); it != existingConditionVariables->end(); ++it)
    {

        if(it->first == id)
            return it->second;
    }
    return NULL;
}



list<ConditionVariable *> * ConditionVariable::getConditionVariables()
{
    list<ConditionVariable *> *listOfConditionVariables = new list<ConditionVariable *>();
    for (std::list<pair<int,ConditionVariable *>>::iterator it = existingConditionVariables->begin(); it != existingConditionVariables->end(); ++it)
    {
        listOfConditionVariables->push_back(it->second);
    }
    return listOfConditionVariables;
}
