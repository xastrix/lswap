#include "hooks.h"

#include "../common.h"
#include "../globals.h"
#include "../fmt/fmt.h"
#include "../utils/utils.h"

#include <locale>
#include <curl/curl.h>

enum clipboard_state {
	cb_idle,
	cb_processing,
};

HWND            m_hwnd  = NULL;
clipboard_state m_state = cb_idle;

static void curl_escape_url(CURL* curl, const std::wstring& in, std::string& url)
{
	char* esc = curl_easy_escape(curl, utils::to_utf8(in).c_str(), 0);

	if (esc) {
		url += esc;
		curl_free(esc);
	}
}

static long __stdcall win_proc_h(HWND h, UINT m, WPARAM w, LPARAM l)
{
	switch (m) {
	case WM_CLIPBOARDUPDATE: {
		CURL* curl = curl_easy_init();
		CURLcode res;

		if (m_state == cb_processing) {
			m_state = cb_idle;
			curl_easy_cleanup(curl);
			break;
		}

		if (curl)
		{
			std::wstring buffer;
			std::wstring clipboard = utils::remove_chars(utils::get_current_clipboard(m_hwnd), FORBIDDEN_CHARS);

			std::string url;
			curl_escape_url(curl, clipboard, url = std::string {
				"https://translate.googleapis.com/translate_a/single?client=gtx&sl=" +
				g::cfg.source_lang + "&tl=" + g::cfg.target_lang + "&dt=t&q="
			});

			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
			curl_easy_setopt(curl, CURLOPT_USERAGENT, LSWAP_APPLICATION_NAME "/" LSWAP_VERSION_STRING);
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
					m_state = cb_processing;
				}
			}
		}
		break;
	}
	case WM_DESTROY: {
		RemoveClipboardFormatListener(m_hwnd);
		PostQuitMessage(0);
		break;
	}
	}

	return DefWindowProcA(h, m, w, l);
}

void hooks::init()
{
	setlocale(LC_ALL, "");

	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = win_proc_h;
	wc.hInstance = GetModuleHandleA(NULL);
	wc.lpszClassName = LSWAP_APPLICATION_CLASS_NAME;

	RegisterClassA(&wc);

	m_hwnd = CreateWindowExA(0, wc.lpszClassName, LSWAP_APPLICATION_NAME, 0, 0, 0, 0, 0, NULL, NULL, wc.hInstance, NULL);

	if (m_hwnd == NULL) {
		free();
		fmt{ fmt_def, fc_red, "fatal: CreateWindowEx() == NULL\n" }.die();
	}

	if (AddClipboardFormatListener(m_hwnd) == FALSE) {
		free();
		fmt{ fmt_def, fc_red, "fatal: AddClipboardFormatListener() == FALSE\n" }.die();
	}
}

void hooks::free()
{
	if (m_hwnd != NULL) {
		RemoveClipboardFormatListener(m_hwnd);
		UnregisterClassA(LSWAP_APPLICATION_CLASS_NAME, GetModuleHandle(NULL));
		m_hwnd = NULL;
	}
}