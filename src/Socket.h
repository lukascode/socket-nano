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
	static Socket *CreateSocket(int type);
	Socket(int socket_descriptor);
	Socket(const Socket &socket) = delete;
	~Socket();

	Address GetRemoteAddress();
	Address *GetBoundAddress();

	void EnableTimeout(int timeout);
	void DisableTimeout();

	int GetSocket();
	void SetSocket(int socket_descriptor);
	void CloseSocket();
	int GetSocketType();

	void Bind(Address *address);
	void Connect(Address *address);
	void Listen(int backlog);
	Socket *Accept();

	void SendAll(const std::string &data);
	void SendAll(const std::vector<uint8_t> &data);
	void SendAll(const uint8_t *buf, size_t len);

	void SendTo(Address *address, const std::string &data);
	void SendTo(Address *address, const std::vector<uint8_t> &data);
	void SendTo(Address *address, const uint8_t *buf, size_t len);

	std::vector<uint8_t> RecvFrom(Address *&address, size_t len);
	size_t RecvFrom(Address *&address, uint8_t *buf, size_t len);

	std::vector<uint8_t> RecvAll(size_t len);
	void RecvAll(uint8_t *buf, size_t len);
	std::vector<uint8_t> RecvUntil(const std::string pattern, size_t maxlen);
	std::vector<uint8_t> RecvUntil(const std::vector<uint8_t> &pattern, size_t maxlen);
	void RecvUntil(uint8_t *buf, size_t buflen, const uint8_t *pattern, size_t patternlen, size_t *len);

private:
	int socket_descriptor;
	Address *boundAddress;
	Address *connectedAddress;
	std::mutex _send;
	std::mutex _recv;
	std::mutex _recvuntil;
	int timeout;

	void ApplyRecvTimeout();
	int RecvTimeoutWrapper(void *buf, size_t len, int flags);
	int IsContainPattern(const uint8_t *buf, size_t len, const uint8_t *pattern, size_t patternlen);
	bool IsValidDescriptor();
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
