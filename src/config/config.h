#pragma once

#include <string>

#define LSWAP_VERSION_STRING         "v1.1.windows"
#define LSWAP_CONFIGURATION_FILENAME ".lswapconf"

struct cfg_t {
	std::string source_lang;
	std::string target_lang;
};

struct config {
	cfg_t init();
};

extern config g_config;