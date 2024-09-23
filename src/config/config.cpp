#include "config.h"

#include <fstream>
#include <sstream>
#include <filesystem>

std::string path;

static cfg_t read_values()
{
	cfg_t cfg;

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

static bool set_config_values(const std::string& source_lang, const std::string& target_lang)
{
	std::ofstream f(path);

	if (!f.is_open())
		return false;

	f << "# lswap configuration file\n\n";
	f << "source_lang=" << source_lang << "\n";
	f << "target_lang=" << target_lang;
	
	f.close();

	return true;
}

cfg_t config::init()
{
	cfg_t cfg;

	std::string user_directory = getenv("USERPROFILE");
	path = user_directory + "\\" LSWAP_CONFIGURATION_FILENAME;

	cfg.source_lang = "ru";
	cfg.target_lang = "en";

	if (!std::filesystem::exists(path))
		set_config_values(cfg.source_lang, cfg.target_lang);

	cfg = read_values();

	return cfg;
}

bool config::change_cfg_values(const std::string& source_lang, const std::string& target_lang)
{
	return set_config_values(source_lang, target_lang);
}