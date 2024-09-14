#pragma once

#include <windows.h>

typedef enum {
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
} fmt_col;

typedef enum {
	// Default printing
	fmt_def,
	// Printing every word with 30ms delay
	fmt_30ms,
	// Printing every word with 50ms delay
	fmt_50ms,
	// Printing every word with 100ms delay
	fmt_100ms,
} fmt_mode;

struct fmt {
	fmt(fmt_mode mode, fmt_col col, const char* fmt, ...);
};

struct wfmt {
	wfmt(fmt_mode mode, fmt_col col, const wchar_t* fmt, ...);
};