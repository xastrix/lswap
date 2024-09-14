#include "utils.h"

#include <unordered_map>
#include <codecvt>

static char hex_char(unsigned int v)
{
	if (v < 10) return v + '0';
	return v - 10 + 'A';
}

std::string utils::replace_characters(const std::string& str, const std::string& chars_to_replace, const std::string& replacements)
{
	std::unordered_map<char, std::string> replacement_map;

	for (size_t i = 0; i < chars_to_replace.size(); ++i) {
		replacement_map[chars_to_replace[i]] = replacements;
	}

	std::string result;
	for (char c : str) {
		if (replacement_map.find(c) != replacement_map.end()) {
			result += replacement_map[c];
		}
		else {
			result += c;
		}
	}

	return result;
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
	std::wstring result;
	size_t start = json.find(L"\"");
	size_t end = json.find(L"\"", start + 1);

	if (start != std::wstring::npos && end != std::wstring::npos) {
		result = json.substr(start + 1, end - start - 1);
	}

	return result;
}

size_t utils::wc(void* contents, size_t size, size_t nmemb, void* userp)
{
	std::wstring* mem = (std::wstring*)userp;
	size_t rlsize = size * nmemb;
	char* wcontents = new char[rlsize];
	std::memcpy(wcontents, contents, rlsize);

	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	std::wstring w_mem = converter.from_bytes(std::string(wcontents, wcontents + rlsize));
	delete[] wcontents;

	mem->append(w_mem.begin(), w_mem.end());

	return rlsize;
}

std::string utils::to_utf8(const std::wstring& str)
{
	static std::wstring_convert<std::codecvt_utf8<wchar_t>> u8;
	return u8.to_bytes(str);
}