#pragma once

#include <map>
#include <utility/data_map.h>
#include "shortkey.h"
#include "commands/command.h"


class Settings {
private:
	Application *app = Application::getInstance();
	std::unique_ptr<DataMap> data = nullptr;

	void extractData(DataMap::DataNode *rootNode);

public:
	std::map<std::string, Command*> shortKeys;

	void save(const std::string &fileName, DataMap::ReadType rt = DataMap::ReadType::Text);
	void load(const std::string &fileName, DataMap::ReadType rt = DataMap::ReadType::Text);

	static Settings *getInstance();

	inline void loadFromText (const std::string &fileName) {
		data->loadFromText(fileName.c_str());
		extractData(data->root.get());
	}

	inline void loadFromBin (const std::string &fileName) {
		data->loadFromBin(fileName.c_str());
		extractData(data->root.get());
	}
};
