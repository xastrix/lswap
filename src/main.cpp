#include "fmt/fmt.h"
#include "cli/cli.h"
#include "hooks/hooks.h"
#include "globals.h"

#include <windows.h>
#include <locale>

int main(int argc, const char** argv)
{
	setlocale(LC_ALL, "");

	cli cli{ []() {
		fmt{ fmt_30ms, fc_none, "lswap is an fast translator of your text from the clipboard" };
		fmt{ fmt_30ms, fc_none, "lswap version %s\n", LSWAP_VERSION_STRING };

		cfg_t cfg = g_config.init();

		fmt{ fmt_def, fc_none, "lswap run\n" };
		fmt{ fmt_def, fc_cyan, "      To do ( %s > %s )\n\n", cfg.source_lang.c_str(), cfg.target_lang.c_str() };

		fmt{ fmt_def, fc_none, "lswap run --log\n" };
		fmt{ fmt_def, fc_cyan, "      To do with log trace\n\n" };

		fmt{ fmt_def, fc_none, "Stay tuned for updates https://github.com/xastrix/lswap\n" };
		fmt{ fmt_def, fc_none, "Copyright (c) 2024 xastrix" };
	} };

	cli.add("run", [](int ac, args_t args) {
		g::cfg = g_config.init();
		g_hooks.init();

		if (ac == 1 && args[1] == "--log")
			g::m_log = true;

		fmt{ fmt_def, fc_none, "lswap...\n" };
		fmt{ fmt_30ms, fc_yellow, "Hint: Highlight your text (CTRL + C) and paste (CTRL + V)" };
		fmt{ fmt_30ms, fc_yellow, "Hint2: You can close the program by pressing CTRL + X\n" };

		if (g::m_log)
			fmt{ fmt_def, fc_yellow, "Warning: Running in logging mode\n\n" };

		cfg_t cfg = g_config.init();

		fmt{ fmt_def, fc_cyan, "  %s > %s\n\n", cfg.source_lang.c_str(), cfg.target_lang.c_str() };

		fmt{ fmt_def, fc_none, "Stay tuned for updates https://github.com/xastrix/lswap\n" };

		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		g_hooks.free();
	});

	return cli.parse(argc, argv);
}