#ifndef SAVE_HPP
#define SAVE_HPP
#include <string>

struct GameSave {
	GameSave ();
	void save();

	std::string name, password, server;
	bool readSuccess = false;
};

#endif // SAVE_HPP
