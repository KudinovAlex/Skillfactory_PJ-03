#include "User.h"
#include <iostream>

User::User(std::string& login, std::string& password, std::string& name, std::string& familyName) :
	_login(login), _password(password), _name(name), _familyName(familyName), _id(_idCounter++) 
{
}

User::User(const char* login, const char* password, const char* name, const char* familyName) :
	_login(login), _password(password), _name(name), _familyName(familyName), _id(_idCounter++) 
{
}

bool User::isPasswordCorrect(const std::string& password) const
{
	return password == _password;
}

void User::setPassword(std::string& newPassword)
{
	_password = newPassword;
}

std::ostream& operator<< (std::ostream& out, const User& user) {
	out << "id:           " << user._id << '\n'
		<< "Пользователь: " << user._name << " " << user._familyName << '\n'
		<< "Логин:        " << user._login << '\n'
		<< "Пароль:       " << user._password << std::endl;
	return out;
}




