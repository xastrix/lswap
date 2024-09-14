#include "config.h"

#include <fstream>
#include <sstream>
#include <cstring>

config g_config;

cfg_t config::init()
{
	cfg_t cfg;

	std::string user_directory = getenv("USERPROFILE");
	std::string path = user_directory + "\\" + LSWAP_CONFIGURATION_FILENAME;

	std::ifstream f(path);

	if (!f.is_open()) {
		return cfg;
	}

	std::string content;

	std::string line;
	while (std::getline(f, line)) {
		content += line + "\n";
	}

	f.close();

	std::stringstream ss(content);

	cfg.source_lang = "?";
	cfg.target_lang = "?";

	while (std::getline(ss, line)) {
		if (line.empty() || line[0] == '#') {
			continue;
		}

		if (line.rfind("source_lang=", 0) == 0) {
			cfg.source_lang = line.substr(12);
		}
		else if (line.rfind("target_lang=", 0) == 0) {
			cfg.target_lang = line.substr(12);
		}
	}

	return cfg;
}