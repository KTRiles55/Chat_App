// Done by Kenneth
#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "connection_manager.h"
#include "Client.h"
#include <netdb.h>

// Connects to a peer given hostname/IP and port.
int connect_to_peer(const char* hostname, int port) {
    int sock;
    struct sockaddr_in peer_addr;
    struct hostent *host;

    // Resolve hostname to IP (supports dynamic DNS/hostnames)
    host = gethostbyname(hostname);
    if (!host) {
        perror("Hostname resolution failed");
        return -1;
    }

    // Create TCP socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return -1;
    }

    // Setup peer address structure
    peer_addr.sin_family = AF_INET;
    peer_addr.sin_port = htons(port);
    peer_addr.sin_addr = *(struct in_addr*)host->h_addr;

    // Attempt to connect to peer
    if (connect(sock, (struct sockaddr*)&peer_addr, sizeof(peer_addr)) < 0) {
        perror("Connection failed");
        close(sock);
        return -1;
    }

    // Successfully connected, add to connection list
    add_connection(sock, inet_ntoa(peer_addr.sin_addr), port);
    printf("Successfully connected to %s (%s) on port %d\n", hostname, inet_ntoa(peer_addr.sin_addr), port);
    
    return sock;
}

// Sends a message to a connected peer identified by connection ID.
void send_message(int conn_id, const char* message) {
    Connection* conn = get_connection(conn_id);
    if (!conn) {
        printf("Invalid connection ID: %d\n", conn_id);
        return;
    }

    // Send message over the socket
    if (send(conn->socket_fd, message, strlen(message), 0) < 0) {
        perror("Sending message failed");
    } else {
        printf("Message sent successfully to %s:%d\n", conn->ip, conn->port);
    }
}