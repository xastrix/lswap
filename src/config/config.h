#pragma once

#include <string>

#define LSWAP_VERSION_STRING         "v1.2.windows"
#define LSWAP_CONFIGURATION_FILENAME ".lswapconf"

struct cfg_t {
	std::string source_lang;
	std::string target_lang;
};

namespace config
{
	cfg_t init();
	void change_cfg_values(const std::string& source_lang, const std::string& target_lang);
}