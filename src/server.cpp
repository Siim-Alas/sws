
#include "server.h"

sws::server::server(char const *service) : listener(service) {
	snd_buf = new char[config::SNDBUF_SIZE];
}

sws::server::~server() {
	delete[] snd_buf;
}

char char_to_hex(char c) {
	// All ASCII chars have values less than 128, meaning that their most
	// significant bit should be set to zero
	c &= 0b01111111;

	// Digits have smaller ASCII values than letters. The char 'A' has the
	// smallest ASCII value at 0x41 = 65, whereas 'z' has the highest at 0x7A =
	// = 122. Note that since c < 128 < 2 * 65 = 130, this is guaranteed to be 0
	// if c is less than 0x41 and 1 otherwise.
	int not_a_digit = c / 0x41;

	// '0' has an ASCII value of 0x30
	int digit_idx = c - 0x30;

	// 'A' has an ASCII value of 0x41, whereas 'a' has a value of 0x61. To
	// account for both upper and lower case letters, we first subtract 0x41 and
	// then take the modulus with respect to 0x61 - 0x41 = 0x20
	int letter_idx = (c - 0x41) % (0x61 - 0x41);

	int result = (1 - not_a_digit) * digit_idx +
		not_a_digit * (letter_idx + 10);

	// Note that if c is not a digit or letter, then the result could have some
	// bogus value, but casting to char guarantees that it is at least int the
	// range of 0-255
	return (char)result;
}

char hex_decode(char c1, char c2) {
	return 16 * char_to_hex(c1) + char_to_hex(c2);
}

int read_path(char *path_buf, sws::tcp_rcv_iter &ri) {
	int path_len = 0, last_slash = 0;
	char current;
	while ((current = ri.next()) != ' ') {
		if (current == '%') {
			char c1 = ri.next(), c2 = ri.next();
			current = hex_decode(c1, c2);
		}

		if (current == '/') {
			if (current < last_slash + 2) {
				// Two subsequent slashes are not allowed
				return -1;
			}

			if (current == last_slash + 3 &&
					path_buf[last_slash + 1] == '.' &&
					path_buf[last_slash + 2] == '.') {
				// Ignore the rule about "/../" going a directory back
				path_len = last_slash;
				continue;
			}

			last_slash = path_len;
		}

		path_buf[path_len] = current;
		path_len++;
	}

	return path_len;
}


void sws::server::start_http() {
	auto recv_callback = [this](sockaddr &client_addr, tcp_connection &c) {
		log_print("[*] Receiving from ");
		log_addr(&client_addr);
		log_print(":\n");
		
		tcp_rcv_iter ri(c);
		
		while (true) {
			char c0 = ri.next(), c1 = ri.next(), c2 = ri.next();
			
			bool get;
		
			if (c0 == 'G' && c1 == 'E' && c2 == 'T') {
				get = true;
			} else if (c0 == 'H' && c1 == 'E' && c2 == 'A' && ri.next() == 'D') {
				get = false;
			} else {
				http::send_501(c);
				break;
			}
		
			if (ri.next() != ' ') {
				http::send_400(c);
				break;
			}
		
			if (ri.next() != '/') {
				http::send_400(c);
				break;
			}
		
			// Note that by omitting the first '/', path_len becomes one less than
			// the uri length. However, inserting the '\0' at the end cancels this
			int path_len = read_path(snd_buf, ri);
			if (path_len == -1) {
				http::send_400(c);
				break;
			}
			if (path_len > config::MAX_URI_LEN) {
				http::send_414(c);
				break;
			}
		
			if (path_len == 0 || snd_buf[path_len - 1] == '/') {
				const char fn[] = "index.html";
		
				if (path_len + sizeof(fn) + 1 > config::MAX_URI_LEN) {
					http::send_414(c);
					break;
				}
		
				memcpy(snd_buf + path_len, fn, sizeof(fn));
				path_len += sizeof(fn);
			}
		
			snd_buf[path_len] = '\0';
		
			// TODO: Read the rest of the request and handle multiple requests in
			// one connection
		
			FILE *requested_file = fopen(snd_buf, "rb");
		
			if (requested_file == NULL) {
				http::send_404(c);
				break;
			}
		
			if (fseek(requested_file, 0, SEEK_END)) {
				http::send_500(c);
				fclose(requested_file);
				break;
			}
		
			int file_size = ftell(requested_file);
			if (file_size == -1) {
				http::send_500(c);
				fclose(requested_file);
				break;
			}
		
			if (fseek(requested_file, 0, SEEK_SET)) {
				http::send_500(c);
				fclose(requested_file);
				break;
			}
		
			int pos = snprintf(
					snd_buf,
					config::SNDBUF_SIZE,
					"HTTP/1.1 200 OK\r\nContent-Length: %d\r\nConnection: close\r\n\r\n",
					file_size);
		
			int total_bytes_read = fread(
					snd_buf + pos, 1, config::SNDBUF_SIZE - pos, requested_file);
			pos += total_bytes_read;

		
			int snd_pos = 0;

			do {
				int bytes_sent = c.snd(snd_buf + snd_pos, pos - snd_pos);
				snd_pos += bytes_sent;
			} while (snd_pos < pos);
		
			while (total_bytes_read < file_size) {
				int bytes_read = fread(snd_buf, 1, config::SNDBUF_SIZE, requested_file);
		
				snd_pos = 0;
				do {
					int bytes_sent = c.snd(snd_buf + snd_pos, bytes_read - snd_pos);
					snd_pos += bytes_sent;
				} while (snd_pos < bytes_read);
		
				total_bytes_read += bytes_read;
			}
		
			fclose(requested_file);
		
			break;
		}
		
		log_print("[*] Connection terminated\n");
	};

	listener.start_listening(recv_callback);
}


