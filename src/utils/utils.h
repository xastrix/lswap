#pragma once

#include <windows.h>
#include <string>

#define FORBIDDEN_CHARS L"@#$%^&*\"\\/:\r\n<>(){}"

namespace utils
{
	void set_locale();
	std::wstring remove_chars(const std::wstring& str, const std::wstring& chars);
	std::wstring get_current_clipboard(HWND hwnd);
	void put_in_clipboard(HWND hwnd, const std::wstring& data);
	std::wstring parse_json(const std::wstring& json);
	size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp);
	std::string to_utf8(const std::wstring& str);
}