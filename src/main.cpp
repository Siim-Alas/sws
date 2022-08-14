
#include "main.h"

int main(int argc, char **argv) {
	if (argc != 2) {
		puts("Usage: sws <port>");
		return 1;
	}

	sws::server s(argv[1]);
	s.start_http();

	return 0;
}

