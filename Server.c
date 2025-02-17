#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <regex.h>

const int BUFFER_SIZE = 1024;
const int ADDRESS_LENGTH = 16;
typedef struct sockaddr_in sockaddr_in;

// Server structure
struct Server {
int server_socket, client_socket, flag;
sockaddr_in svr_addr, conn_addr;
char* buffer;
}; 
typedef struct Server Server;

// Initialize socket address
Server setSocketAddr(Server s, int port, const char ip[]);

// Start connection
int activate_server(int server_socket, sockaddr_in s_addr);


int main(int argc, char* argv[]) {
    // check for missing user input on command interface
    if ((argv[1] == NULL) || (argv[2] == NULL)) {
        fprintf(stderr, "\nMissing port number and/or IP address.\nPlease enter in form: ./[*Program exec*] [*Port number*] [*IP Address*]\n\n");
        return -1;
    }
    Server s;
    int port = atoi(argv[1]);
    const char* ip = argv[2]; 
    s = setSocketAddr(s, port, ip);
    // check if port number and ip are valid
    if (s.flag > 0) { 
        fprintf(stderr, "Port number and/or IP address invalid.\nUnable to connect.\n\n");
        return s.flag;
    }
    // prepare socket for binding to server and listening to connections
    s.server_socket = activate_server(s.server_socket, s.svr_addr);

    // close all open sockets
    printf("\nClosing sockets...\n");
    close(s.server_socket);
    close(s.client_socket);
    printf("Successfully disconnected.\n\n");
    return 0;
}


Server setSocketAddr(Server s, int port, const char ip[]) {
    // set port number and ip
    int ipValid = -1;
    regex_t re;
    if (port < 0) {
        fprintf(stderr, "\n\nThat is not a number! Please try again.\n\n");  
        s.flag = 1;
    }
    s.svr_addr.sin_port = htons(port);

    // set Internet protocol family
    s.svr_addr.sin_family = AF_INET;

    // set Ip address 
    // check if Ip is valid
    const char* pattern = "^(([0-9]|[0-9][0-9]|[1][0-9][0-9]|[2][0-4][0-9]|[2][5][0-5])\\.){3}([0-9]|[0-9][0-9]|[1][0-9][0-9]|[2][0-4][0-9]|[2][5][0-5])$";
    ipValid = regcomp(&re, pattern, REG_EXTENDED);
    ipValid = regexec(&re, ip, 0, NULL, 0);
    if (ipValid == 0) {
        printf("\nSetting IP address to %s\n\n", ip);
        s.svr_addr.sin_addr.s_addr = inet_addr(ip);
    }

    else {
        fprintf(stderr, "\n\nInvalid IP address! Please try again.\n\n");
        s.flag = 1;
    }
    regfree(&re);
    return s;
}

int activate_server(int server_socket, sockaddr_in s_addr) {
    // connect server socket
    int errno;
    int sAddrLen = sizeof(s_addr);
    server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_socket < 0) {
        perror("Failed to connect socket.\n");
        return server_socket;
    }

    printf("Connected server socket.\nBinding socket to server...\n\n");
    errno = bind(server_socket, (struct sockaddr *)&s_addr, sAddrLen);
    if (errno < 0) {
        perror("Failed to bind socket.\n");
        return errno;
    }

    printf("Binded socket to server.\nPreparing to listen for connections...\n\n");

    errno = listen(server_socket, 3);
    if (errno < 0) {
        perror("Socket failed to listen.\n");
        return errno;
    }

    printf("Server is listening for upcoming connections...\n");
    return server_socket;
}