#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <limits>
#include <iomanip>
#include <exception>

#include "User.h"
#include "Message.h"

size_t find_user_id_by_login(const std::vector<User>& users, const std::string& user_login) {
	for (size_t i = 0; i < users.size(); ++i) {
		if (users[i].getLogin() == user_login) {
			return i;
		}
	}
	return SIZE_MAX; // пользователь не найдет
}

void enter_field(std::string& str, const char* message, const size_t minLength = 1) {
	std::cout << message;
	while (true) {
		std::getline(std::cin, str);
		if (str.size() >= minLength) return; // успешный ввод
		std::cout << "Поле не может быть ";
		if (minLength == 1) std::cout << "пустым,";
		else if (minLength > 1) std::cout << "менее " << minLength << " символов";
		std::cout << ", попробуйте ещё раз : ";
	}
}

template<typename T>
T enter_value(const T minVal, const T maxVal, const char* msg) { // minVal и maxVal задают ДИАПАЗОН допустимых значений включительно
	std::cout << msg;
	T val; // либо инициализируется через cin, либо не будет использована далее.
	while (true) {
		std::cin >> val;
		std::cin.clear();
		std::cin.ignore(32767, '\n');
		if (!std::cin.fail()) {
			if (val >= minVal && val <= maxVal) {
				return val;
			}
		}
		std::cout << "Ошибка ввода, попробуйте ещё раз: ";
	}
}

void print_one_message(const Message& msg, const std::vector<User>& users)
{
	std::cout << "От кого: " << users[msg._idFrom].getName() << ' ' << users[msg._idFrom].getFamilyName() << '\n' << "Кому   : ";
	if (msg._idTo == SIZE_MAX) std::cout << "Все\n";
	else std::cout << users[msg._idTo].getName() << ' ' << users[msg._idTo].getFamilyName() << '\n';
	std::cout.fill('_');
	char str[26];
	ctime_s(str, sizeof str, &msg._timestamp);
	std::cout << "Дата   : " << str << std::setw(80) << '\n' << msg._message << '\n' << std::endl;
}

void print_all_messages(const std::vector<Message>& messages, const size_t start_msg_id, const std::vector<User>& users, const size_t user_id) {

	if (start_msg_id == messages.size() - 1) {
		std::cout << "Нет сообщений для отображения\n" << std::endl;
	}
	else {
		for (size_t msg_id = start_msg_id; msg_id < messages.size(); ++msg_id) {
			if (messages[msg_id]._idTo == SIZE_MAX || messages[msg_id]._idTo == user_id) {
				print_one_message(messages[msg_id], users);
			}
		}
	}
}

void post_message(std::vector<Message>& messages, std::vector<User>& users, const size_t user_id_from) {

	size_t user_id_to = SIZE_MAX; // по умолчанию отправляем всем.

	while (true) {
		std::cout << "Кому Вы хотите отправить сообщение: " << std::endl;
		for (size_t i = 0; i < users.size(); i++) {
			std::cout << users[i].getLogin() << " - " << users[i].getName() << " " << users[i].getFamilyName() << std::endl;
		}

		std::string input;
		enter_field(input, "Введите ник получателя(ALL - для отправки всем, EXIT - для отмены): ");

		if (input == "EXIT" || input == "exit") { return; }
		else if (input == "ALL" || input == "all") { break; } // user_id_to у нас инициализирован при создании
		else {
			user_id_to = find_user_id_by_login(users, input);
			if (user_id_to < SIZE_MAX) { // введен корректный логин, согласен что криво использовать SIZE_MAX как флаг для ALL и пользователь не найден, но вроде прижилось.
				user_id_to = users[user_id_to].getId();
				break;
			}
			std::cout << "Пользователь на найден!" << std::endl;
		}
	}

	std::string message;
	enter_field(message, "Введите сообщение: ");

	try {
		if (messages.size() == SIZE_MAX) throw std::overflow_error("Сообщение не может быть добавлено");
		messages.emplace_back(Message(user_id_from, user_id_to, message)); 
		std::cout << "Сообщение успешно отправлено!" << std::endl;
	}
	catch (std::exception& exception) {
		throw;
	}
}


