
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
	typedef void (*recv_callback)(int);

	class server {
	public:
		server(char const *service);
		~server();

		void receive(void *buf, int len, recv_callback cb);

	private:
		int socketfd;

		void bind_to_first_possible(struct addrinfo *suitable_addresses);
	};

	addrinfo* get_suitable_addresses(const char *service);
}

