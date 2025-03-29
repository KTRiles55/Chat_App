// Done by Hector, edited by Kenneth
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "connection_manager.h"

static Connection connections[MAX_CONNECTIONS];
static int connection_count = 0;

const int getConnectionCount() { return connection_count; }

void add_connection(int sock, const char* ip, int port) {
    if (connection_count >= MAX_CONNECTIONS) {
        printf("Maximum connections reached!\n");
        return;
    }
    connections[connection_count].id = connection_count;
    connections[connection_count].socket_fd = sock;
    strncpy(connections[connection_count].ip, ip, ADDRESS_LENGTH);
    connections[connection_count].port = port;
    connection_count++;
}

Connection* get_connection(int id) {
    if (id < 0 || id >= connection_count) return NULL;
    return &connections[id];
}

void list_connections() {
    printf("\nActive connections:\n");
    for (int i = 0; i < connection_count; i++) {
        printf("ID: %d, IP: %s, Port: %d\n",
            connections[i].id,
            connections[i].ip,
            connections[i].port);
    }
}

void terminate_connection(int id) {
    Connection* conn = get_connection(id);
    if (!conn) {
        printf("Invalid connection ID!\n");
        return;
    }
    close(conn->socket_fd);
    printf("Connection %d terminated.\n", id);

    for (int i = id; i < connection_count - 1; i++)
        connections[i] = connections[i + 1];

    connection_count--;
}
