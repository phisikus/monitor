#ifndef INCLUDE_MUTEX_HPP
#define INCLUDE_MUTEX_HPP
#include "../message/Message.hpp"
#include <vector>
#include <algorithm>
#include <list>
#include <mutex>
#include <stdexcept>
#include <condition_variable>
using namespace std;

class Mutex
{
	public:
		int id; // Mutex ID
		bool requesting = false; // It tells if this mutex is locked and waiting for C.S.
		bool locked = false;
		long requestClock = 0; // timestamp to reject AGREE packets that came late for previous request
		vector<bool> *agreeVector; // it will be filled with negation of activePeers vector. If other agrees we can enter C. S.
		list<int> heldUpRequests; // processes that sent REQUEST but should wait for their turn - we will send AGREE after unlock()
		mutex operationMutex; // local mutex that will block communicationLoop for this distributed Mutex during lock/unlock.
		mutex localMutex; // local mutex for thread safe behaviour
		Message *previousReturn = NULL; // This field will contain most recent RETURN message (based on clock) or DATA. When we enter CS it will be possible to get current data and save it to this variable.

		condition_variable criticalSectionCondition; // On this variable lock() will wait till all agrees are collected by communicationLoop and DATA packet is send if needed
		unique_lock<mutex> *criticalSectionConditionLock;
		mutex criticalSectionConditionMutex;

		Mutex(int id);
		static Mutex * getMutex(int id);
		static list<Mutex *> * getMutexes();
		bool agreeVectorTrue();
		void * getData();
		long getDataSize();
		void setDataForReturn(void *data, long size); // Create new previousReturn Message of type DATA with given data

	private:
		static mutex mutexListMutex;
		static list<pair<int,Mutex*>> *existingMutexes;

};

#endif
