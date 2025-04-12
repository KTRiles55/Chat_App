// Done by Kenneth
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <netdb.h>
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
    Server s = setSocketAddr(port, INADDR_ANY);

    if (s.flag > 0) {
        fprintf(stderr, "Invalid port number.\n");
        return s.flag;
    }

    s.server_socket = activate_server(s.svr_addr);
    if (s.server_socket < 0) {
        fprintf(stderr, "Failed to start server.\n");
        return -1;
    }

    pthread_t listener_thread;
    pthread_create(&listener_thread, NULL, listener_thread_func, &s.server_socket);
    pthread_detach(listener_thread);

    printf("Chat server is up. Type 'help' for available commands.\n");

    char input[256];
    char ipAddress[ADDRESS_LENGTH];

    while (1) {
        printf(">> ");
        fflush(stdout);
        if (!fgets(input, sizeof(input), stdin)) break;
        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "exit") == 0) {
            terminate_all_connections();
            close(s.server_socket);
            printf("Server shut down.\n");
            pthread_exit(NULL);
        }
        else if (strncmp(input, "myip", 4) == 0) {
            int sock = socket(AF_INET, SOCK_DGRAM, 0);
            if (sock == -1) {
                perror("socket");
                continue;
            }

            const char* kGoogleDnsIp = "8.8.8.8";
            int dns_port = 53;

            struct sockaddr_in serv;
            memset(&serv, 0, sizeof(serv));
            serv.sin_family = AF_INET;
            serv.sin_addr.s_addr = inet_addr(kGoogleDnsIp);
            serv.sin_port = htons(dns_port);

            int err = connect(sock, (const struct sockaddr*)&serv, sizeof(serv));
            if (err == -1) {
                perror("connect");
                close(sock);
                continue;
            }

            struct sockaddr_in name;
            socklen_t namelen = sizeof(name);
            err = getsockname(sock, (struct sockaddr*)&name, &namelen);
            if (err == -1) {
                perror("getsockname");
                close(sock);
                continue;
            }

            char buffer[INET_ADDRSTRLEN];
            const char* p = inet_ntop(AF_INET, &name.sin_addr, buffer, sizeof(buffer));
            if (p != NULL) {
                printf("Local IP: %s\n", buffer);
            } else {
                printf("Failed to get local IP address.\n");
            }
            close(sock);
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

    return 0;
}