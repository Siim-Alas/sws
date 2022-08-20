
#pragma once

#include <cstring>
#include <netdb.h>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>

#include "log.h"
#include "tcp_connection.h"

namespace sws {
	class tcp_listener {
	public:
		typedef void (*recv_callback)(sockaddr&, tcp_connection&);

		tcp_listener(const char *service);
		~tcp_listener();

		void start_listening(recv_callback on_receive);

	private:
		int socketfd;

		addrinfo* get_suitable_addresses(const char *service);
		void bind_to_first_possible(addrinfo *suitable_addresses);
	};
}

