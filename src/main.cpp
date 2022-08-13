
#include "main.h"

char *buf;

void recv_cb(struct sockaddr *client_address, int bytes_received) {
	printf("Received data!\n");
	fwrite(buf, 1, bytes_received, stdout);
}

int main(int argc, char **argv) {
	if (argc != 2) {
		puts("Usage: sws <port>");
		return 1;
	}

	sws::server s(argv[1]);

	int buflen = 1024;
	buf = new char[buflen];

	s.receive(buf, buflen, recv_cb);

	delete[] buf;

	return 0;
}

