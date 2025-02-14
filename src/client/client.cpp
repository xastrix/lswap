#include "client.h"

HWND            g_hwnd  = NULL;
HANDLE          g_mutex = NULL;
clipboard_state g_state = cb_idle;
cfg_t           g_cfg   = {};

static void init_mutex();
static void free_mutex();
static void init_class();
static void free_class();
static void curl_escape_url(CURL* curl, const std::wstring& in, std::string& url);
static long __stdcall win_proc_h(HWND h, UINT m, WPARAM w, LPARAM l);

int main(int argc, const char** argv)
{
	g_cfg = config::init();

	cli cli {
		LSWAP_APPLICATION_NAME " (" LSWAP_VERSION_STRING ") is a command line tool designed for swift translation between languages by simply copying and pasting data from the clipboard\n\n"

		LSWAP_APPLICATION_NAME " run\n"
		"  Starting the application\n\n"

		LSWAP_APPLICATION_NAME " config <SourceLanguage> <TargetLanguage>\n"
		"  Change the source and target languages in the configuration file\n"
	};

	cli.add("run", [](int ac, arguments_t args) {
		return client::init();
	});

	cli.add("config", [](int ac, arguments_t args) {
		if (ac != 2) {
			fmt{ fmt_def, fc_none, "Current configuration: %s > %s\n", g_cfg.source_lang.c_str(), g_cfg.target_lang.c_str() };
			fmt{ fmt_def, fc_none, "For change it, type \"%s config <SourceLanguage> <TargetLanguage>\"\n", LSWAP_APPLICATION_NAME };
			return;
		}

		if (config::change_cfg_values(args[1], args[2]))
		{
			g_cfg = config::init(); // Reload configuration

			fmt{ fmt_30ms, fc_green, "The configuration file has been updated!" };
			fmt{ fmt_def, fc_none, "Updated configuration: %s > %s\n", g_cfg.source_lang.c_str(), g_cfg.target_lang.c_str() };
			return;
		}

		fmt{ fmt_30ms, fc_none, "Failed to change the configuration, check permissions" };
	});

	return cli.parse(argc, argv);
}

void client::init()
{
	init_mutex();
	init_class();

	fmt{ fmt_def, fc_none, "The service has been launched...\n" };
	fmt{ fmt_def, fc_none, "Current configuration: %s > %s\n", g_cfg.source_lang.c_str(), g_cfg.target_lang.c_str() };

	MSG msg;
	while (GetMessageA(&msg, NULL, 0, 0)) {
		if (msg.message == WM_QUIT) break;
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}

	free();
}

void client::free()
{
	free_class();
	free_mutex();
}

static long __stdcall win_proc_h(HWND h, UINT m, WPARAM w, LPARAM l)
{
	switch (m) {
	case WM_CLIPBOARDUPDATE: {
		if (g_state == cb_processing) {
			g_state = cb_idle;
			break;
		}

		if (auto* curl = curl_easy_init())
		{
			auto current_clipboard = utils::get_current_clipboard(g_hwnd);

			if (!current_clipboard.empty())
			{
				std::string url;
				curl_escape_url(curl, current_clipboard, url = std::string {
					"https://translate.googleapis.com/translate_a/single?client=gtx&sl=" +
					g_cfg.source_lang + "&tl=" + g_cfg.target_lang + "&dt=t&q="
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
					auto res = utils::parse_json(buffer, g_cfg);

					if (!res.empty())
						utils::put_in_clipboard(g_hwnd, res);

					g_state = cb_processing;
				}
				else {
					std::string msg = curl_easy_strerror(res);
					utils::put_in_clipboard(g_hwnd, L"curl: " + std::wstring{ msg.begin(), msg.end() });
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

static void curl_escape_url(CURL* curl, const std::wstring& in, std::string& url)
{
	char* esc = curl_easy_escape(curl, utils::to_utf8(in).c_str(), 0);

	if (esc) {
		url += esc;
		curl_free(esc);
	}
}

static void free_class()
{
	if (g_hwnd != NULL) {
		RemoveClipboardFormatListener(g_hwnd);
		UnregisterClassA(LSWAP_APPLICATION_CLASS_NAME, GetModuleHandleA(NULL));
		g_hwnd = NULL;
	}
}

static void init_class()
{
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = win_proc_h;
	wc.hInstance = GetModuleHandleA(NULL);
	wc.lpszClassName = LSWAP_APPLICATION_CLASS_NAME;

	RegisterClassA(&wc);

	g_hwnd = CreateWindowExA(0, wc.lpszClassName, LSWAP_APPLICATION_NAME, 0, 0, 0, 0, 0, NULL, NULL, wc.hInstance, NULL);

	if (g_hwnd == NULL) {
		fmt{ fmt_def, fc_red, "fatal: CreateWindowEx() == NULL\n" }.die();
	}

	if (AddClipboardFormatListener(g_hwnd) == FALSE) {
		free_class();
		fmt{ fmt_def, fc_red, "fatal: AddClipboardFormatListener() == FALSE\n" }.die();
	}
}

static void free_mutex()
{
	ReleaseMutex(g_mutex);
	CloseHandle(g_mutex);
}

static void init_mutex()
{
	g_mutex = CreateMutexA(NULL, FALSE, LSWAP_MUTEX_NAME);

	if (g_mutex == NULL)
		fmt{ fmt_30ms, fc_red, "fatal: failed to create mutex" }.die();

	else if (GetLastError() == ERROR_ALREADY_EXISTS) {
		CloseHandle(g_mutex);
		fmt{ fmt_30ms, fc_red, "fatal: %s is already running", LSWAP_APPLICATION_NAME }.die();
	}
}