#pragma once

#include "../globals.h"
#include "../fmt/fmt.h"

#include "../utils/utils.h"

namespace hooks
{
	/**
	 * Initialize hooks
	 * 
	*/
	void init();

	/**
	 * Uninitialize hooks
	 * 
	*/
	void free();
}