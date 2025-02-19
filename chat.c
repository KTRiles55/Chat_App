#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <regex.h>
#include "Server.c"


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
    communicate_with_client(s.server_socket, s.client_socket, s.svr_addr, s.conn_addr);

    // close all open sockets
    printf("\nClosing sockets...\n");
    close(s.server_socket);
    close(s.client_socket);
    printf("Successfully disconnected.\n\n");
    return 0;
}