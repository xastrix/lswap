#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <map>
#include <cctype>

using args_t = std::vector<std::string>;

class cli {
public:
	cli(const std::string& msg) : _msg(msg) {};
	int parse(int argc, const char** argv);
	void add(const std::string& cmd, std::function<void(int, args_t)> function);
	void call(const std::string& cmd, int argc, args_t args);
private:
	std::string _msg;
	args_t _arguments;
	std::map<std::string, std::function<void(int, args_t)>> _commands;
};