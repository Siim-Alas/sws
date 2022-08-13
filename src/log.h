
#pragma once

#ifdef LOG_TO_STDOUT

#include <arpa/inet.h>

#define LOG_ADDR(addr_info_ptr) { \
	char s[INET6_ADDRSTRLEN]; \
	void *addr = (addr_info_ptr->ai_family == AF_INET) ? \
		(void*)&(((struct sockaddr_in*)addr_info_ptr->ai_addr)->sin_addr) : \
		(void*)&(((struct sockaddr_in6*)addr_info_ptr->ai_addr)->sin6_addr); \
	inet_ntop(addr_info_ptr->ai_family, addr, s, sizeof(s)); \
	printf("%s", s); \
}

#define LOG_PRINTF(x) printf(x)

#else

#define LOG_ADDR(addr_info_ptr)
#define LOG_PRINTF(x)

#endif

