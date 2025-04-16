#pragma once
#include <string>
#include <ctime>

class Message
{
public: 
	// не смог сообразить как обойтись одним конструктором Message(const size_t, const size_t, string&), буду признателен за подсказку
	Message(const size_t idFrom, const size_t idTo, const std::string& _message);
	Message(const size_t idFrom, const size_t idTo, const char* _message);
	~Message() = default;
	// не вижу смысла делать поля приватными поскольку все константные, делать геттеры и сеттеры на мой взгляд - оверинжиниринг		
	const size_t _idFrom;
	const size_t _idTo;
	const std::string _message;
	const time_t _timestamp = time(NULL);
};

