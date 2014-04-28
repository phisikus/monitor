#ifndef INCLUDE_MUTEX_HPP
#define INCLUDE_MUTEX_HPP
#include <vector>
#include <algorithm>
#include <list>
#include <stdexcept>
using namespace std;

class Mutex
{
	public:
		int id;
		bool requesting = false;
		long requestClock = -1;
		vector<bool> *agreeVector;
		
		Mutex(int id);	
	private:
		static list<int> *existingMutexes;
		
};

#endif
