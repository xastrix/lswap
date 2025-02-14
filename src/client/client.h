#pragma once

#include "../common.h"
#include "../cli/cli.h"
#include "../fmt/fmt.h"
#include "../utils/utils.h"

#include <curl/curl.h>

enum clipboard_state {
	cb_idle,
	cb_processing,
};

namespace client
{
	void init();
	void free();
}