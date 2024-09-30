#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <string>
#include <iostream>
#include <cerrno>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include "./gui/gui.hpp"

#define PORT 4242
#define	sock_in		struct sockaddr_in
#define	sock_addr	struct sockaddr

sock_in	init_socket_struct(void)
{
    sock_in _addr;
	// Init struct that the socket needs
	//  IPV4 addresses
	_addr.sin_family      = AF_INET;
	//  Convert our port to a network address (host to network)
	_addr.sin_port        = htons(PORT);
	//  Our address as integer
	_addr.sin_addr.s_addr = INADDR_ANY; // Probar Localhost
    return _addr;
}

void send_command(int sockfd, const std::string& command) {
    send(sockfd, command.c_str(), command.size(), 0);
}

int create_socket(void) {
	sock_in addr = init_socket_struct();
    int socket_fd = 0;
	// Create the socket
	if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
			exit(EXIT_FAILURE);
	}
	if (connect(socket_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "Connection to daemon failed.\n";
        close(socket_fd);
        exit(EXIT_FAILURE);
    }
    return socket_fd;
}

int main ( void )
{
    int client_fd = create_socket();
    x11_gui gui;
    gui.display_client(client_fd);
    close(client_fd);
    return EXIT_SUCCESS;
}