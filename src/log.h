
#pragma once

#include <arpa/inet.h>
#include <cstdio>

#include "config.h"

namespace sws {
	void log_addr(sockaddr *sa);
	void log_print(const char *s);
}

