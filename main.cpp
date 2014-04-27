#include <iostream>
#include <stdio.h>
#include "src/Monitor.hpp"

int main(int argc, char *argv[])
{
	Monitor *monitor = new Monitor();			
	while(true);;
	monitor->finalize();	
	delete monitor;
	monitor = NULL;
	return 0;
}
