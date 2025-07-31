#include "client.h"

int main(int argc, const char** argv)
{
	cli cli {
		LSWAP_APPLICATION_NAME " (" LSWAP_VERSION_STRING ") is a command line tool designed for swift translation between languages by simply copying and pasting data from the clipboard\n\n"

		LSWAP_APPLICATION_NAME " run\n"
		"  Starting the application\n\n"

		LSWAP_APPLICATION_NAME " c/config <SourceLanguage> <TargetLanguage>\n"
		"  Change the source and target languages in the configuration file\n\n"

		LSWAP_APPLICATION_NAME " ar/autorun <--on/--off>\n"
		"  Adding a program to startup\n"
	};

	client::globals::g_cfg = config::init();

	cli.add(_cmd[cl_run], [](int, args_t) {
		return client::init();
	});

	cli.add(_cmd[cl_config], [](int ac, args_t args) {
		if (ac != 2) {
			fmt{ fmt_def, fc_none, "Current configuration: %s > %s\n", client::globals::g_cfg.source_lang.c_str(), client::globals::g_cfg.target_lang.c_str() };
			fmt{ fmt_def, fc_none, "For change it, type \"%s c/config <SourceLanguage> <TargetLanguage>\"\n", LSWAP_APPLICATION_NAME };
			return;
		}

		if (!config::change_cfg_values(args[1], args[2]))
			return;

		client::globals::g_cfg = config::init(); // Reload configuration

		fmt{ fmt_30ms, fc_green, "The configuration file has been updated!" };
		fmt{ fmt_def, fc_none, "Updated configuration: %s > %s\n", client::globals::g_cfg.source_lang.c_str(), client::globals::g_cfg.target_lang.c_str() };
	});

	cli.add(_cmd[cl_autorun], [](int ac, args_t args) {
		if (ac != 1 || (args[1] != "--on" && args[1] != "--off")) {
			fmt{ fmt_30ms, fc_none, "Invalid Arguments. Usage: %s ar/autorun --on/--off", LSWAP_APPLICATION_NAME };
			return;
		}

		utils::put_in_autorun(LSWAP_APPLICATION_NAME, args[1] == "--on" ? true : false);

		fmt{ fmt_30ms, fc_green, "%s successfully %s autorun", LSWAP_APPLICATION_NAME,
			args[1] == "--on" ? "added to" : "removed from"};
	});

	if (argc == 1 && utils::is_in_autorun(LSWAP_APPLICATION_NAME))
		cli.call(_cmd[cl_run], 0, {});

	return cli.parse(argc, argv);
}

void client::init()
{
	init_mutex();
	init_class();

	fmt{ fmt_def, fc_none, "The service has been launched...\n" };
	fmt{ fmt_def, fc_none, "Current configuration: %s > %s\n", globals::g_cfg.source_lang.c_str(), globals::g_cfg.target_lang.c_str() };

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