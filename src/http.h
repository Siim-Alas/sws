
#pragma once

#include "tcp_connection.h"

namespace sws::http {
	const int SHORTEST_REQ_LEN = sizeof("GET / HTTP/1.1\r\n");

	void send_400(tcp_connection &c);
	void send_404(tcp_connection &c);
	void send_414(tcp_connection &c);
	void send_500(tcp_connection &c);
	void send_501(tcp_connection &c);
}

