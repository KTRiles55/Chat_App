#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/select.h>
#include "connection_manager.h"

#define h_addr h_addr_list[0]

int connect_to_peer(const char* host, int port) {
    int sock;
    struct sockaddr_in peer_addr;
    struct hostent* server;

    // Resolve hostname to IP address
    server = gethostbyname(host);
    if (server == NULL) {
        fprintf(stderr, "Error: No such host '%s'\n", host);
        return -1;
    }

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return -1;
    }

    // Set up peer address
    memset(&peer_addr, 0, sizeof(peer_addr));
    peer_addr.sin_family = AF_INET;
    peer_addr.sin_port = htons(port);
    memcpy(&peer_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    // Optional: set non-blocking connect with timeout
    fcntl(sock, F_SETFL, O_NONBLOCK);
    int result = connect(sock, (struct sockaddr*)&peer_addr, sizeof(peer_addr));
    if (result < 0 && errno != EINPROGRESS) {
        perror("Connection failed immediately");
        close(sock);
        return -1;
    }

    // Set timer of 5 seconds to connect before timeout
    fd_set fdset;
    struct timeval tv;
    FD_ZERO(&fdset);
    FD_SET(sock, &fdset);
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    
    if (select(sock + 1, NULL, &fdset, NULL, &tv) == 1) {
        int so_error;
        socklen_t len = sizeof(so_error);
        getsockopt(sock, SOL_SOCKET, SO_ERROR, &so_error, &len);
        if (so_error != 0) {
            fprintf(stderr, "Connection error: %s\n", strerror(so_error));
            close(sock);
            return -1;
        }
    } else {
        fprintf(stderr, "Connection timed out\n");
        close(sock);
        return -1;
    }

    // Set back to blocking mode
    fcntl(sock, F_SETFL, fcntl(sock, F_GETFL) & ~O_NONBLOCK);

    // Add connection
    add_connection(sock, inet_ntoa(*(struct in_addr*)server->h_addr), port);
    printf("Connected to %s:%d\n", inet_ntoa(*(struct in_addr*)server->h_addr), port);
    return sock;
}

	void send_message(int conn_id, const char* message) { 
		Connection* conn = get_connection(conn_id);
		if (!conn) { 
			printf("Invalid connection ID!\n"); 
			return; 
	}

	if(send(conn->socket_fd, message, strlen(message), 0) < 0) { 
		perror("Message send failed"); 
	} else {
		printf("Message sent to %s:%d\n", conn->ip, conn->port); 
	} 
}