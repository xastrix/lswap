#include "utils.h"

#include <unordered_map>
#include <codecvt>
#include <algorithm>
#include <cctype>

static bool is_md5(const std::wstring& string)
{
	if (string.length() != 32)
		return false;

	for (wchar_t c : string) {
		if (!std::isxdigit(c)) {
			return false;
		}
	}

	return true;
}

std::string utils::get_user_directory()
{
	return getenv("USERPROFILE");
}

std::wstring utils::replace_patterns(const std::wstring& input,
	const std::vector<std::wstring>& a_list,
	const std::vector<std::wstring>& b_list) {
	std::wstring result = input;

	for (int i = 0; i < a_list.size(); ++i) {
		std::wstring::size_type pos = 0;
		while ((pos = result.find(a_list[i], pos)) != std::wstring::npos) {
			result.replace(pos, a_list[i].length(), b_list[i]);
			pos += b_list[i].length();
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

std::wstring utils::parse_json(const std::wstring& json, cfg_t cfg)
{
	std::vector<std::wstring> result;

	size_t pos = 0;
	while ((pos = json.find(L'"', pos)) != std::wstring::npos) {
		size_t begin_pos = pos + 1;
		size_t end_pos = begin_pos;

		while (end_pos < json.size()) {
			end_pos = json.find(L'"', end_pos);

			if (end_pos == std::wstring::npos)
				break;

			if (end_pos > 0 && json[end_pos - 1] == L'\\') {
				end_pos++;
				continue;
			}
			break;
		}

		if (end_pos != std::wstring::npos) {
			result.push_back({ json.substr(begin_pos, end_pos - begin_pos) });
			pos = end_pos + 1;
		}
		else {
			break;
		}
	}

	result.erase(std::remove_if(result.begin(), result.end(),
		[](const std::wstring& s) { return is_md5(s); }),
	result.end());

	std::vector<std::wstring> ignore_tables = {
		L"efficient_models_2022q2.md",
		L"offline",
	};

	ignore_tables.push_back({ cfg.source_lang.begin(), cfg.source_lang.end() });

	result.erase(std::remove_if(result.begin(), result.end(), [&ignore_tables](const std::wstring& item) {
		return std::find(ignore_tables.begin(), ignore_tables.end(), item) != ignore_tables.end();
	}), result.end());

	std::vector<std::wstring> filtered;

	for (int i = 0; i < result.size(); i += 2) {
		filtered.push_back(result[i]);
	}

	std::wstring final;

	for (const auto& word : filtered) {
		final += replace_patterns(word, { L"\\r\\n" }, { L"\n" });
	}

	return final;
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