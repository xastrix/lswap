#include "fmt.h"

#include <cstdarg>
#include <thread>
#include <chrono>

#define MSG_BUFFER_SIZE 4096

fmt::fmt(fmt_mode mode, fmt_col col, const char* fmt, ...)
{
	char buffer[MSG_BUFFER_SIZE];

	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, args);
	va_end(args);

	if (col != fc_none) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), col);
	}

	if (mode != fmt_def) {
		char* word = strtok(buffer, " ");
		while (word != NULL) {
			printf("%s ", word);
			switch (mode) {
			case fmt_30ms: {
				std::this_thread::sleep_for(std::chrono::milliseconds(30));
				break;
			}
			}
			word = strtok(NULL, " ");
		}
		printf("\n");
	}
	else {
		printf("%s", buffer);
	}

	if (col != fc_none) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), fc_reset);
	}
}