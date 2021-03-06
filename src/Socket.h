#pragma once

#include <exception>
#include <stdexcept>
#include <vector>
#include <mutex>
#include <cstddef>
#include <string>
#include "NetworkUtils.h"
#include "Address.h"
#include "NanoException.h"
#include <poll.h>
#include <memory>

class Socket
{
public:
	/// Creates tcp/udp socket object base on type (SOCK_STREAM / SOCK_DGRAM)
	static std::shared_ptr<Socket> Create(int type);

	/// Creates socket object using provided existing descriptor
	Socket(int socket_descriptor);
	Socket(const Socket &socket) = delete;
	~Socket();

	/// Gets the socket low level descriptor
	int GetSocket();

	/// Sets the socket low level descriptor
	void SetSocket(int socket_descriptor);

	/// Checks if socket is valid
	bool Valid();

	/// Enables read timeout in secons
	void EnableTimeout(int timeout);

	/// Disables read timeout
	void DisableTimeout();

	Address GetRemoteAddress();
	std::shared_ptr<Address> GetBoundAddress();

	void Close();
	void Shutdown();

	int GetSocketType();

	void Bind(std::shared_ptr<Address> address);
	void Connect(std::shared_ptr<Address> address);
	void Listen(int backlog);

	/// Accept incoming connection and return back client socket
	std::shared_ptr<Socket> Accept();

	// TCP
	void SendAll(const std::string &data);
	void SendAll(const std::vector<uint8_t> &data);
	void SendAll(const uint8_t *buf, size_t len);
	std::string RecvAllString(size_t len);
	std::vector<uint8_t> RecvAll(size_t len);
	void RecvAll(uint8_t *buf, size_t len);
	std::string RecvUntilString(const std::string pattern, size_t maxlen);
	std::vector<uint8_t> RecvUntil(const std::string pattern, size_t maxlen);
	std::vector<uint8_t> RecvUntil(const std::vector<uint8_t> &pattern, size_t maxlen);
	void RecvUntil(uint8_t *buf, size_t buflen, const uint8_t *pattern, size_t patternlen, size_t *len);

	// UDP
	void SendTo(const std::shared_ptr<Address> address, const std::string &data);
	void SendTo(const std::shared_ptr<Address> address, const std::vector<uint8_t> &data);
	void SendTo(const std::shared_ptr<Address> address, const uint8_t *buf, size_t len);
	std::vector<uint8_t> RecvFrom(std::shared_ptr<Address> &address, size_t len);
	size_t RecvFrom(std::shared_ptr<Address> &address, uint8_t *buf, size_t len);

private:
	int socket_descriptor;
	std::shared_ptr<Address> boundAddress;
	std::shared_ptr<Address> connectedAddress;
	std::mutex _send;
	std::mutex _recv;
	std::mutex _recvuntil;
	int timeout;

	void ApplyRecvTimeout();
	int RecvTimeoutWrapper(void *buf, size_t len, int flags);
	int IsContainPattern(const uint8_t *buf, size_t len, const uint8_t *pattern, size_t patternlen);
	bool IsValidDescriptor();
};

class SocketException : public NanoException
{
public:
	SocketException(std::string msg) : NanoException(msg) {}
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
