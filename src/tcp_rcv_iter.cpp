
#include "tcp_rcv_iter.h"

sws::tcp_rcv_iter::tcp_rcv_iter(tcp_connection &connection) :
	c(connection), bytes_rcvd(0), i(0) {
	rcv_buf = new char[config::RCVBUF_SIZE];
}

sws::tcp_rcv_iter::~tcp_rcv_iter() {
	delete[] rcv_buf;
}

void sws::tcp_rcv_iter::operator++() {
	i++;

	if (i >= bytes_rcvd) {
		bytes_rcvd = c.rcv(rcv_buf, config::RCVBUF_SIZE);
		i = 0;
	}
}

char sws::tcp_rcv_iter::operator*() {
	return rcv_buf[i];
}

char sws::tcp_rcv_iter::next() {
	++(*this);
	return *(*this);
}

bool sws::tcp_rcv_iter::nothing_received() {
	return bytes_rcvd <= 0;
}

