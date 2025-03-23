// Done by Kenneth
#ifndef CLIENT_H
#define CLIENT_H

int connect_to_peer(const char* ip, int port);
void send_message(int conn_id, const char* message);

#endif // CLIENT_H
