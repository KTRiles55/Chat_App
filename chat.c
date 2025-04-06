// Done by Kenneth
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "Server.h"
#include "Command_handler.h"
#include "Client.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <Port>\n", argv[0]);
        return -1;
    }

    Server s;
    int port = atoi(argv[1]);
    char ipAddress[ADDRESS_LENGTH];
    s = setSocketAddr(port, INADDR_ANY);

    if (s.flag > 0) {
        fprintf(stderr, "Invalid port number.\n");
        return s.flag;
    }

    s.server_socket = activate_server(s.svr_addr);
    char input[256];

    while (1) {
        printf(">> ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "exit") == 0) {
            break;
        }

        else if (strncmp(input, "connect", 7) == 0) {
        struct sockaddr_in client_addr;
        sscanf(input, "connect %s %d", ipAddress, &port);
        client_addr.sin_addr.s_addr = inet_addr(ipAddress);
        client_addr.sin_port = htons(port);
        communicate_with_client(s.server_socket, client_addr);
        connect_to_peer((const char*)ipAddress, port);
    }
        else {
            execute_command(input);
        }
    }

    close(s.server_socket);
    return 0;
}