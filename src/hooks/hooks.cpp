#include "hooks.h"
#include "../globals.h"
#include "../fmt/fmt.h"
#include "../utils/utils.h"

#include <curl/curl.h>

HHOOK h = NULL;
HWND  hwnd = NULL;

int m_processing = 0;

static long __stdcall keyboard_proc_h(int c, WPARAM w, LPARAM l)
{
	switch (c) {
	case HC_ACTION: {
		KBDLLHOOKSTRUCT* k = (KBDLLHOOKSTRUCT*)l;

		if ((GetAsyncKeyState(VK_CONTROL) & 0x8000))
		{
			if (k->vkCode == 'X')
			{
				exit(EXIT_SUCCESS);
			}
		}

		break;
	}
	}

	return CallNextHookEx(h, c, w, l);
}

static long __stdcall win_proc_h(HWND h, UINT m, WPARAM w, LPARAM l)
{
	switch (m) {
	case WM_CLIPBOARDUPDATE: {
		CURL*    curl;
		CURLcode res;

		if (m_processing == 1) {
			m_processing = 0;
			return 0;
		}

		curl = curl_easy_init();

		if (curl)
		{
			std::string url = "https://translate.googleapis.com/translate_a/single?client=gtx&";

			url += "sl=" + g::cfg.source_lang + "&";
			url += "tl=" + g::cfg.target_lang + "&";
			url += "dt=t&q=";

			std::wstring buffer;
			std::string s = utils::to_utf8(utils::get_current_clipboard(hwnd));

			char* esc = curl_easy_escape(curl, s.c_str(), 0);
			if (esc) {
				url += esc;
				curl_free(esc);
			}

			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
			curl_easy_setopt(curl, CURLOPT_USERAGENT, "lswap/" LSWAP_VERSION_STRING);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, utils::wc);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
			curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_0);

			res = curl_easy_perform(curl);

			std::wstring translated = utils::parse_json(buffer);

			if (g::m_log)
			{
				fmt{ fmt_def, fc_cyan, "[Log] " };
				fmt{ fmt_def, fc_none, "<Url> %s\n", url.c_str() };

				fmt{ fmt_def, fc_cyan, "[Log] " };
				wfmt{ fmt_def, fc_none, L"<NonTranslated> %ls\n", utils::get_current_clipboard(hwnd).c_str() };

				fmt{ fmt_def, fc_cyan, "[Log] " };
				wfmt{ fmt_def, fc_none, L"<Translated> %ls\n", translated.c_str() };
			}

			utils::put_in_clipboard(hwnd, translated);
			m_processing = 1;
		}

		curl_global_cleanup();

		return 0;
	}
	case WM_DESTROY: {
		RemoveClipboardFormatListener(hwnd);
		PostQuitMessage(0);

		return 0;
	}
	}

	return DefWindowProc(h, m, w, l);
}

void hooks::init()
{
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = win_proc_h;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = "lswap001";

	RegisterClass(&wc);

	hwnd = CreateWindowEx(0, wc.lpszClassName, "lswap", 0, 0, 0, 0, 0, NULL, NULL, wc.hInstance, NULL);
	h = SetWindowsHookEx(WH_KEYBOARD_LL, keyboard_proc_h, NULL, 0);

	AddClipboardFormatListener(hwnd);
}

void hooks::free()
{
	if (h != NULL) {
		UnhookWindowsHookEx(h);
		h = NULL;
	}

	if (hwnd != NULL) {
		RemoveClipboardFormatListener(hwnd);
		UnregisterClass("lswap001", GetModuleHandle(NULL));
		hwnd = NULL;
	}
}