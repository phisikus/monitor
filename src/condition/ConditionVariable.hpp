#ifndef CONDITIONVARIABLE_HPP_INCLUDE
#define CONDITIONVARIABLE_HPP_INCLUDE
#include <vector>
#include <mutex>
#include <list>
#include <condition_variable>
using namespace std;

class ConditionVariable
{
	public:
		int id;
		mutex operationMutex;
		condition_variable cv;
		bool waiting = false;
		list<int> waitingProcesses;

		ConditionVariable(int id);
		static ConditionVariable * getConditionVariable(int id);
		static list<ConditionVariable *> * getConditionVariables();

	private:
		static mutex conditionListMutex;
		static list<pair<int,ConditionVariable*>> *existingConditionVariables;


};
#endif
