
#include "sws.h"

sws::server::server(const char *service) {
	addrinfo *suitable_addresses = sws::get_suitable_addresses(service);
	if (suitable_addresses == NULL) {
		throw std::runtime_error("Failed to get a suitable address");
	}

	bind_to_first_possible(suitable_addresses);

	freeaddrinfo(suitable_addresses);

	if (socketfd == -1) {
		throw std::runtime_error("Failed to bind to a socket");
	}

	buf = new char[config::BUFSIZE];
}

sws::server::~server() {
	delete[] buf;
	close(socketfd);
}

void sws::server::start_http() {
	if (listen(socketfd, config::BACKLOG) == -1) {
		throw std::runtime_error("Failed to listen");
	}

	// TODO: Handle child processes

	sockaddr client_addr;
	socklen_t addrlen = sizeof(client_addr);

	log_print("[*] Listening for incoming requests\n");

	while (true) {
		int recv_sockfd = accept(socketfd, &client_addr, &addrlen);
		if (recv_sockfd == -1) {
			throw std::runtime_error("Failed to accept connection");
		}

		log_print("[*] Receiving a TCP connection from ");
		log_addr(&client_addr);
		log_print(":\n");

		int pos = 0;
		int bytes_recvd = 0;

		do {
			bytes_recvd = recv(recv_sockfd, buf + pos, config::BUFSIZE - pos, 0);

			if constexpr (config::LOG_TO_STDOUT) {
				fwrite(buf + pos, 1, bytes_recvd, stdout);
			}

			pos = (pos + bytes_recvd) % config::BUFSIZE;
		} while (bytes_recvd > 0);

		log_print("[*] Connection terminated\n");
	}
}

void sws::server::bind_to_first_possible(addrinfo *suitable_addresses) {
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

addrinfo* sws::get_suitable_addresses(const char *service) {
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

