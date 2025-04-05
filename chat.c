// Done by Kenneth
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "Server.h"
#include "Client.h"
#include "Command_handler.h"
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <pthread.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "\nMissing port number and/or IP address.\nPlease enter in form: ./program [Port] [IP]\n\n");
        return -1;
    }
    
    Server s;
    struct sockaddr_in sa;
    const char ipAddress[INET_ADDRSTRLEN];
    int port = atoi(argv[1]);
    s = setSocketAddr(port, INADDR_ANY);

    if (s.flag > 0) {
        fprintf(stderr, "Invalid port number.\n");
        return s.flag;
    }

    s.server_socket = activate_server(s.svr_addr);
    char input[256];

    // Handle user commands and client communication concurrently
    while (1) {
        // Accept client connections
        int* new_sock = malloc(sizeof(int));
        *new_sock = communicate_with_client(s.server_socket);
        if (*new_sock < 0) {
            perror("Accept failed");
            free(new_sock);
            continue;
        }

        printf("Client connected\n");

        // Create a new thread to handle this client
        pthread_t client_thread;
        if (pthread_create(&client_thread, NULL, (void*)client_handler, (void*)new_sock) < 0) {
            perror("Thread creation failed");
            free(new_sock);
            continue;
        }

        // Handle user commands (exit, myip, etc.)
        scanf("%s", input);

        if (strncmp(input, "exit", 4) == 0) {
            printf("Exiting...\n");
            break;  // Exit the loop and shut down the server
        }
        else if (strncmp(input, "myip", 4) == 0) {
            inet_ntop(AF_INET, &sa.sin_addr, (char*)ipAddress, INET_ADDRSTRLEN);
            printf("Server IP: %s\n", ipAddress);
        }
        else if (strncmp(input, "myport", 6) == 0) {
            printf("Server Port: %d\n", ntohs(sa.sin_port));
        }
        else {
            execute_command(input);
        }
    }

    pthread_exit(NULL);
    close(s.server_socket);  // Close the server socket
    return 0;
}