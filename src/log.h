
#pragma once

#ifdef LOG_TO_STDOUT

#include <arpa/inet.h>

#define LOG_ADDR(sockaddr_ptr) { \
	char s[INET6_ADDRSTRLEN]; \
	void *addr = ((sockaddr_ptr)->sa_family == AF_INET) ? \
		(void*)&(((struct sockaddr_in*)(sockaddr_ptr))->sin_addr) : \
		(void*)&(((struct sockaddr_in6*)(sockaddr_ptr))->sin6_addr); \
	inet_ntop((sockaddr_ptr)->sa_family, addr, s, sizeof(s)); \
	printf("%s", s); \
}

#define LOG_PRINTF(x) printf(x)

#else

#define LOG_ADDR(sockaddr_ptr)
#define LOG_PRINTF(x)

#endif

