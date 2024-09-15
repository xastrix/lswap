#pragma once

#include <windows.h>
#include <string>

namespace utils
{
	std::string replace_characters(const std::string& str, const std::string& chars_to_replace, const std::string& replacements);
	std::wstring get_current_clipboard(HWND hwnd);
	void put_in_clipboard(HWND hwnd, const std::wstring& data);
	std::wstring parse_json(const std::wstring& json);
	size_t wc(void* contents, size_t size, size_t nmemb, void* userp);
	std::string to_utf8(const std::wstring& str);
}