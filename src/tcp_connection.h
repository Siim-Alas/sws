
#pragma once

#include <sys/socket.h>
#include <unistd.h>

namespace sws {
	class tcp_connection {
	public:
		tcp_connection(int connection_socketfd);
		~tcp_connection();

		int snd(const void *msg, int msg_len);
		int rcv(void *buf, int buf_len);

	private:
		int socketfd;
	};
}
