#pragma once

#include <string>

#define LSWAP_APPLICATION_NAME       "lswap"
#define LSWAP_APPLICATION_CLASS_NAME LSWAP_APPLICATION_NAME "001"
#define LSWAP_VERSION_STRING         "v1.5.windows"
#define LSWAP_CONFIGURATION_FILENAME "." LSWAP_APPLICATION_NAME "conf"
#define LSWAP_MUTEX_NAME             LSWAP_APPLICATION_NAME ".mutex"

struct cfg_t {
	std::string source_lang;
	std::string target_lang;
};

namespace config
{
	/**
	 * Load configuration file
	 * @return configratuion struct with values
	*/
	cfg_t init();

	/**
	 * Change values in configuration file
	 * @param source language (std::string)
	 * @param target language (std::string)
	 * @return configratuion struct with values
	*/
	bool change_cfg_values(const std::string& source_lang, const std::string& target_lang);
}