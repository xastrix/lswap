#include "common.h"
#include "cli/cli.h"
#include "fmt/fmt.h"
#include "hooks/hooks.h"
#include "utils/utils.h"

int main(int argc, const char** argv)
{
	g::cfg = config::init();

	cli cli {
		LSWAP_APPLICATION_NAME " (" LSWAP_VERSION_STRING ") is a command line tool designed for swift translation between languages by simply copying and pasting data from the clipboard\n\n"
		
		LSWAP_APPLICATION_NAME " run\n"
		"  Starting the application\n\n"

		LSWAP_APPLICATION_NAME " config <SourceLanguage> <TargetLanguage>\n"
		"  Change the source and target languages in the configuration file\n"
	};

	cli.add("run", [](int ac, arguments_t args) {
		auto mutex = CreateMutexA(NULL, FALSE, LSWAP_MUTEX_NAME);

		if (mutex == NULL) {
			fmt{ fmt_30ms, fc_red, "fatal: failed to create mutex" };
			return;
		}

		else if (GetLastError() == ERROR_ALREADY_EXISTS) {
			CloseHandle(mutex);
			fmt{ fmt_30ms, fc_red, "fatal: %s is already running", LSWAP_APPLICATION_NAME };
			return;
		}

		hooks::init();

		fmt{ fmt_def, fc_none, "the service has been launched...\n" };
		fmt{ fmt_def, fc_none, "config: %s > %s\n", g::cfg.source_lang.c_str(), g::cfg.target_lang.c_str() };

		MSG msg;
		while (GetMessageA(&msg, NULL, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}

		hooks::free();

		ReleaseMutex(mutex);
		CloseHandle(mutex);
	});

	cli.add("config", [](int ac, arguments_t args) {
		if (ac != 2) {
			fmt{ fmt_def, fc_none, "current configuration: %s > %s\n", g::cfg.source_lang.c_str(), g::cfg.target_lang.c_str() };
			fmt{ fmt_def, fc_none, "for change it, type \"%s config <SourceLanguage> <TargetLanguage>\"\n", LSWAP_APPLICATION_NAME };
			return;
		}

		if (!config::change_cfg_values(args[1], args[2]))
			return;

		g::cfg = config::init(); // Reload configuration

		fmt{ fmt_30ms, fc_green, "the configuration file has been updated!" };
		fmt{ fmt_def, fc_none, "updated configuration: %s > %s\n", g::cfg.source_lang.c_str(), g::cfg.target_lang.c_str() };
	});

	return cli.parse(argc, argv);
}