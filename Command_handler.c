#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include "connection_manager.h"
#include "Client.c" 

void execute_command(char* input) { 
	char command[256]; 
	sscanf(input, "%s", command); 

	if(strcmp(command, "help") == 0) { 
		printf("\nAvailable Commands:\n");
		printf("1. help - Show available commands\n"); 
		print("2. myip - Display the local IP address\n"); 
		printf("3. myport - Show the port this server is listening on\n"); 
		printf("4. connect <IP> <port> - Connect to a peer\n"); 
		printf("5. list - Show active connections\n"); 
		printf("6. terminate <id> - Close a specific connection\n"); 
		printf("7. send <id> <message> - Send a message to connection\n"); 
		printf("8. exit - Close all connections and exit\n");
}

else if (strcmp(command, "list") == 0) { 
	list_connections(); 
}
else if (strncmp(command, "connect", 7) == 0) { 
	char ip[ADDRESS_LENGTH]; 
	int port; 
	sscanf(input, "connect %s %d, ip, &port"); 
	connect_to_peer(ip, port); 
} 
else if (strncmp(command, "terminate", 9) == 0) { 
	int id; 
	sscanf(input, "terminate %d, &id"); 
	terminate_connection(id); 
} 
else if (strncmp(command, "send", 4) == 0) { 
	int id; 
	const char* message[100];
	sscanf(input, "send %d %[^\n]", &id, message); 
	send_message(id, message); 
}
else if (strcmp(command, "exit") == 0) { 
	printf("Closing all connections and exiting...\n"); 
	//exit(0); 
}
else {
	printf("Unknown command. Type 'help' for a list of commands.\n");
	
}

}