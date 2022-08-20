
#include "sws.h"

sws::server::server(char const *service) : listener(service) {
}

void sws::server::start_http() {
	auto recv_callback = [](sockaddr &client_addr, tcp_connection &c) {
		log_print("[*] Receiving from ");
		log_addr(&client_addr);
		log_print(":\n");

		char *buf = new char[config::BUFSIZE];

		int bytes_received = 0, pos = 0;

		do {
			bytes_received = c.rcv(buf + pos, config::BUFSIZE - pos);

			const char *response = "Something in return\n";
			c.snd(response, strlen(response));

			if constexpr (config::LOG_TO_STDOUT) {
				fwrite(buf + pos, 1, bytes_received, stdout);
			}

			pos = (pos + bytes_received) % config::BUFSIZE;
		} while (bytes_received > 0);

		log_print("[*] Connection terminated");

		delete[] buf;
	};

	listener.start_listening(recv_callback);
}


