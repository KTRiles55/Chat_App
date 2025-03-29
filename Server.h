// Done by Hector Velez, fixed typos by Kenneth
#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>

#define BUFFER_SIZE 1024

typedef struct sockaddr_in sockaddr_in;

typedef struct Server {
    int server_socket;
    int client_socket;
    int flag;
    sockaddr_in svr_addr;
    sockaddr_in conn_addr;

} Server;

Server setSocketAddr(int port, const char ip[]);
int activate_server(sockaddr_in s_addr);
void communicate_with_client(int server_socket);

#endif // SERVER_H