int main()
{
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	const size_t min_password_length = 3; // А чтобы жизнь мёдом не казалась пользователям

	std::vector<User> users = {
		User("Administrator", "Administrator1", "Administrator", ""),	// чтобы база пустой не была
		User("donnyT", "donnyT1", "Donald", "Trump"),					// чтобы база пустой не была
		User("joeB", "joeB1", "John", "Biden"),							// чтобы база пустой не была
		User("billG", "billG1", "Bill", "Gates")						// чтобы база пустой не была
	};

	std::vector<Message> messages = {
		Message(users[0].getId(), SIZE_MAX, "Добро пожаловать в чат, ведите себя хорошо и я вас не застрелю!"),	// чтобы база пустой не была
		Message(users[0].getId(), users[1].getId(), "Donald Trump, ты уже достал со своими играми в пошлины!"),	// чтобы база пустой не была
		Message(users[0].getId(), users[2].getId(), "John Biden, мы будем по тебе скучать..."),					// чтобы база пустой не была		
		Message(users[1].getId(), users[2].getId(), "Fuck you John, I'm the president again, Ha-ha-ha!"),		// чтобы база пустой не была
		Message(users[3].getId(), SIZE_MAX, "Fuck all of you, old farths!")										// чтобы база пустой не была
	};

	size_t active_user_id = SIZE_MAX;

	while (true) {
		// ======================= Логин / регистрация нового пользователя =======================
		while (active_user_id == SIZE_MAX) {

			std::string user_name;
			enter_field(user_name, "Введите логин пользователя, или EXIT для завершения работы: ");
			if (user_name == "EXIT" || user_name == "exit") break; // выход в две ступени получился, поскольку циклом while многовато... Это первая ступень...

			active_user_id = find_user_id_by_login(users, user_name); // получаем либо id пользователя, либо SIZE_MAX если логин не найден

			if (active_user_id < SIZE_MAX) { // пользователь найден
				while (true) { // проверяем пароль
					enter_field(user_name, "Введите пароль: ");
					if (users[active_user_id].isPasswordCorrect(user_name)) { break; };
					std::cout << "Пароль неверный, попробуйте еще раз" << std::endl;
				}
			}
			else { // логин не найден

				{
					std::string tmp = user_name;
					for (size_t i = 0; i < tmp.size(); ++i) tmp[i] = toupper(tmp[i]);
					if (tmp == "EXIT" || tmp == "ADMINISTRATOR" || tmp == "ALL" || tmp == "ADMIN") continue; // зарезервировано для служебных нужд
				}

				std::cout << "Логин " << user_name << " не найден,";
				if (enter_value(1, 2, " хотите зарегистрироваться? (1 - да / 2 - нет) ") == 1) { // регистрация нового пользователя

					std::string user_password;
					enter_field(user_password, "Введите пароль: ", min_password_length);

					std::string user_name;
					enter_field(user_name, "Введите имя пользователя: ");

					std::string user_family_name;
					enter_field(user_family_name, "Введите фамилию пользователя: ");

					try {
						if (users.size() == SIZE_MAX - 1) throw std::overflow_error("Пользователь не может быт добавлен");
						users.emplace_back(User(user_name, user_password, user_name, user_family_name)); // тут по идее исключение может быть
						active_user_id = users.back().getId();								
					}
					catch (std::exception& exception) {
						std::cout << exception.what();  
					}
				}
			}
		}

		if (active_user_id == SIZE_MAX) break; // выход в две ступени получился, поскольку циклом while многовато... Это вторая ступень...

		// ======================= Успешный вход в систему, меню чата =======================
		std::cout << "Здравствуйте, " << users[active_user_id].getName() << " " << users[active_user_id].getFamilyName() << "!" << std::endl;

		while (true) {
			std::cout << "Что вы хотите сделать:\n"
				<< "1 - прочитать все сообщения\n"
				<< "2 - прочитать новые сообщения\n"
				<< "3 - написать сообщение\n"
				<< "4 - изменить пароль\n"
				<< "0 - выйти из чата" << std::endl;

			switch (enter_value(0, 4, "Введите команду: ")) {

			case 0:
				std::cout << "Выйти из чата" << std::endl;
				active_user_id = SIZE_MAX;
				break;

			case 1: // Прочитать все сообщения			
				print_all_messages(messages, 0, users, active_user_id);
				users[active_user_id].setLastReadMessage(messages.size() - 1);
	
			break;

			case 2: // Прочитать новые сообщения
				print_all_messages(messages, users[active_user_id].getLastReadMessage(), users, active_user_id);
				users[active_user_id].setLastReadMessage(messages.size() - 1);
				break;

			case 3: // Написать сообщение
				try {
					post_message(messages, users, active_user_id);
				}
				catch (std::exception& exception) {
					std::cout << exception.what();
				}

				break;

			case 4: // Изменить пароль	
			{
				std::string password;
				enter_field(password, "Введите новый пароль: ", min_password_length);
				users[active_user_id].setPassword(password);
				std::cout << "Пароль изменён." << std::endl;
			}
			break;

			default: // учитывая что в вызове enter_value заданы диапазоны допустимых значений, тут мы оказаться не можем, но...
				std::cout << "И на старуху бывает порнуха!" << std::endl;
			}

			if (active_user_id == SIZE_MAX) break;
		}
	}
	std::cout << '\n' << "Список пользователей чата:" << '\n' << std::endl;
	for (size_t i = 0; i < users.size(); ++i) std::cout << users[i] << std::endl;
	return 0;
}