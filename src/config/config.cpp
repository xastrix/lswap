#include "config.h"

#include <fstream>
#include <sstream>
#include <filesystem>

#include "../common.h"
#include "../fmt/fmt.h"
#include "../utils/utils.h"

static cfg_t read_values()
{
	cfg_t cfg;

	std::ifstream f{ utils::get_user_directory() + "\\" LSWAP_CONFIGURATION_FILENAME };

	if (!f.is_open()) {
		return cfg;
	}

	std::string line, content;
	while (std::getline(f, line)) {
		content += line + "\n";
	}

	f.close();

	std::stringstream ss{ content };
	while (std::getline(ss, line)) {
		if (line.empty() || line[0] == '#') {
			continue;
		}

		size_t equal_pos = line.find('=');
		if (equal_pos == std::string::npos) {
			continue;
		}

		std::string key = line.substr(0, equal_pos);
		std::string value = line.substr(equal_pos + 1);

		value = value.substr(1, value.length() - 2);

		if (key == "Source") {
			cfg.source_lang = value;
		}

		else if (key == "Target") {
			cfg.target_lang = value;
		}
	}

	return cfg;
}

static bool set_config_values(const std::string& source_lang, const std::string& target_lang)
{
	std::ofstream f{ utils::get_user_directory() + "\\" LSWAP_CONFIGURATION_FILENAME };

	if (!f.is_open())
		return false;

	f << "# " LSWAP_APPLICATION_NAME " (" LSWAP_VERSION_STRING ")\n\n";

	f << "Source=\"" << source_lang << "\"\n";
	f << "Target=\"" << target_lang << "\"";
	
	f.close();

	return true;
}

cfg_t config::init()
{
	cfg_t cfg;

	cfg.source_lang = lang_arr[0][0];
	cfg.target_lang = lang_arr[4][0];

	if (!std::filesystem::exists(utils::get_user_directory() + "\\" LSWAP_CONFIGURATION_FILENAME))
		set_config_values(cfg.source_lang, cfg.target_lang);

	return cfg = read_values();
}

bool config::change_cfg_values(const std::string& source_lang, const std::string& target_lang)
{
	bool source_found = false;
	bool target_found = false;

	for (int i = 0; i < LANG_ARRAY_MAXSIZE; i++)
	{
		if (lang_arr[i][0] == source_lang) source_found = true;
		if (lang_arr[i][0] == target_lang) target_found = true;
	}

	if (!source_found || !target_found)
	{
		fmt{ fmt_30ms, fc_none, "Failed to change the configuration" };

		if (!source_found)
			fmt{ fmt_30ms, fc_none, "fatal: Source language (%s) not found in database", source_lang.c_str() };

		if (!target_found)
			fmt{ fmt_30ms, fc_none, "fatal: Target language (%s) not found in database", target_lang.c_str() };

		fmt{ fmt_def, fc_none, "\n" };
		fmt{ fmt_def, fc_none, "List of available languages:\n" };

		for (int i = 0; i < LANG_ARRAY_MAXSIZE; i++) {
			fmt{ fmt_def, fc_none, "  \"%s\" - %s", lang_arr[i][0].c_str(), lang_arr[i][1].c_str() };
			if (i != (LANG_ARRAY_MAXSIZE - 1))
				fmt{ fmt_def, fc_none, ",\n" };
		}

		return false;
	}

	return set_config_values(source_lang, target_lang);
}