#include "cli.h"

#include <algorithm>

int cli::parse(int argc, const char** argv)
{
	if (argc < 2) {
		std::cout << _msg;
		return 0;
	}

	_arguments.assign(argv + 1, argv + argc);

	std::string cmd = _arguments.front();
	std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

	auto it = _commands.find(cmd);
	if (it != _commands.end()) {
		it->second(argc - 2, _arguments);
	}
	else {
		std::cout << _msg;
	}

	return 0;
}

void cli::add(const std::string& cmd, std::function<void(int, arguments_t)> function)
{
	size_t pos = 0, prev_pos = 0;

	while ((pos = cmd.find("/", prev_pos)) != std::string::npos) {
		_commands[cmd.substr(prev_pos, pos - prev_pos)] = function;
		prev_pos = pos + 1;
	}

	_commands[cmd.substr(prev_pos)] = function;
}