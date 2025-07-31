#pragma once

#include "../common.h"
#include "../cli/cli.h"
#include "../fmt/fmt.h"
#include "../utils/utils.h"

#include <curl/curl.h>

enum _cmd_list {
	cl_run,
	cl_config,
	cl_autorun,
};

inline const char* _cmd[] = {
	"run",
	"c/config",
	"ar/autorun",
};

enum clipboard_state {
	cb_idle,
	cb_processing,
};

enum resp_codes : long {
	resp_ok_code = 200,
	bad_request_code = 400,
	unauthorized_code = 401,
	forbidden_code = 403,
	not_found_code = 404,
	internal_server_err_code = 500,
	bad_gateway_code = 502,
	service_unavailable_code = 503,
};

namespace client
{
	void init();
	void free();

	namespace globals
	{
		inline HWND            g_hwnd = NULL;
		inline HANDLE          g_mutex = NULL;
		inline clipboard_state g_state = cb_idle;
		inline cfg_t           g_cfg = {};
	}

	inline void curl_escape_url(CURL* curl, const std::wstring& in, std::string& url)
	{
		char* esc = curl_easy_escape(curl, utils::to_utf8(in).c_str(), 0);

		if (esc) {
			url += esc;
			curl_free(esc);
		}
	}

	inline long __stdcall win_proc_h(HWND h, UINT m, WPARAM w, LPARAM l)
	{
		switch (m) {
		case WM_CLIPBOARDUPDATE: {
			if (globals::g_state == cb_processing) {
				globals::g_state = cb_idle;
				break;
			}

			if (auto* curl = curl_easy_init())
			{
				auto current_clipboard = utils::get_current_clipboard(globals::g_hwnd);

				if (!current_clipboard.empty())
				{
					std::string url;
					curl_escape_url(curl, current_clipboard, url = std::string {
						"https://translate.googleapis.com/translate_a/single?client=gtx&sl=" +
						globals::g_cfg.source_lang + "&tl=" + globals::g_cfg.target_lang + "&dt=t&q="
					});

					std::wstring buffer;

					curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
					curl_easy_setopt(curl, CURLOPT_USERAGENT, LSWAP_APPLICATION_NAME "/" LSWAP_VERSION_STRING);
					curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, utils::write_callback);
					curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
					curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_0);

					auto res = curl_easy_perform(curl);

					if (res == CURLE_OK)
					{
						resp_codes response_code;
						curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

						if (response_code == resp_ok_code)
						{
							auto json_res = utils::parse_json(buffer, globals::g_cfg);

							if (!json_res.empty())
								utils::put_in_clipboard(globals::g_hwnd, json_res);

							globals::g_state = cb_processing;
						}
						else
						{
							std::wstring error_msg;

							switch (response_code) {
							case bad_request_code: {
								error_msg = L"(Google Translate Service) Bad Request - The server could not understand the request due to invalid syntax";
								break;
							}
							case unauthorized_code: {
								error_msg = L"(Google Translate Service) Unauthorized - The client must authenticate itself to get the requested response";
								break;
							}
							case forbidden_code: {
								error_msg = L"(Google Translate Service) Forbidden - The client does not have access rights to the content";
								break;
							}
							case not_found_code: {
								error_msg = L"(Google Translate Service) Not Found - The server can not find the requested resource";
								break;
							}
							case internal_server_err_code: {
								error_msg = L"(Google Translate Service) Internal Server Error - The server has encountered a situation it doesn't know how to handle";
								break;
							}
							case bad_gateway_code: {
								error_msg = L"(Google Translate Service) Bad Gateway - The server was acting as a gateway or proxy and received an invalid response from the upstream server";
								break;
							}
							case service_unavailable_code: {
								error_msg = L"(Google Translate Service) Service Unavailable - The server is not ready to handle the request";
								break;
							}
							default: {
								error_msg = L"(Google Translate Service) Unexpected Error - Returned " + std::to_wstring(response_code) + L" code";
								break;
							}
							}

							utils::put_in_clipboard(globals::g_hwnd, L"curl: " + error_msg);
						}
					}
					else {
						std::string msg = curl_easy_strerror(res);
						utils::put_in_clipboard(globals::g_hwnd, L"curl: " + std::wstring{ msg.begin(), msg.end() });
					}
				}

				curl_easy_cleanup(curl);
			}
			break;
		}
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
		}

		return DefWindowProcA(h, m, w, l);
	}

	inline void free_class()
	{
		if (globals::g_hwnd != NULL) {
			RemoveClipboardFormatListener(globals::g_hwnd);
			UnregisterClassA(LSWAP_APPLICATION_CLASS_NAME, GetModuleHandleA(NULL));
			globals::g_hwnd = NULL;
		}
	}

	inline void init_class()
	{
		WNDCLASS wc = { 0 };
		wc.lpfnWndProc = win_proc_h;
		wc.hInstance = GetModuleHandleA(NULL);
		wc.lpszClassName = LSWAP_APPLICATION_CLASS_NAME;

		RegisterClassA(&wc);

		globals::g_hwnd = CreateWindowExA(0, wc.lpszClassName, LSWAP_APPLICATION_NAME, 0, 0, 0, 0, 0, NULL, NULL, wc.hInstance, NULL);

		if (globals::g_hwnd == NULL) {
			fmt{ fmt_def, fc_red, "fatal: CreateWindowEx(...) == NULL\n" }.die();
		}

		if (AddClipboardFormatListener(globals::g_hwnd) == FALSE) {
			free_class();
			fmt{ fmt_def, fc_red, "fatal: AddClipboardFormatListener(...) == FALSE\n" }.die();
		}
	}

	inline void free_mutex()
	{
		ReleaseMutex(globals::g_mutex);
		CloseHandle(globals::g_mutex);
	}

	inline void init_mutex()
	{
		globals::g_mutex = CreateMutexA(NULL, FALSE, LSWAP_MUTEX_NAME);

		if (globals::g_mutex == NULL)
			fmt{ fmt_30ms, fc_red, "fatal: failed to create mutex" }.die();

		else if (GetLastError() == ERROR_ALREADY_EXISTS) {
			CloseHandle(globals::g_mutex);
			fmt{ fmt_30ms, fc_red, "warning: %s is already running", LSWAP_APPLICATION_NAME }.die();
		}
	}
}