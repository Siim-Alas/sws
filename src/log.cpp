
#include "log.h"

void sws::log_addr(struct sockaddr *sa) {
	if constexpr (config::LOG_TO_STDOUT) {
		char s[INET6_ADDRSTRLEN];
		void *addr = (sa->sa_family == AF_INET) ?
			(void*)&(((struct sockaddr_in*)sa)->sin_addr) :
			(void*)&(((struct sockaddr_in6*)sa)->sin6_addr);
		inet_ntop(sa->sa_family, addr, s, sizeof(s));
		fputs(s, stdout);
	}
}

void sws::log_print(const char *s) {
	if constexpr (config::LOG_TO_STDOUT) {
		fputs(s, stdout);
	}
}

