
#pragma once

#include "tcp_listener.h"

namespace sws {
	class server {
	public:
		server(char const *service);

		void start_http();

	private:
		tcp_listener listener;
	};
}

