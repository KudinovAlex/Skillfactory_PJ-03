#include "Message.h"

#include <limits>

Message::Message(const size_t idFrom, const size_t idTo, const std::string& _message) :
	_idFrom(idFrom), _idTo(idTo), _message(_message)
{
}

Message::Message(const size_t idFrom, const size_t idTo, const char* _message) :
	_idFrom(idFrom), _idTo(idTo), _message(_message)
{
}

