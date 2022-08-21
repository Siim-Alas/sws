
#pragma once

#include <cstring>
#include <functional>
#include <netdb.h>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>

#include "log.h"
#include "tcp_connection.h"

namespace sws {
	class tcp_listener {
	public:
		tcp_listener(const char *service);
		~tcp_listener();

		void start_listening(std::function<void(sockaddr&, tcp_connection&)> on_receive);

	private:
		int socketfd;

		addrinfo* get_suitable_addresses(const char *service);
		void bind_to_first_possible(addrinfo *suitable_addresses);
	};
}

