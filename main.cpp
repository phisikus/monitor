#include <iostream>
#include "src/Monitor.hpp"

int main(int argc, char *argv[])
{
	Monitor *monitor = new Monitor();
	monitor->finalize();
	delete monitor;
	monitor = NULL;
	return 0;
}
