// Done by Hector Velez, fixed typos by Kenneth
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "connection_manager.h"


int connect_to_peer(const char* ip, int port) { 
	int sock;
	struct sockaddr_in peer_addr;
	
	// Create socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) { 
		perror("Socket creation failed");
		return -1; 
	}

	// Setup peer address 
	peer_addr.sin_family = AF_INET; 
	peer_addr.sin_port = htons(port);
	peer_addr.sin_addr.s_addr = inet_addr(ip);
 
	// Connect to the peer
	if (connect(sock, (struct sockaddr*)&peer_addr, sizeof(peer_addr)) < 0) {
	perror ("Connection failed");
	close(sock);
	return -1; 
	}

	// Add connection to the list
	add_connection(sock, ip, port); 
	printf("Connected to %s:%d\n", ip, port); 
	return sock; 
	}

	void send_message(int conn_id, const char message[]) { 
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
	
