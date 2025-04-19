// Done by Kenneth
#ifndef CLIENT_H
#define CLIENT_H

int connect_to_peer(const char* host, int port);
void send_message(int conn_id, const char* message);
void* receiver_thread(void* arg);

#endif // CLIENT_H
