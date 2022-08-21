
cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)

project(sws)

add_executable(sws src/config.h src/http.cpp src/http.h src/log.cpp src/log.h src/main.cpp src/main.h src/server.cpp src/server.h src/tcp_connection.cpp src/tcp_connection.h src/tcp_listener.cpp src/tcp_listener.h src/tcp_rcv_iter.cpp src/tcp_rcv_iter.h)

