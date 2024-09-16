#include "utils.h"

#include <locale>
#include <unordered_map>
#include <codecvt>

void utils::set_locale()
{
	setlocale(LC_ALL, "");
}

std::wstring utils::remove_chars(const std::wstring& str, const std::wstring& chars)
{
	std::vector<wchar_t> chars_to_remove;
	for (size_t i = 0; i < chars.length(); ++i)
	{
		chars_to_remove.push_back(chars[i]);
	}

	std::wstring mod;

	for (size_t i = 0; i < str.length(); ++i)
	{
		if (std::find(chars_to_remove.begin(), chars_to_remove.end(), str[i]) == chars_to_remove.end())
		{
			mod += str[i];
		}
	}

	return mod;
}

std::wstring utils::get_current_clipboard(HWND hwnd)
{
	if (!OpenClipboard(hwnd)) {
		return L"";
	}

	HANDLE h = GetClipboardData(CF_UNICODETEXT);

	if (!h) {
		CloseClipboard();
		return L"";
	}

	wchar_t* data = (wchar_t*)GlobalLock(h);
	if (!data) {
		CloseClipboard();
		return L"";
	}

	std::wstring result = data;

	GlobalUnlock(h);
	CloseClipboard();

	return result;
}

void utils::put_in_clipboard(HWND hwnd, const std::wstring& data)
{
	if (!OpenClipboard(hwnd)) {
		return;
	}

	EmptyClipboard();

	HGLOBAL h = GlobalAlloc(GMEM_MOVEABLE, (data.length() + 1) * sizeof(wchar_t));
	if (h == NULL) {
		CloseClipboard();
		return;
	}

	wchar_t* text = (wchar_t*)GlobalLock(h);
	if (text == NULL) {
		GlobalFree(h);
		CloseClipboard();
		return;
	}

	wcscpy(text, data.c_str());

	GlobalUnlock(h);
	SetClipboardData(CF_UNICODETEXT, h);
	CloseClipboard();
}

std::wstring utils::parse_json(const std::wstring& json)
{
	size_t start = json.find(L"\"");
	size_t end = json.find(L"\"", start + 1);

	std::wstring result;

	if (start != std::wstring::npos && end != std::wstring::npos) {
		result = json.substr(start + 1, end - start - 1);
	}

	return result;
}

size_t utils::write_callback(void* contents, size_t size, size_t nmemb, void* userp)
{
	size_t rlsize = size * nmemb;
	char* wcontents = new char[rlsize];
	std::memcpy(wcontents, contents, rlsize);

	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	std::wstring w_mem = converter.from_bytes(std::string(wcontents, wcontents + rlsize));

	delete[] wcontents;

	std::wstring* mem = (std::wstring*)userp;
	mem->append(w_mem.begin(), w_mem.end());

	return rlsize;
}

std::string utils::to_utf8(const std::wstring& str)
{
	static std::wstring_convert<std::codecvt_utf8<wchar_t>> u8;
	return u8.to_bytes(str);
}