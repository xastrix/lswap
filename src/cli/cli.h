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
	cli(std::function<void()> fn) : _fn(fn) {};
	int parse(int argc, const char** argv);
	void add(const std::string& cmd, std::function<void(int, args_t)> function);
private:
	std::function<void()> _fn;
	args_t _arguments;
	std::map<std::string, std::function<void(int, args_t)>> _commands;
};