#include "hooks.h"

#include <curl/curl.h>

HHOOK m_h = NULL;
HWND  m_hwnd = NULL;
int   m_processing = 0;
int   m_processing_press = 0;
bool  m_hidden = false;

static long __stdcall keyboard_proc_h(int c, WPARAM w, LPARAM l)
{
	switch (c) {
	case HC_ACTION: {

		if ((GetAsyncKeyState(VK_CONTROL) & 0x8000))
		{
			KBDLLHOOKSTRUCT* k = (KBDLLHOOKSTRUCT*)l;

			if (k->vkCode == 'X')
			{
				if (m_processing_press == 1) {
					m_processing_press = 0;
					break;
				}

				ShowWindow(GetConsoleWindow(), m_hidden ? SW_SHOW : SW_HIDE);
				m_hidden =! m_hidden;

				m_processing_press = 1;
			}
		}

		break;
	}
	}

	return CallNextHookEx(m_h, c, w, l);
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

			std::wstring clipboard = utils::remove_chars(utils::get_current_clipboard(m_hwnd), FORBIDDEN_CHARS);

			char* esc = curl_easy_escape(curl, utils::to_utf8(clipboard).c_str(), 0);
			if (esc) {
				url += esc;
				curl_free(esc);
			}

			std::wstring buffer;

			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
			curl_easy_setopt(curl, CURLOPT_USERAGENT, "lswap/" LSWAP_VERSION_STRING);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, utils::write_callback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
			curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_0);

			res = curl_easy_perform(curl);

			if (res == CURLE_OK)
			{
				std::wstring text = utils::parse_json(buffer);

				if (!text.empty())
				{
					if (g::m_log)
					{
						fmt{ fmt_def, fc_cyan, "LOG: " };
						wfmt{ fmt_def, fc_none, L"<NonTranslated> %ls\n", clipboard.c_str() };

						fmt{ fmt_def, fc_cyan, "LOG: " };
						wfmt{ fmt_def, fc_none, L"<Translated> %ls\n", text.c_str() };
					}

					utils::put_in_clipboard(m_hwnd, text);
					m_processing = 1;
				}
			}
		}

		curl_global_cleanup();

		return 0;
	}
	case WM_DESTROY: {
		RemoveClipboardFormatListener(m_hwnd);
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

	m_hwnd = CreateWindowEx(0, wc.lpszClassName, "lswap", 0, 0, 0, 0, 0, NULL, NULL, wc.hInstance, NULL);
	m_h = SetWindowsHookEx(WH_KEYBOARD_LL, keyboard_proc_h, NULL, 0);

	AddClipboardFormatListener(m_hwnd);
}

void hooks::free()
{
	if (m_h != NULL) {
		UnhookWindowsHookEx(m_h);
		m_h = NULL;
	}

	if (m_hwnd != NULL) {
		RemoveClipboardFormatListener(m_hwnd);
		UnregisterClass("lswap001", GetModuleHandle(NULL));
		m_hwnd = NULL;
	}
}