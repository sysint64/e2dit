#pragma once

#include <map>
#include "shortkey.h"
#include "commands/command.h"


class Settings {
public:
	std::map<ShortKey, Command> shortKeys;

	void save(const std::string &fileName);
	void load(const std::string &fileName);

	static Settings *getInstance();
};
