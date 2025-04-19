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

enum resp_codes : long {
	resp_ok_code = 200,
	bad_request_code = 400,
	unauthorized_code = 401,
	forbidden_code = 403,
	not_found_code = 404,
	internal_server_err_code = 500,
	bad_gateway_code = 502,
	service_unavailable_code = 503,
};

namespace client
{
	void init();
	void free();
}