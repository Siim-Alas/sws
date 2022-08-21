
#pragma once

#include "http.h"
#include "tcp_listener.h"
#include "tcp_rcv_iter.h"

namespace sws {
	class server {
	public:
		server(char const *service);
		~server();

		void start_http();

	private:
		tcp_listener listener;
		char *snd_buf;
	};
}

