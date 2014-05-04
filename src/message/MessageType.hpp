/**
 * START - welcome packet sent to all processes.
 * REQUEST - is sent to all processes to inform that this one wants to enter critical section
 * AGREE - this message should be received from all active processes before process can enter critical section
 * QUIT - this message is equal to AGREE and also states that process will no longer communicate
 * RETURN - this message is sent by process leaving critical section to all active processes. Special flag determines if there is any data.
 * REQUEST_DATA - this message is sent by next process that is entering critical section to get data from previous process.
 * DATA - used to transmit data described above
 */
#ifndef INCLUDE_MESSAGE_TYPE_HPP
#define INCLUDE_MESSAGE_TYPE_HPP
#include <iostream>

enum MessageType { REQUEST, AGREE, RETURN, REQUEST_DATA, DATA, WAIT, WAIT_RETURN, SIGNAL};

inline std::string toString(MessageType v)
{
	switch (v)
	{
		case REQUEST:
			return "REQUEST";
		case AGREE:
			return "AGREE";
		case RETURN:
			return "RETURN";
		case REQUEST_DATA:
			return "REQUEST_DATA";
		case DATA:
			return "DATA";
		case WAIT:
			return "WAIT";		
		case WAIT_RETURN:
			return "WAIT_RETURN";
		case SIGNAL:
			return "SIGNAL";

		default:
			return "[unknown]";
	}
}

#endif
