#include "common.h"
#include "globals.h"
#include "cli/cli.h"
#include "fmt/fmt.h"
#include "hooks/hooks.h"
#include "utils/utils.h"

int main(int argc, const char** argv)
{
	setlocale(LC_ALL, "");
	g::cfg = config::init();

	cli cli{ []() {
		fmt{ fmt_30ms, fc_none, "%s is a command line tool designed for swift translation between languages by simply copying and pasting data from the clipboard", LSWAP_APPLICATION_NAME };
		fmt{ fmt_30ms, fc_none, "%s version %s\n", LSWAP_APPLICATION_NAME, LSWAP_VERSION_STRING };

		fmt{ fmt_def, fc_cyan, "  %s > %s\n\n", g::cfg.source_lang.c_str(), g::cfg.target_lang.c_str() };

		fmt{ fmt_def, fc_none, "%s r/run --log\n", LSWAP_APPLICATION_NAME };
		fmt{ fmt_def, fc_cyan, "      With logging (Displaying nontranslated and translated results)\n\n" };

		fmt{ fmt_def, fc_none, "%s c/config <SourceLanguage> <TargetLanguage>\n", LSWAP_APPLICATION_NAME };
		fmt{ fmt_def, fc_cyan, "      Change the source and target languages in the configuration file\n" };
	} };

	cli.add("r/run", [](int ac, arguments_t args) {
		HANDLE mutex = CreateMutex(NULL, FALSE, LSWAP_MUTEX_NAME);

		if (GetLastError() == ERROR_ALREADY_EXISTS) {
			CloseHandle(mutex);
			fmt{ fmt_30ms, fc_red, "fatal: %s is already running", LSWAP_APPLICATION_NAME }.die();
		}

		if (ac == 1 && args[1] == "--log")
			g::m_log = true;

		hooks::init();

		fmt{ fmt_def, fc_none, "\n" };
		fmt{ fmt_def, fc_cyan, "  %s > %s\n\n", g::cfg.source_lang.c_str(), g::cfg.target_lang.c_str() };

		fmt{ fmt_30ms, fc_yellow, "Hint: Highlight your text (CTRL + C) and paste (CTRL + V)" };

		if (g::m_log)
			fmt{ fmt_def, fc_blue, "Info: Running in logging mode\n\n" };

		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		hooks::free();

		ReleaseMutex(mutex);
		CloseHandle(mutex);
	});

	cli.add("c/config", [](int ac, arguments_t args) {
		if (ac != 2) {
			fmt{ fmt_30ms, fc_none, "Invalid Arguments. Usage: %s c/config <source_lang> <target_lang>", LSWAP_APPLICATION_NAME }.die();
		}

		if (config::change_cfg_values(args[1], args[2])) {
			fmt{ fmt_30ms, fc_green, "The configuration file has been updated!" };

			g::cfg = config::init(); // Reload configuration
			fmt{ fmt_def, fc_none, "Updated: %s > %s\n", g::cfg.source_lang.c_str(), g::cfg.target_lang.c_str() };
		}
	});

	return cli.parse(argc, argv);
}