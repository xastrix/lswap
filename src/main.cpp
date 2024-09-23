#include "cli/cli.h"
#include "hooks/hooks.h"

int main(int argc, const char** argv)
{
	utils::set_locale();
	g::cfg = config::init();

	cli cli{ []() {
		fmt{ fmt_30ms, fc_none, "%s is a command line tool designed for swift translation between languages by simply copying and pasting data from the clipboard", LSWAP_APPLICATION_NAME };
		fmt{ fmt_30ms, fc_none, "%s version %s\n", LSWAP_APPLICATION_NAME, LSWAP_VERSION_STRING };

		fmt{ fmt_def, fc_cyan, "  %s > %s\n\n", g::cfg.source_lang.c_str(), g::cfg.target_lang.c_str() };

		fmt{ fmt_def, fc_none, "%s run --log\n", LSWAP_APPLICATION_NAME };
		fmt{ fmt_def, fc_cyan, "      With logging (Displaying nontranslated and translated texts)\n\n" };

		fmt{ fmt_def, fc_none, "%s config <source_lang> <target_lang>\n", LSWAP_APPLICATION_NAME };
		fmt{ fmt_def, fc_cyan, "      Change the source and target languages in the configuration file\n\n" };
	} };

	cli.add("run", [](int ac, args_t args) {
		if (ac == 1 && args[1] == "--log")
			g::m_log = true;

		hooks::init();

		fmt{ fmt_def, fc_none, "\n" };
		fmt{ fmt_def, fc_cyan, "  %s > %s\n\n", g::cfg.source_lang.c_str(), g::cfg.target_lang.c_str() };

		fmt{ fmt_30ms, fc_yellow, "Hint: Highlight your text (CTRL + C) and paste (CTRL + V)" };
		fmt{ fmt_30ms, fc_yellow, "Hint2: To close the program focus on window and press CTRL + C" };
		fmt{ fmt_30ms, fc_yellow, "Hint3: You can hide the console window by pressing CTRL + X\n" };

		if (g::m_log)
			fmt{ fmt_def, fc_yellow, "Warning: Running in logging mode\n" };

		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		hooks::free();
	});

	cli.add("config", [](int ac, args_t args) {
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

				g::cfg = config::init(); // Reload configuration
				fmt{ fmt_def, fc_none, "Updated: %s > %s\n", g::cfg.source_lang.c_str(), g::cfg.target_lang.c_str() };
			}
			break;
		}
		}
	});

	return cli.parse(argc, argv);
}