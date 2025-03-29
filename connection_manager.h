// Done by Hector Velez, fixed typos by Kenneth
#ifndef CONNECTION_MANAGER_H
#define CONNECTION_MANAGER_H 

#define MAX_CONNECTIONS 10 
#define ADDRESS_LENGTH 16 

typedef struct { 
	int id; 
	int socket_fd; 
	char ip[ADDRESS_LENGTH]; 
	int port; 
} Connection; 

const int getConnectionCount();
void add_connection(int sock, const char* ip, int port); 
Connection* get_connection(int id); 
void list_connections(); 
void terminate_connection(int id); 

#endif 
