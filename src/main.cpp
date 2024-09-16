#include "cli/cli.h"
#include "hooks/hooks.h"

int main(int argc, const char** argv)
{
	utils::set_locale();
	g::cfg = config::init();

	cli cli{ [&]() {
		fmt{ fmt_30ms, fc_none, "lswap is an fast translator of your text from the clipboard" };
		fmt{ fmt_30ms, fc_none, "lswap version %s\n", LSWAP_VERSION_STRING };

		fmt{ fmt_def, fc_none, "target language - %s\n", g::cfg.target_lang.c_str() };
		fmt{ fmt_def, fc_none, "source language - %s\n\n", g::cfg.source_lang.c_str() };

		fmt{ fmt_def, fc_none, "lswap run --log\n" };
		fmt{ fmt_def, fc_cyan, "      With logging (Displaying nontranslated and translated texts)\n\n" };

		fmt{ fmt_def, fc_none, "lswap run --hidden\n" };
		fmt{ fmt_def, fc_cyan, "      With hidden console window (But you can only terminate the program by closing the process)\n\n" };

		fmt{ fmt_def, fc_none, "lswap config <source_lang> <target_lang>\n" };
		fmt{ fmt_def, fc_cyan, "      Change the source and target languages in the configuration file\n\n\n" };

		fmt{ fmt_def, fc_none, "Stay tuned for updates\n" };
		fmt{ fmt_def, fc_cyan, "      https://github.com/xastrix/lswap" };
	} };

	cli.add("run", [&](int ac, args_t args) {
		if (ac == 1 && args[1] == "--log")
			g::m_log = true;

		else if (ac == 1 && args[1] == "--hidden")
			ShowWindow(GetConsoleWindow(), SW_HIDE);

		fmt{ fmt_30ms, fc_yellow, "Hint: Highlight your text (CTRL + C) and paste (CTRL + V)" };
		fmt{ fmt_30ms, fc_yellow, "Hint2: To close the program focus on window and press CTRL + C\n" };

		if (g::m_log)
			fmt{ fmt_def, fc_yellow, "Warning: Running in logging mode\n\n" };

		fmt{ fmt_def, fc_cyan, "  %s > %s\n\n", g::cfg.source_lang.c_str(), g::cfg.target_lang.c_str() };

		hooks::init();

		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		hooks::free();
	});

	cli.add("config", [&](int ac, args_t args) {
		switch (ac) {
		case 0: {
			fmt{ fmt_30ms, fc_none, "Source & Target languages is empty" };
			break;
		}
		case 1: {
			fmt{ fmt_30ms, fc_none, "Target language is empty" };
			break;
		}
		case 2: {
			if (config::change_cfg_values(args[1], args[2])) {
				fmt{ fmt_30ms, fc_none, "Parameters in the configuration file is updated!" };

				g::cfg = config::init();
				fmt{ fmt_def, fc_none, "Updated: %s > %s\n", g::cfg.source_lang.c_str(), g::cfg.target_lang.c_str() };
			}
			break;
		}
		}
	});

	return cli.parse(argc, argv);
}