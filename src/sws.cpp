
#include "sws.h"

sws::server::server(const char *service) {
	struct addrinfo *suitable_addresses = sws::get_suitable_addresses(service);
	if (suitable_addresses == NULL) {
		throw std::runtime_error("Failed to get a suitable address");
	}

	bind_to_first_possible(suitable_addresses);

	freeaddrinfo(suitable_addresses);

	if (socketfd == -1) {
		throw std::runtime_error("Failed to bind to a socket");
	}
}

sws::server::~server() {
	close(socketfd);
}

void sws::server::receive(void *buf, int len, recv_callback cb) {
	if (listen(socketfd, BACKLOG) == -1) {
		throw std::runtime_error("Failed to listen");
	}

	// TODO: Handle child processes

	struct sockaddr client_addr;
	socklen_t addrlen = sizeof(client_addr);

	LOG_PRINTF("Listening for incoming requests...\n");

	while (true) {
		int recv_sockfd = accept(socketfd, &client_addr, &addrlen);
		if (recv_sockfd == -1) {
			throw std::runtime_error("Failed to accept connection");
		}

		int bytes_recvd = recv(recv_sockfd, buf, len, 0);
		if (bytes_recvd == -1) {
			throw std::runtime_error("Error receiving bytes");
		}

		cb(bytes_recvd);
	}
}

void sws::server::bind_to_first_possible(struct addrinfo *suitable_addresses) {
	socketfd = -1;
	for (struct addrinfo *ai = suitable_addresses; ai != NULL; ai = ai->ai_next) {
		socketfd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
		if (socketfd == -1) {
			continue;
		}

		if (bind(socketfd, ai->ai_addr, ai->ai_addrlen) == -1) {
			close(socketfd);
			socketfd = -1;
			continue;
		}

		LOG_PRINTF("Binding to ");
		LOG_ADDR(ai);
		LOG_PRINTF("...\n");

		break;
	}
}

addrinfo* sws::get_suitable_addresses(const char *service) {
	struct addrinfo hints, *res;
	memset(&hints, 0, sizeof(hints)); // Zefo all fields

	hints.ai_family = AF_UNSPEC; // Accept both IPv4 and IPv6
	hints.ai_socktype = SOCK_STREAM; // Request a TCP stream socket
	hints.ai_flags = AI_PASSIVE; // Automatically set the local IP address

	if (getaddrinfo(NULL, service, &hints, &res) != 0) {
		res = NULL;
	}

	return res;
}

