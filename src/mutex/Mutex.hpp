#ifndef INCLUDE_MUTEX_HPP
#define INCLUDE_MUTEX_HPP
#include <vector>
#include <algorithm>
#include <list>
#include <mutex>
#include <stdexcept>
using namespace std;

class Mutex
{
	public:
		int id; // Mutex ID
		bool requesting = false; // It tells if this mutex is locked and waiting for C.S.
		long requestClock = -1; // timestamp to reject AGREE packets that came late for previous request
		vector<bool> *agreeVector; // it will be filled with negation of activePeers vector. If other agrees we can enter C. S.
		list<int> heldUpRequests; // processes that sent REQUEST but should wait for their turn - we will send AGREE after unlock()
		mutex operationMutex; // local mutex that will block communicationLoop for this distributed Mutex during lock/unlock.		
		Message *previousReturn; // This field will contain most recent RETURN message (based on clock). When we enter CS it will be possible to get current data.
		condition_variable criticalSectionCondition; // On this variable lock() will wait till all agrees are collected by communicationLoop and DATA packet is send if needed
		
		
		Mutex(int id);	
		static Mutex * getMutex(int id);
		void * getData();
		int getDataSize();
		
	private:
		static list<pair<int,Mutex*>> *existingMutexes;
		
};

#endif
