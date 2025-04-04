// Done by Kenneth
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "Server.h"
#include "Client.h"
#include "Command_handler.h"
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "\nMissing port number and/or IP address.\nPlease enter in form: ./program [Port] [IP]\n\n");
        return -1;
    }
    
    Server s;
    int connection = 1;
    const char ipAddress[INET_ADDRSTRLEN];
    int port = atoi(argv[1]);
    uint32_t ip;
    s = setSocketAddr(port, INADDR_ANY);
    ip = s.svr_addr.sin_addr.s_addr;
    

    if (s.flag > 0) {
        fprintf(stderr, "Invalid port number.\n");
        return s.flag;
    }

    s.server_socket = activate_server(s.svr_addr);
    char input[256];
    while (connection > 0) {
        communicate_with_client(s.server_socket);
    while (fgets(input, sizeof(input), stdin)) {
        if (strncmp(input, "exit", 4) == 0) {
            connection = 0;
            break;
        }
        else if (strncmp(input, "myip", 4) == 0) {
            inet_ntop(AF_INET, &ip, (char*)ipAddress, INET_ADDRSTRLEN);
            printf("%s\n", ipAddress); 
        }
        else if (strncmp(input, "myport", 6) == 0) {
            printf("%d\n", port);
        }
        else {
            execute_command(input);
        }
    }
}
    
    printf("\nClosing sockets...\n");
    close(s.server_socket);
    close(s.client_socket);
    printf("Successfully disconnected.\n\n");

    return 0;
}
