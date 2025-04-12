#include "Server.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "connection_manager.h"
#include <sys/select.h>

static struct sockaddr_in client_addr;

Server setSocketAddr(uint16_t port, uint32_t ip) {
    Server s;
    s.flag = 0;

    // Check if port number is valid
    if (port <= 0 || port > 65535) {
        fprintf(stderr, "Invalid port number!\n");
        s.flag = 1;
    } else {
        // Configure server socket address
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

    // Bind server socket to address
    if (bind(server_socket, (struct sockaddr *)&s_addr, sizeof(s_addr)) < 0) {
        perror("Bind failed");
        close(server_socket);
        return -1;
    }

    // Start listening
    if (listen(server_socket, MAX_CONNECTIONS) < 0) {
        perror("Listen failed");
        close(server_socket);
        return -1;
    }

    printf("Server activated and listening...\n");
    return server_socket;
}

// Thread function for handling new clients and existing threads
void* client_handler(void* socket_desc) {
    int client_socket = *(int*)socket_desc;
    char buffer[BUFFER_SIZE];
    int recv_len;

    send(client_socket, "Connected to chat server\n", 25, 0);

    while ((recv_len = recv(client_socket, buffer, BUFFER_SIZE - 1, 0)) > 0) {
        buffer[recv_len] = '\0';
        printf("Message received from %s\nSender's Port: %d\nMessage: %s\n",
              inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buffer);
    }

    if (recv_len == 0)
        printf("Client disconnected\n");
    else
        perror("recv failed\n");
    close(client_socket);
    free(socket_desc);
    pthread_exit(NULL);
}


void communicate_with_client(int server_socket,struct sockaddr_in client_addr) {
    socklen_t client_len = sizeof(client_addr);
    pthread_t thread;
    // Loop until incoming connection is accepted or not accepted
    while (1) {
        int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        if (client_socket < 0) {
            perror("accept failed");
            continue;
        }

        printf("New connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        // Once accepted, add new connection to list 
        add_connection(client_socket, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Create temporary copy of socket to add into a thread
        int* new_sock = malloc(sizeof(int));
        *new_sock = client_socket;

        // Create new thread for new connection
        if (pthread_create(&thread, NULL, client_handler, (void*)new_sock) != 0) {
            perror("Failed to create thread");
            close(client_socket);
            free(new_sock);
        } else {
            // Detach thread to freely terminate once user disconnects
            pthread_detach(thread);
        }
    }
}