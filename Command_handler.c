// Done by Hector Velez, fixed typos by Kenneth
#include <stdio.h>
#include <string.h>
#include "connection_manager.h"
#include "Client.h"
#include "Command_handler.h"

void execute_command(char* input) {
    char command[256];
    sscanf(input, "%s", command);

    if (strcmp(command, "help") == 0) {
        printf("\nAvailable Commands:\n");
        printf("1. help - Show available commands\n");
        printf("2. myip - Display local IP\n");
        printf("3. myport - Show server port\n");
        printf("4. connect <IP> <port> - Connect to peer\n");
        printf("5. list - Show active connections\n");
        printf("6. terminate <id> - Close connection\n");
        printf("7. send <id> <message> - Send message\n");
        printf("8. exit - Exit chat\n");
    } else if (strcmp(command, "list") == 0) {
        list_connections();
    } else if (strncmp(command, "connect", 7) == 0) {
        char ip[ADDRESS_LENGTH]; int port;
        sscanf(input, "connect %s %d", ip, &port);
        connect_to_peer(ip, port);
    }
     else if (strncmp(command, "terminate", 9) == 0) {
        int id;
        sscanf(input, "terminate %d", &id);
        terminate_connection(id);
    } else if (strncmp(command, "send", 4) == 0) {
        int id; char message[100];
        sscanf(input, "send %d %[^\n]", &id, message);
        send_message(id, message);
    } else {
        printf("Unknown command.\n");
    }
}
