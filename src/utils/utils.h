#pragma once

#include <windows.h>
#include <string>

/*
* String of forbidden characters that will be deleted
* @description this is needed for curl_easy_escape() function to avoid some bugs
*/
#define FORBIDDEN_CHARS L"@#$%^&*\"\\/:\r\n\t<>(){}"

namespace utils
{
	/*
	* It sets the current locale for the entire program, which can affect how the program
	* processes strings, formats text, and handles date and time
	* @inside setlocale(LC_ALL, "");
	*/
	void set_locale();

	/*
	* Remove characters from wstring
	* @param wstring
	* @param chars to remove
	* @return result in std::wstring
	*/
	std::wstring remove_chars(const std::wstring& str, const std::wstring& chars);

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
	* Parse json
	* @param json data
	* @return result in std::wstring
	*/
	std::wstring parse_json(const std::wstring& json);

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
	std::string to_utf8(const std::wstring& str);
}