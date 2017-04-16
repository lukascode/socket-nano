#ifndef SOCKET_H
#define SOCKET_H

#include "NetworkUtils.h"
#include "Address.h"
#include <exception>
#include <vector>
#include <mutex>

class Socket {

public:
	Socket(int type, int notsocketdescriptor); /* type ~ SOCK_STREAM/SOCK_DGRAM */
	Socket(int socket_descriptor);
	~Socket();


	int getDescriptor();
	void setDescriptor(int desc);
	Address getRemoteAddress();
	int closeSocket();

	int sendall(const std::vector<uint8_t>& data, int* sended);
	int recvall(std::vector<uint8_t>& data, int size);
	int recvuntil(std::vector<uint8_t>& data, const std::vector<uint8_t>& pattern);

	int sendall(const uint8_t* buf, int* len);
	int recvall(uint8_t* buf, int* len);
	int recvuntil(uint8_t* buf, int maxlen, const uint8_t* pattern, int patternlen, int* len);
	int recvtimeout(int s, uint8_t* buf, int len, int timeout);
	int isValid();
	int getSockType();

private:
	int descriptor;
	std::mutex _send;
	std::mutex _recv;

	int isContain(const uint8_t* buf, int buflen, const uint8_t* pattern, int patternlen);

};

class SocketException : public std::exception {
public:
	SocketException(std::string msg) {
		this->msg = msg;
	}
	virtual const char* what() const noexcept {
		return msg.c_str();
	}	
private:
	std::string msg;
};

#endif /* SOCKET_H */
