#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>
#include "connection_manager.h"
#define BUFFER_SIZE 1024

typedef struct sockaddr_in sockaddr_in;

typedef struct Server {
    int server_socket;
    int client_socket;
    int flag;
    sockaddr_in svr_addr;

} Server;

Server setSocketAddr(uint16_t port, uint32_t ip);
int activate_server(sockaddr_in s_addr);
void* client_handler(void* socket_desc);
void communicate_with_client(int server_socket, struct sockaddr_in client_addr);

#endif // SERVER_H