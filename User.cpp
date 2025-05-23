#include "User.h"
#include <iostream>

User::User(const std::string& login, const std::string& password, const std::string& name, const std::string& familyName) :
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
		<< "������������: " << user._name << " " << user._familyName << '\n'
		<< "�����:        " << user._login << '\n'
		<< "������:       " << user._password << std::endl;
	return out;
}




