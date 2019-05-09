#pragma once

#include "NetworkUtils.h"
#include "Address.h"
#include <exception>
#include <vector>
#include <mutex>
#include <cstddef>

class Socket
{
public:
	static Socket *createSocket(int type);
	Socket(int socket_descriptor);
	Socket(const Socket &socket) = delete;
	~Socket();

	Address getRemoteAddress();
	Address *getBoundAddress();

	void enableTimeout(int timeout);
	void disableTimeout();

	int getSocket();
	void setSocket(int socket_descriptor);
	void closeSocket();
	int getSocketType();

	void _bind(Address *address);
	void _listen(int backlog);
	Socket *_accept();

	void sendall(const std::string &data);
	void sendall(const std::vector<uint8_t> &data);
	void sendall(const uint8_t *buf, size_t len);

	// TODO
	void SendTo(Address *address, const std::string &data);
	void SendTo(Address *address, const std::vector<uint8_t> &data);
	void SendTo(Address *address, const uint8_t *buf, size_t len);

	// TODO recvfrom
	std::vector<uint8_t> RecvFrom(Address *&address, size_t len);
	size_t RecvFrom(Address *&address, uint8_t *buf, size_t len);

	std::vector<uint8_t> recvall(size_t len);
	void recvall(uint8_t *buf, size_t len);
	std::vector<uint8_t> recvuntil(const std::string pattern, size_t maxlen);
	std::vector<uint8_t> recvuntil(const std::vector<uint8_t> &pattern, size_t maxlen);
	void recvuntil(uint8_t *buf, size_t buflen, const uint8_t *pattern, size_t patternlen, size_t *len);

private:
	int socket_descriptor;
	Address *boundAddress;
	std::mutex _send;
	std::mutex _recv;
	std::mutex _recvuntil;

	int timeout;

	void applyRecvTimeout();
	int recvtimeoutwrapper(void *buf, size_t len, int flags);
	int isContainPattern(const uint8_t *buf, size_t len, const uint8_t *pattern, size_t patternlen);
	bool isValidDescriptor();
};

class SocketException : public std::runtime_error
{
public:
	SocketException(std::string msg) : std::runtime_error(msg) {}
};

class SendException : public SocketException
{
public:
	SendException(std::string msg) : SocketException(msg) {}
};

class RecvException : public SocketException
{
public:
	RecvException(std::string msg) : SocketException(msg) {}
};

class SocketConnectionClosedException : public SocketException
{
public:
	SocketConnectionClosedException(std::string msg) : SocketException(msg) {}
};

class TimeoutException : public SocketException
{
public:
	TimeoutException(std::string msg) : SocketException(msg) {}
};
