#include "hooks.h"

#include "../common.h"
#include "../fmt/fmt.h"
#include "../utils/utils.h"

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
			std::wstring current_clipboard = utils::get_current_clipboard(m_hwnd);

			if (!current_clipboard.empty())
			{
				std::string url;
				curl_escape_url(curl, current_clipboard, url = std::string {
					"https://translate.googleapis.com/translate_a/single?client=gtx&sl=" +
					g::cfg.source_lang + "&tl=" + g::cfg.target_lang + "&dt=t&q="
				});

				std::wstring buffer;

				curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
				curl_easy_setopt(curl, CURLOPT_USERAGENT, LSWAP_APPLICATION_NAME "/" LSWAP_VERSION_STRING);
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, utils::write_callback);
				curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
				curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_0);

				res = curl_easy_perform(curl);

				if (res == CURLE_OK)
				{
					std::wstring res = utils::parse_json(buffer, g::cfg);

					if (!res.empty())
						utils::put_in_clipboard(m_hwnd, res);

					m_state = cb_processing;
				}
				else {
					std::string msg = curl_easy_strerror(res);
					utils::put_in_clipboard(m_hwnd, L"curl: " + std::wstring{ msg.begin(), msg.end() });
				}
			}
		}
		break;
	}
	case WM_DESTROY: {
		hooks::free();
		PostQuitMessage(0);
		break;
	}
	}

	return DefWindowProcA(h, m, w, l);
}

void hooks::init()
{
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = win_proc_h;
	wc.hInstance = GetModuleHandleA(NULL);
	wc.lpszClassName = LSWAP_APPLICATION_CLASS_NAME;

	RegisterClassA(&wc);

	m_hwnd = CreateWindowExA(0, wc.lpszClassName, LSWAP_APPLICATION_NAME, 0, 0, 0, 0, 0, NULL, NULL, wc.hInstance, NULL);

	if (m_hwnd == NULL) {
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
		UnregisterClassA(LSWAP_APPLICATION_CLASS_NAME, GetModuleHandleA(NULL));
		m_hwnd = NULL;
	}
}