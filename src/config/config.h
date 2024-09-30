#pragma once

#include <string>

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