#pragma once

struct hooks {
	void init();
	void free();
};

extern hooks g_hooks;