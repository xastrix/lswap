#include "cli.h"

int cli::parse(int argc, const char** argv)
{
	for (int i{ 1 }; i < argc; ++i) {
		_arguments.push_back(argv[i]);
	}

	if (_arguments.empty()) {
		_fn();
		return 0;
	}

	std::string cmd = _arguments[0];

	if (cmd.empty()) {
		_fn();
		return 0;
	}

	for (char& c : cmd) {
		c = std::tolower(static_cast<unsigned char>(c));
	}

	if (_commands.find(cmd) != _commands.end()) {
		_commands[cmd](argc - 2, _arguments);
	}
	else {
		_fn();
		return 0;
	}
}

void cli::add(const std::string& cmd, std::function<void(int, args_t)> function)
{
	size_t pos = 0, prev_pos = 0;

	while ((pos = cmd.find("/", prev_pos)) != std::string::npos) {
		_commands[cmd.substr(prev_pos, pos - prev_pos)] = function;
		prev_pos = pos + 1;
	}

	_commands[cmd.substr(prev_pos)] = function;
}