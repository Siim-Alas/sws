
#pragma once

namespace sws::config {
	const int BACKLOG = 10;
	const bool LOG_TO_STDOUT = true;
	const int MAX_URI_LEN = 1024; // Must be less than or equal to SNDBUF_SIZE
	const int RCVBUF_SIZE = 512;
	const int SNDBUF_SIZE = 4096;
}

