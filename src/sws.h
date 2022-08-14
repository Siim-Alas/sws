
#pragma once

#include <cstring>
#include <netdb.h>
#include <stdexcept>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "config.h"
#include "log.h"

namespace sws {
	typedef void (*recv_callback)(sockaddr*, int);

	class server {
	public:
		server(char const *service);
		~server();

		void start_http();

	private:
		int socketfd;
		char *buf;

		void bind_to_first_possible(addrinfo *suitable_addresses);
	};

	addrinfo* get_suitable_addresses(const char *service);
}

