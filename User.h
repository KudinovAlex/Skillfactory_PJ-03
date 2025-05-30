#pragma once
#include <string>

class User
{
public:
	User(const std::string& login, const std::string& password, const std::string& name, const std::string& family_name);
	~User() = default;

	inline const std::string& getLogin() const { return _login; }
	inline const std::string& getName() const { return _name; }
	inline const std::string& getFamilyName() const { return _familyName; }
	inline const size_t getId() const { return _id; }

	bool isPasswordCorrect(const std::string& password) const;
	void setPassword(std::string& newPassword);

	inline size_t getLastReadMessage() const { return _lastReadMessageId; }
	inline void setLastReadMessage(const size_t msg_id) { _lastReadMessageId = msg_id; }

	friend std::ostream& operator<< (std::ostream& out, const User& user);

private:
	const std::string _login;
	std::string _password;
	const std::string _name;
	const std::string _familyName;
	const size_t _id;
	size_t _lastReadMessageId{0};	
	static inline size_t _idCounter{};
};

