#include "common.h"
#include "globals.h"
#include "cli/cli.h"
#include "fmt/fmt.h"
#include "hooks/hooks.h"
#include "utils/utils.h"

int main(int argc, const char** argv)
{
	g::cfg = config::init();

	cli cli {
		LSWAP_APPLICATION_NAME " (" LSWAP_VERSION_STRING ") is a command line tool designed for swift translation between languages by simply copying and pasting data from the clipboard\n\n"
		
		LSWAP_APPLICATION_NAME " r, run\n"
		"  Starting the application\n\n"

		LSWAP_APPLICATION_NAME " c, config <SourceLanguage> <TargetLanguage>\n"
		"  Change the source and target languages in the configuration file\n"
	};

	cli.add("r/run", [](int ac, arguments_t args) {
		HANDLE mutex = CreateMutexA(NULL, FALSE, LSWAP_MUTEX_NAME);

		if (mutex == NULL) {
			fmt{ fmt_30ms, fc_red, "fatal: failed to create mutex" }.die();
		}

		else if (GetLastError() == ERROR_ALREADY_EXISTS) {
			CloseHandle(mutex);
			fmt{ fmt_30ms, fc_red, "fatal: %s is already running", LSWAP_APPLICATION_NAME }.die();
		}

		hooks::init();

		fmt{ fmt_def, fc_cyan, "config: %s > %s\n", g::cfg.source_lang.c_str(), g::cfg.target_lang.c_str() };
		fmt{ fmt_30ms, fc_yellow, "hint: highlight your text (ctrl + c) and paste (ctrl + v)" };

		MSG msg;
		while (GetMessageA(&msg, NULL, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}

		hooks::free();

		ReleaseMutex(mutex);
		CloseHandle(mutex);
	});

	cli.add("c/config", [](int ac, arguments_t args) {
		if (ac != 2) {
			fmt{ fmt_30ms, fc_none, "warning: Invalid arguments, -- %s config <SourceLanguage> <TargetLanguage>", LSWAP_APPLICATION_NAME }.die();
		}

		if (config::change_cfg_values(args[1], args[2])) {
			fmt{ fmt_30ms, fc_green, "the configuration file has been updated!" };

			g::cfg = config::init(); // Reload configuration
			fmt{ fmt_def, fc_none, "updated: %s > %s\n", g::cfg.source_lang.c_str(), g::cfg.target_lang.c_str() };
		}
	});

	return cli.parse(argc, argv);
}