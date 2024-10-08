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
	cli(std::function<void()> fn) : _fn(fn) {};

	/*
	* Handle parse arguments
	* @param count of arguments
	* @param arguments
	* @return 0
	*/
	int parse(int argc, const char** argv);

	/*
	* Add argument to the map
	* @param cmd (std::string)
	* @param func
	*/
	void add(const std::string& cmd, std::function<void(int, arguments_t)> function);
private:
	std::function<void()> _fn;
	arguments_t _arguments;
	std::map<std::string, std::function<void(int, arguments_t)>> _commands;
};