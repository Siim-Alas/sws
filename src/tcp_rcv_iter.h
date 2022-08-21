
#pragma once

#include "config.h"
#include "tcp_connection.h"

namespace sws {
	class tcp_rcv_iter {
	public:
		tcp_rcv_iter(tcp_connection &connection);
		~tcp_rcv_iter();

		void operator++();
		char operator*();

		char next();

		bool nothing_received();

	private:
		tcp_connection &c;
		int bytes_rcvd, i;
		char *rcv_buf;
	};
}

