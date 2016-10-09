#include "settings.h"

Settings *Settings::getInstance() {
	static Settings *instance = nullptr;

	if (instance == nullptr) {
		instance = new Settings();
		instance->load("/home/dev/dev/e2dit/settings.e2t");  // TODO: Hardcode path
	}

	return instance;
}

void Settings::load(const std::string &fileName, DataMap::ReadType rt) {
	data = std::make_unique<DataMap>();
	data->hierarchy = true;

	switch (rt) {
		case DataMap::ReadType::Bin  : loadFromBin  (fileName); break;
		case DataMap::ReadType::Text : loadFromText (fileName); break;
	}
}

void Settings::save(const std::string &fileName, DataMap::ReadType rt) {

}

void Settings::extractData(DataMap::DataNode *rootNode) {
	auto general = rootNode->findChild("general");

}
