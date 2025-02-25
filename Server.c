#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <regex.h>

const int BUFFER_SIZE = 1024;
const int ADDRESS_LENGTH = 16;
const int TOTAL_CONNECTIONS = 10;
typedef struct sockaddr_in sockaddr_in;

// Server structure
struct Server {
int server_socket, client_socket, flag;
sockaddr_in svr_addr, conn_addr;
}; 
typedef struct Server Server;

// Initialize socket address
Server setSocketAddr(Server s, int port, const char ip[]);

// Start connection
int activate_server(int server_socket, sockaddr_in s_addr);

// communicate with clients
void communicate_with_client(int s_sock, int c_sock, sockaddr_in sAddr, sockaddr_in cAddr);


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

    errno = bind(server_socket, (struct sockaddr *)&s_addr, sAddrLen);
    if (errno < 0) {
        perror("Failed to bind socket.\n");
        return errno;
    }

    errno = listen(server_socket, TOTAL_CONNECTIONS);
    if (errno < 0) {
        perror("Socket failed to listen.\n");
        return errno;
    }
    
    return server_socket;
}


void communicate_with_client(int s_sock, int c_sock, sockaddr_in sAddr, sockaddr_in cAddr) {
    // initialize client address to server address
    char buffer[BUFFER_SIZE];
    int connected = 1;
    const char* message = "User connected to the chat server.\n\n";
    int cAddrSize = sizeof(cAddr);
    cAddr.sin_addr = sAddr.sin_addr;
    cAddr.sin_family = sAddr.sin_family;
    cAddr.sin_port = sAddr.sin_port;

    // attempt to detect connections from client
    while (connected > 0) {
        c_sock = accept(s_sock, (struct sockaddr*)&cAddr, &cAddrSize);
        if (c_sock < 0) {
            perror("Connection is not established.\n");
            connected = 0;
        }

        else {
            // send message to server when client is connected
            if (sendmsg(c_sock, (struct msghdr*)&message, 0) < 0) {
                printf("Failed to transmit message to server.\n");
                connected = 0;
            }
            // receive message from client socket
            if (recvmsg(s_sock, (struct msghdr*)&message, 0) < 0) {
                printf("Failed to receive message from client.\n");
                connected = 0;
            }
        }
    }
    

}