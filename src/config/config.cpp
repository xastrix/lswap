#include "config.h"

#include <fstream>
#include <sstream>
#include <cstring>

cfg_t config::init()
{
	cfg_t cfg;

	std::string user_directory = getenv("USERPROFILE");
	std::string path = user_directory + "\\" + LSWAP_CONFIGURATION_FILENAME;

	cfg.source_lang = "ru";
	cfg.target_lang = "en";

	auto exists = [](const char* path) {
		struct stat s;
		if (stat(path, &s) == 0) {
			if (s.st_mode & S_IFREG) {
				return true;
			}
		}
		return false;
	};

	if (!exists(path.c_str())) {
		FILE* f = fopen(path.c_str(), "w");
		if (f) {
			fprintf(f, "source_lang=ru\n");
			fprintf(f, "target_lang=en");
		}
		fclose(f);
	}

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

void config::change_cfg_values(const std::string& source_lang, const std::string& target_lang)
{
	std::string user_directory = getenv("USERPROFILE");
	std::string path = user_directory + "\\" + LSWAP_CONFIGURATION_FILENAME;

	FILE* f = fopen(path.c_str(), "w");

	if (f) {
		fprintf(f, "source_lang=%s\n", source_lang.c_str());
		fprintf(f, "target_lang=%s", target_lang.c_str());
	}

	fclose(f);
}