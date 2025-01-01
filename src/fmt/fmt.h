#pragma once

#include <windows.h>

enum fmt_col {
	fc_none = 0,
	fc_red = FOREGROUND_RED,
	fc_green = FOREGROUND_GREEN,
	fc_yellow = FOREGROUND_RED | FOREGROUND_GREEN,
	fc_blue = FOREGROUND_BLUE,
	fc_magenta = FOREGROUND_RED | FOREGROUND_BLUE,
	fc_cyan = FOREGROUND_GREEN | FOREGROUND_BLUE,
	fc_white = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
	fc_gray = 0 | FOREGROUND_INTENSITY,
	fc_reset = fc_white,
};

enum fmt_mode {
	fmt_def,
	fmt_30ms,
	fmt_50ms,
	fmt_100ms,
};

struct fmt {
	fmt(fmt_mode mode, fmt_col col, const char* fmt, ...);
	void die() { exit(EXIT_FAILURE); }
};