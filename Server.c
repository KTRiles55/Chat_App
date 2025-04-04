// Done by Kenneth
#include "Server.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <regex.h>
#include <string.h>
#include "connection_manager.h"
#include <pthread.h>


Server setSocketAddr(int port, uint32_t ip) {
    Server s;
    s.flag = 0;

    if (port <= 0 || port > 65535) {
        fprintf(stderr, "\nInvalid port number!\n");
        s.flag = 1;
    } else {
        s.svr_addr.sin_family = AF_INET;
        s.svr_addr.sin_port = htons(port);
        s.svr_addr.sin_addr.s_addr = htonl(ip);
        s.client_socket = -1;
    }
    return s;
}

int activate_server(sockaddr_in s_addr) {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        return -1;
    }

    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(server_socket, (struct sockaddr *)&s_addr, sizeof(s_addr)) < 0) {
        perror("Failed to bind socket");
        close(server_socket);
        return -1;
    }

    if (listen(server_socket, MAX_CONNECTIONS) < 0) {
        perror("Socket failed to listen");
        close(server_socket);
        return -1;
    }

    printf("Server is activated and listening...\n");
    return server_socket;
}

void communicate_with_client(int server_socket) {
    char buffer[BUFFER_SIZE];
    int client_socket;
    sockaddr_in client_addr;
    Connection* conn;
    socklen_t client_len = sizeof(client_addr);

    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
    add_connection(client_socket, (const char*)&client_addr.sin_addr, client_addr.sin_port);
    conn = get_connection(getConnectionCount()-1);
    if (client_socket < 0) {
        perror("Accept failed");
        return;
    }
    printf("User %d has entered the chat.\n", conn->id);
    send(client_socket, "Welcome to the Server!\n", 23, 0);

    int bytes_received;
    while ((bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0)) > 0) {
        buffer[bytes_received] = '\0';
        //printf("Received:\n%s", buffer);
    }
}
