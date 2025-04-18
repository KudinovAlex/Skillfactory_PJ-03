#pragma once
#include <string>
#include <ctime>

class Message
{
public: 
	Message(const size_t idFrom, const size_t idTo, const std::string& _message);
	~Message() = default;

	const size_t _idFrom;
	const size_t _idTo;
	const std::string _message;
	const time_t _timestamp = time(NULL);
};

