#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include "connection_manager.h"

Connection connections[MAX_CONNECTIONS];
int connection_count = 0;

void add_connection(int sock, const char* ip, int port) {
    if (connection_count >= MAX_CONNECTIONS) {
        printf("Connection limit reached!\n");
        return;
    }
    connections[connection_count].id = connection_count + 1;
    connections[connection_count].socket_fd = sock;
    strncpy(connections[connection_count].ip, ip, ADDRESS_LENGTH);
    connections[connection_count].port = port;
    connection_count++;
}

Connection* get_connection(int id) {
    for (int i = 0; i < connection_count; i++) {
        if (connections[i].id == id) {
            return &connections[i];
        }
    }
    return NULL;
}

void list_connections() {
    printf("\nActive Connections:\n");
    printf("ID\tIP Address\tPort\n");
    for (int i = 0; i < connection_count; i++) {
        printf("%d\t%s\t%d\n", connections[i].id, connections[i].ip, connections[i].port);
    }
}

void terminate_connection(int id) {
    for (int i = 0; i < connection_count; i++) {
        if (connections[i].id == id) {
            shutdown(connections[i].socket_fd, SHUT_WR);
            close(connections[i].socket_fd);
            printf("Connection %d terminated.\n", id);
            for (int j = i; j < connection_count - 1; j++) {
                connections[j] = connections[j + 1];
                connections[j].id = j + 1; // reassign ID
            }
            connection_count--;
            return;
        }
    }
    printf("Invalid connection ID!\n");
}

void terminate_all_connections() {
    for (int i = 0; i < connection_count; i++) {
        close(connections[i].socket_fd);
    }
    connection_count = 0;
    printf("All peer connections terminated.\n");
}
