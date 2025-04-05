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

void* client_handler(void* socket_desc) {
    int client_socket = *(int*)socket_desc;
    char buffer[BUFFER_SIZE];
    int recv_len;

    send(client_socket, "Connected to chat server\n", 25, 0);

    while ((recv_len = recv(client_socket, buffer, BUFFER_SIZE - 1, 0)) > 0) {
        buffer[recv_len] = '\0';
        printf("Message received: %s\n", buffer);
    }

    if (recv_len == 0)
        printf("Client disconnected\n");
    else
        perror("recv failed");

    close(client_socket);
    free(socket_desc); // Free allocated memory
    pthread_exit(NULL);
}

void communicate_with_client(int server_socket) {
    int client_socket, c;
    struct sockaddr_in client_addr;
    pthread_t thread_id;

    c = sizeof(struct sockaddr_in);
    printf("Server is ready to handle multiple clients.\n");

    while ((client_socket = accept(server_socket, (struct sockaddr*)&client_addr, (socklen_t*)&c))) {
        printf("New client connected: %s\n", inet_ntoa(client_addr.sin_addr));

        int* new_sock = malloc(sizeof(int));
        *new_sock = client_socket;

        if (pthread_create(&thread_id, NULL, client_handler, (void*)new_sock) < 0) {
            perror("Could not create thread");
            free(new_sock);
        } else {
            printf("Client handler assigned, thread ID: %ld\n", thread_id);
        }
    }

    if (client_socket < 0) {
        perror("Accept failed");
    }
}
