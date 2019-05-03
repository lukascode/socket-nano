#ifndef SOCKET_H
#define SOCKET_H

#include "NetworkUtils.h"
#include "Address.h"
#include <exception>
#include <vector>
#include <mutex>

class Socket 
{
public:

	static Socket* createSocket(int type); /* type SOCK_STREAM / SOCK_DGRAM */
	Socket(int socket_descriptor);
	Socket(const Socket& socket) = delete;
	~Socket();

	Address getRemoteAddress();
	Address* getBoundAddress();

	int getSocket();
	void setSocket(int socket_descriptor);
	void closeSocket();
	int getSocketType();

	void _bind(Address* address);
	void _listen(int backlog);
	Socket* _accept();

	int sendall(const std::vector<uint8_t>& data, int* sended);
	int recvall(std::vector<uint8_t>& data, int size);
	int recvuntil(std::vector<uint8_t>& data, const std::vector<uint8_t>& pattern);

	int sendall(const uint8_t* buf, int* len);
	int recvall(uint8_t* buf, int* len);
	int recvuntil(uint8_t* buf, int maxlen, const uint8_t* pattern, int patternlen, int* len);
	int recvtimeout(int s, uint8_t* buf, int len, int timeout);

private:
	int socket_descriptor;
	Address* boundAddress;

	std::mutex _send;
	std::mutex _recv;

	int isContain(const uint8_t* buf, int buflen, const uint8_t* pattern, int patternlen);
	bool isValidDescriptor();
};

class SocketException : public std::runtime_error 
{
public:
	SocketException(std::string msg): std::runtime_error(msg) {}
};

#endif /* SOCKET_H */
