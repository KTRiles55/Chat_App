// Done by Kenneth
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "Server.h"
#include "Command_handler.h"
#include "Client.h"
#include "connection_manager.h"

// Add new thread for handling accepting connections from new clients
void* listener_thread_func(void* arg) {
    int server_socket = *(int*)arg;
    struct sockaddr_in client_addr;
    communicate_with_client(server_socket, client_addr);
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <Port>\n", argv[0]);
        return -1;
    }

    int port = atoi(argv[1]);
    // Assign port and machine's ip address to server socket
    Server s = setSocketAddr(port, INADDR_ANY);

    if (s.flag > 0) {
        fprintf(stderr, "Invalid port number.\n");
        return s.flag;
    }

    // Bind server socket to address and make it listen for incoming connections
    s.server_socket = activate_server(s.svr_addr);
    if (s.server_socket < 0) {
        fprintf(stderr, "Failed to start server.\n");
        return -1;
    }

    // Create a listener socket thread
    pthread_t listener_thread;
    pthread_create(&listener_thread, NULL, listener_thread_func, &s.server_socket);
    // Detach thread from join operations to terminate freely
    pthread_detach(listener_thread);

    printf("Chat server is up. Type 'help' for available commands.\n");

    char input[256];
    char ipAddress[ADDRESS_LENGTH];
    // Start loop for input commands until "exit" is entered
    while (1) {
        printf(">> ");
        fflush(stdout);
        if (!fgets(input, sizeof(input), stdin)) break;
        input[strcspn(input, "\n")] = 0;  // Remove newline

        if (strcmp(input, "exit") == 0) {
            break;
        }
        else if (strncmp(input, "myip", 4) == 0) {
            printf("%d\n", s.svr_addr.sin_addr.s_addr);
        }
        else if (strncmp(input, "myport", 6) == 0) {
            printf("%d\n", port);
        }
        else if (strncmp(input, "connect", 7) == 0) {
            int peer_port;
            if (sscanf(input, "connect %s %d", ipAddress, &peer_port) == 2) {
                connect_to_peer(ipAddress, peer_port);
            } else {
                printf("Usage: connect <IP> <PORT>\n");
            }
        }
        else {
            execute_command(input);
        }
    }
    // Close server socket and terminate remanining threads
    close(s.server_socket);
    pthread_exit(NULL);
    printf("Server shut down.\n");
    return 0;
}