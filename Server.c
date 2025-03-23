#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <regex.h>
#include <string.h>
#include <errno.h>

#define BUFFER_SIZE 1024
#define TOTAL_CONNECTIONS 10

typedef struct sockaddr_in sockaddr_in;

struct Server {
    int server_socket;
    int client_socket;
    int flag;
    sockaddr_in svr_addr;
    sockaddr_in conn_addr;
};
typedef struct Server Server;

// Initialize socket address
Server setSocketAddr(Server s, int port, const char ip[]);

// Start connection
int activate_server(sockaddr_in s_addr);

// communicate with clients
void communicate_with_client(int s_sock);

Server setSocketAddr(Server s, int port, const char ip[]) {
    s.flag = 0; // Initialize flag
    regex_t re;

    if (port <= 0 || port > 65535) {
        fprintf(stderr, "\nInvalid port number! Please use range 1-65535.\n\n");  
        s.flag = 1;
        return s;
    }
    s.svr_addr.sin_port = htons(port);
    s.svr_addr.sin_family = AF_INET;

    const char* pattern = "^(([0-9]{1,3})\\.){3}([0-9]{1,3})$";
    if (regcomp(&re, pattern, REG_EXTENDED) != 0 || regexec(&re, ip, 0, NULL, 0) != 0) {
        fprintf(stderr, "\nInvalid IP address format! Please try again.\n\n");
        s.flag = 1;
    } else {
        s.svr_addr.sin_addr.s_addr = inet_addr(ip);
    }
    regfree(&re);
    s.client_socket = -1; // initialize to an invalid socket
    return s;
}

int activate_server(sockaddr_in s_addr) {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        return -1;
    }

    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        close(server_socket);
        return -1;
    }

    if (bind(server_socket, (struct sockaddr *)&s_addr, sizeof(s_addr)) < 0) {
        perror("Failed to bind socket");
        close(server_socket);
        return -1;
    }

    if (listen(server_socket, TOTAL_CONNECTIONS) < 0) {
        perror("Socket failed to listen");
        close(server_socket);
        return -1;
    }

    printf("Server is activated and listening...\n");
    return server_socket;
}

void communicate_with_client(int s_sock) {
    char buffer[BUFFER_SIZE];
    int c_sock;
    sockaddr_in cAddr;
    socklen_t cAddrSize = sizeof(cAddr);

    while (1) {
        c_sock = accept(s_sock, (struct sockaddr*)&cAddr, &cAddrSize);
        if (c_sock < 0) {
            perror("Accept failed");
            continue; // try next connection
        }

        printf("Client connected: %s\n", inet_ntoa(cAddr.sin_addr));
        
        const char* welcome_msg = "User connected to the chat server.\n";
        if (send(c_sock, welcome_msg, strlen(welcome_msg), 0) < 0) {
            perror("Failed to transmit message to client");
            close(c_sock);
            continue;
        }

        int recv_len;
        while ((recv_len = recv(c_sock, buffer, BUFFER_SIZE - 1, 0)) > 0) {
            buffer[recv_len] = '\0';
            printf("Received: %s\n", buffer);
        }

        if (recv_len < 0) {
            perror("Failed to receive message from client");
        }

        printf("Client disconnected: %s\n", inet_ntoa(cAddr.sin_addr));
        close(c_sock);
    }
}
