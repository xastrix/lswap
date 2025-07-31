#pragma once

#include <windows.h>
#include <string>
#include <vector>

#include "../config/config.h"

namespace utils
{
	/*
	 * Get the directory of the current user
	 * @return directory
	*/
	std::string get_user_directory();

	/*
	 * Replace patterns in a given string
	 * This function takes an input string and replaces all occurrences of
	 * patterns defined in the a_list with corresponding patterns in b_list.
	 * The function performs the replacements in the order they appear in a_list.
	 * @param input The original string in which patterns will be replaced
	 * @param a_list A vector of strings containing the patterns to be replaced
	 * @param b_list A vector of strings containing the replacement patterns
	 * corresponding to those in a_list.
	 * Must be the same size as a_list.
	 * @return A new string with all occurrences of patterns in a_list replaced
	 * by corresponding patterns in b_list.
	*/
	std::wstring replace_patterns(const std::wstring& input,
		const std::vector<std::wstring>& a_list,
		const std::vector<std::wstring>& b_list);

	/*
	 * Get clipboard data
	 * @param hwnd
	 * @return result in std::wstring
	*/
	std::wstring get_current_clipboard(HWND hwnd);
	
	/*
	 * Write data to clipboard
	 * @param hwnd
	 * @param data to write
	*/
	void put_in_clipboard(HWND hwnd, const std::wstring& data);
	
	/*
	 * Parse JSON string and extract relevant data
	 * This function takes a JSON string and a configuration object as input,
	 * extracts the strings within double quotes, filters out ignored tables,
	 * and returns a new string with the extracted data.
	 * @param json The input JSON string to be parsed
	 * @param cfg A configuration object containing information about the source language
	 * @return A new string with the extracted data
	*/
	std::wstring parse_json(const std::wstring& json, cfg_t cfg);

	/*
	 * Write callback function (Using in cURL)
	 * @param pointer to the data to be written
	 * @param size of the data to be written
	 * @param number of elements in the data to be written
	 * @param user pointer passed to the cURL call
	 * @return number of bytes written
	*/
	size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp);

	/*
	 * Converts a wstring to a UTF-8 string
	 * @param the wstring to be converted
	 * @return result in std::string
	*/
	std::string to_utf8(const std::wstring& string);

	/*
	 * Checks whether the specified application is set to run automatically at startup.
	 * @param app_name - the name of the application to check.
	 * @return true if the application is configured to autorun, false otherwise.
	*/
	bool is_in_autorun(const std::string& app_name);

	/*
	 * Adds or removes an application from the system's autorun list.
	 * @param app_name - the name of the application to add or remove.
	 * @param v - if true, adds the application to autorun; if false, removes it.
	*/
	void put_in_autorun(const std::string& app_name, bool v);
}