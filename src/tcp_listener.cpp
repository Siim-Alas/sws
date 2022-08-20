
#include "tcp_listener.h"

sws::tcp_listener::tcp_listener(const char *service) {
	addrinfo *suitable_addresses = get_suitable_addresses(service);
	if (suitable_addresses == NULL) {
		throw std::runtime_error("Failed to get a suitable address");
	}

	bind_to_first_possible(suitable_addresses);

	freeaddrinfo(suitable_addresses);

	if (socketfd == -1) {
		throw std::runtime_error("Failed to bind to a socket");
	}
}

sws::tcp_listener::~tcp_listener() {
	close(socketfd);
}

void sws::tcp_listener::start_listening(recv_callback on_receive) {
	if (listen(socketfd, config::BACKLOG) == -1) {
		throw std::runtime_error("Failed to listen");
	}

	sockaddr client_addr;
	socklen_t addrlen = sizeof(client_addr);

	log_print("[*] Listening for incoming requests\n");

	while (true) {
		int recv_sockfd = accept(socketfd, &client_addr, &addrlen);
		if (recv_sockfd == -1) {
			throw std::runtime_error("Failed to accept connection");
		}

		tcp_connection c(recv_sockfd);
		on_receive(client_addr, c);
	}
}

void sws::tcp_listener::bind_to_first_possible(addrinfo *suitable_addresses) {
	socketfd = -1;
	for (addrinfo *ai = suitable_addresses; ai != NULL; ai = ai->ai_next) {
		socketfd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
		if (socketfd == -1) {
			continue;
		}

		if (bind(socketfd, ai->ai_addr, ai->ai_addrlen) == -1) {
			close(socketfd);
			socketfd = -1;
			continue;
		}

		log_print("[*] Successfully bound to ");
		log_addr(ai->ai_addr);
		log_print("\n");

		break;
	}
}

addrinfo* sws::tcp_listener::get_suitable_addresses(const char *service) {
	struct addrinfo hints, *res;
	memset(&hints, 0, sizeof(hints)); // Zero all fields

	hints.ai_family = AF_UNSPEC; // Accept both IPv4 and IPv6
	hints.ai_socktype = SOCK_STREAM; // Request a TCP stream socket
	hints.ai_flags = AI_PASSIVE; // Automatically set the local IP address

	if (getaddrinfo(NULL, service, &hints, &res) != 0) {
		res = NULL;
	}

	return res;
}

