#ifndef SOCKET_H
#define SOCKET_H

#include "NetworkUtils.h"
#include "Address.h"

class Socket {

public:
	Socket(int type, int* err); /* type ~ SOCK_STREAM/SOCK_DGRAM */
	Socket(int socket_descriptor);
	~Socket();


	int getDescriptor();
	void setDescriptor(int desc);
	Address getRemoteAddress(int* err);
	int closeSocket();
	int Send(std::string* data);
	int Recv(std::string* data, int nbytes);

	int sendall(const char* buf, int* len);
	int recvall(char* buf, int* len);

	int isValid();

private:
	int descriptor;

};


#endif /* SOCKET_H */