// Done by Kenneth
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "Server.h"
#include "Client.h"
#include "Command_handler.h"
#include <string.h>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "\nMissing port number and/or IP address.\nPlease enter in form: ./program [Port] [IP]\n\n");
        return -1;
    }

    Server s;
    int port = atoi(argv[1]);
    const char* ip = argv[2];
    s = setSocketAddr(port, ip);

    if (s.flag > 0) {
        fprintf(stderr, "Invalid port number and/or IP address.\n");
        return s.flag;
    }

    s.server_socket = activate_server(s.svr_addr);
    communicate_with_client(s.server_socket);
    char input[256];
    while (fgets(input, sizeof(input), stdin)) {
        if (strncmp(input, "exit", 4) == 0) break;
        execute_command(input);
    }

    printf("\nClosing sockets...\n");
    close(s.server_socket);
    close(s.client_socket);
    printf("Successfully disconnected.\n\n");

    return 0;
}
