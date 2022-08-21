
#include "http.h"

void sws::http::send_400(tcp_connection &c) {
	const char response[] = "HTTP/1.1 400 Bad Request\r\n\r\n";
	int bytes_sent = c.snd(response, sizeof(response));

	while (bytes_sent < sizeof(response)) {
		bytes_sent += c.snd(response + bytes_sent, sizeof(response) - bytes_sent);
	}
}

void sws::http::send_404(tcp_connection &c) {
	const char response[] = "HTTP/1.1 404 Not Found\r\n\r\n";
	int bytes_sent = c.snd(response, sizeof(response));

	while (bytes_sent < sizeof(response)) {
		bytes_sent += c.snd(response + bytes_sent, sizeof(response) - bytes_sent);
	}
}

void sws::http::send_414(tcp_connection &c) {
	const char response[] = "HTTP/1.1 414 URI Too Long\r\n\r\n";
	int bytes_sent = c.snd(response, sizeof(response));

	while (bytes_sent < sizeof(response)) {
		bytes_sent += c.snd(response + bytes_sent, sizeof(response) - bytes_sent);
	}
}

void sws::http::send_500(tcp_connection &c) {
	const char response[] = "HTTP/1.1 500 Internal Server Error\r\n\r\n";
	int bytes_sent = c.snd(response, sizeof(response));

	while (bytes_sent < sizeof(response)) {
		bytes_sent += c.snd(response + bytes_sent, sizeof(response) - bytes_sent);
	}
}

void sws::http::send_501(tcp_connection &c) {
	const char response[] = "HTTP/1.1 501 Not Implemented\r\n\r\n";
	int bytes_sent = c.snd(response, sizeof(response));

	while (bytes_sent < sizeof(response)) {
		bytes_sent += c.snd(response + bytes_sent, sizeof(response) - bytes_sent);
	}
}

