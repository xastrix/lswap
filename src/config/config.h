#pragma once

#include <string>

#define LANG_ARRAY_MAXSIZE 44

struct cfg_t {
	std::string source_lang;
	std::string target_lang;
};

namespace config
{
	/**
	 * Load configuration file
	 * @return cfg_t struct
	*/
	cfg_t init();

	/**
	 * Change values in configuration file
	 * @param source language
	 * @param target language
	 * @return boolean of successful
	*/
	bool change_cfg_values(const std::string& source_lang, const std::string& target_lang);

	/*
	 * List of available languages
	*/
	inline std::string lang_arr[LANG_ARRAY_MAXSIZE][2] = {
		{ "en", "English" }, { "ru", "Russian" },
	    { "uk", "Ukrainian" }, { "be", "Belarusian" },
	    { "de", "Deutsch" }, { "fr", "French" },
	    { "he", "Hebrew" }, { "af", "Afrikaans" },
	    { "ar", "Arabic" }, { "hy", "Armenian" },
	    { "az", "Azerbaijani" }, { "pl", "Polish" },
	    { "da", "Danish" }, { "bg", "Bulgarian" },
	    { "sr", "Serbian" }, { "es", "Spanish" },
	    { "ja", "Japanese" }, { "it", "Italian" },
	    { "ko", "Korean" }, { "sv", "Swedish" },
	    { "et", "Estonian" }, { "cs", "Czech" },
	    { "fi", "Finnish" }, { "fy", "Frisian" },
	    { "hi", "Hindi" }, { "tr", "Turkish" },
	    { "tk", "Turkmen" }, { "tg", "Tajik" },
	    { "tt", "Tatar" }, { "ro", "Romanian" },
	    { "sk", "Slovak" }, { "mo", "Moldavian" },
	    { "mn", "Mongolian" }, { "la", "Latin" },
	    { "id", "Indonesian" }, { "kk", "Kazakh" },
	    { "uz", "Uzbek" }, { "lv", "Latvian" },
	    { "th", "Thai" }, { "ur", "Urdu" },
	    { "ku", "Kurdish" }, { "no", "Norwegian" },
	    { "pt", "Portuguese" }, { "hu", "Hungarian" },
	};
}