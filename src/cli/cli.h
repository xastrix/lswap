#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <map>
#include <cctype>

using arguments_t = std::vector<std::string>;

class cli {
public:
	cli(const std::string& msg) : _msg(msg) {};
	int parse(int argc, const char** argv);
	void add(const std::string& cmd, std::function<void(int, arguments_t)> function);
private:
	std::string _msg;
	arguments_t _arguments;
	std::map<std::string, std::function<void(int, arguments_t)>> _commands;
};