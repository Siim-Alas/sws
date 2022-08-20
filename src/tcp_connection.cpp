
#include "tcp_connection.h"

sws::tcp_connection::tcp_connection(int connection_socketfd) :
	socketfd(connection_socketfd) {
}

sws::tcp_connection::~tcp_connection() {
	close(socketfd);
}

int sws::tcp_connection::snd(const void *msg, int msg_len) {
	return send(socketfd, msg, msg_len, 0);
}

int sws::tcp_connection::rcv(void *buf, int buf_len) {
	return recv(socketfd, buf, buf_len, 0);
}
