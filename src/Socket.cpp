#include "Socket.h"

Socket *Socket::createSocket(int type)
{
	int socket_descriptor = socket(AF_INET, type, 0);
	if (socket_descriptor < 0)
	{
		std::string err(strerror(errno));
		throw SocketException("socket error: " + err);
	}
	return new Socket(socket_descriptor);
}

Socket::Socket(int socket_descriptor)
{
	setSocket(socket_descriptor);
	boundAddress = nullptr;
	timeout = 0;
}

Socket::~Socket()
{
	if (isValidDescriptor())
	{
		closeSocket();
	}
	if (boundAddress)
	{
		delete boundAddress;
	}
}

int Socket::getSocket()
{
	return socket_descriptor;
}

void Socket::setSocket(int socket_descriptor)
{
	this->socket_descriptor = socket_descriptor;
	if (!isValidDescriptor())
	{
		throw SocketException("Invalid socket descriptor");
	}
}

void Socket::enableTimeout(int timeout)
{
	if (timeout > 0)
	{
		this->timeout = timeout;
	}
}

void Socket::disableTimeout()
{
	this->timeout = 0;
}

bool Socket::isValidDescriptor()
{
	return (fcntl(socket_descriptor, F_GETFD) != -1) || (errno != EBADF);
}

int Socket::getSocketType()
{
	int type;
	socklen_t length = sizeof(int);
	getsockopt(socket_descriptor, SOL_SOCKET, SO_TYPE, &type, &length);
	return type;
}

void Socket::_bind(Address *address)
{
	int yes = 1;
	if (setsockopt(socket_descriptor, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0)
	{
		std::string err(strerror(errno));
		throw SocketException("setsockopt error: " + err);
	}
	if (!address)
	{
		throw std::invalid_argument("Param address must not be null");
	}
	if (bind(socket_descriptor, (struct sockaddr *)address->getRawAddress(), sizeof(struct sockaddr)) < 0)
	{
		std::string err(strerror(errno));
		throw SocketException("bind error: " + err);
	}
	this->boundAddress = address;
}

void Socket::_listen(int backlog)
{
	if (listen(socket_descriptor, backlog) < 0)
	{
		std::string err(strerror(errno));
		throw SocketException("listen error: " + err);
	}
}

Socket *Socket::_accept()
{
	int socket = accept(socket_descriptor, nullptr, nullptr);
	if (socket < 0)
	{
		std::string err(strerror(errno));
		throw SocketException("accept error: " + err);
	}
	return new Socket(socket);
}

Address Socket::getRemoteAddress()
{
	struct sockaddr_in remote_addr;
	socklen_t addrlen = sizeof(struct sockaddr);
	int ret = getpeername(socket_descriptor, (struct sockaddr *)&remote_addr, &addrlen);
	if (ret != -1)
	{
		return Address(remote_addr);
	}
	else
	{
		std::string err(strerror(errno));
		throw SocketException("getpeername error: " + err);
	}
}

Address *Socket::getBoundAddress()
{
	return boundAddress;
}

void Socket::closeSocket()
{
	if (close(socket_descriptor) < 0)
	{
		std::string err(strerror(errno));
		throw SocketException("close: " + err);
	}
}

void Socket::sendall(const std::string &data)
{
	sendall(std::vector<uint8_t>(data.begin(), data.end()));
}

void Socket::sendall(const std::vector<uint8_t> &data)
{
	sendall(data.data(), data.size());
}

std::vector<uint8_t> Socket::recvall(size_t size)
{
	std::vector<uint8_t> data(size);
	recvall(data.data(), data.size());
	return data;
}

std::vector<uint8_t> Socket::recvuntil(const std::string pattern, size_t maxlen)
{
	return recvuntil(std::vector<uint8_t>(pattern.begin(), pattern.end()), maxlen);
}

std::vector<uint8_t> Socket::recvuntil(const std::vector<uint8_t> &pattern, size_t maxlen)
{
	size_t len = 0;
	std::vector<uint8_t> data(maxlen);
	recvuntil(data.data(), data.size(), pattern.data(), pattern.size(), &len);
	data.resize(len);
	return data;
}

void Socket::sendall(const uint8_t *buf, size_t len)
{
	size_t total = 0;
	size_t bytesleft = len;
	ssize_t n;

	std::lock_guard<std::mutex> lock(_send);
	while (bytesleft > 0)
	{
		if ((n = send(socket_descriptor, buf + total, bytesleft, 0)) <= 0)
		{
			if (n < 0 && errno == EINTR)
				n = 0;
			else
			{
				n = -1;
				break;
			}
		}
		total += n;
		bytesleft -= n;
	}
	if (n < 0)
	{
		std::string err(strerror(errno));
		throw SendException("sendall error: " + err);
	}
}

void Socket::recvall(uint8_t *buf, size_t len)
{
	size_t total = 0;
	size_t bytesleft = len;
	ssize_t n;

	std::lock_guard<std::mutex> lock(_recv);
	while (bytesleft > 0)
	{
		if ((n = recvtimeoutwrapper(socket_descriptor, buf + total, bytesleft, 0)) < 0)
		{
			if (errno == EINTR)
				n = 0;
			else
			{
				n = -1;
				break;
			}
		}
		else if (n == 0)
			break;
		total += n;
		bytesleft -= n;
	}
	if (n < 0)
	{
		std::string err(strerror(errno));
		throw RecvException("recvall error: " + err);
	}
	if (n == 0)
	{
		throw SocketConnectionClosedException("Connection has been closed");
	}
}

void Socket::recvuntil(uint8_t *buf, size_t buflen, const uint8_t *pattern, size_t patternlen, size_t *len)
{
	size_t total = 0;
	ssize_t bytesleft = buflen;
	ssize_t n;

	std::lock_guard<std::mutex> lock(_recvuntil);
	int patternidx;
	do
	{
		if (bytesleft <= 0)
		{
			throw std::overflow_error("recvuntil error: Overflow error");
		}
		if ((n = recvtimeoutwrapper(socket_descriptor, buf + total, bytesleft, MSG_PEEK)) < 0)
		{
			if (errno == EINTR)
				n = 0;
			else
			{
				n = -1;
				break;
			}
		}
		else if (n == 0)
			break;
		if (n > 0)
		{
			patternidx = isContainPattern(buf, total + n, pattern, patternlen);
			if (patternidx < 0)
			{
				recvall(buf + total, n);
			}
			else
			{
				n = patternidx - (total - 1);
				recvall(buf + total, n);
			}
		}
		total += n;
		bytesleft -= n;
	} while (patternidx < 0);
	if (n < 0)
	{
		std::string err(strerror(errno));
		throw RecvException("recvall error: " + err);
	}
	if (n == 0)
	{
		throw SocketConnectionClosedException("Connection has been closed");
	}
	*len = total;
}

int Socket::isContainPattern(const uint8_t *buf, size_t len, const uint8_t *pattern, size_t patternlen)
{
	int notfound = -1;
	if (len < patternlen)
		return notfound;
	else if ((len == 0) && (patternlen == 0))
		return notfound;
	else if (patternlen == 0)
		return notfound;
	for (size_t i = 0, j = 0; i < len; ++i)
	{
		if (buf[i] == pattern[j])
		{
			if (j == patternlen - 1)
				return i;
			else
				++j;
		}
		else
			j = 0;
	}
	return notfound;
}

int Socket::recvtimeoutwrapper(int socket, void *buf, size_t len, int flags)
{
	if (timeout > 0)
	{
		int n = recvtimeout(socket, buf, len, flags, timeout);
		if (n == -2)
			throw TimeoutException("Waiting time has been exceeded");
		return n;
	}
	return recv(socket, buf, len, flags);
}

int Socket::recvtimeout(int socket, void *buf, size_t len, int flags, int timeout)
{
	fd_set fds;
	int n;
	struct timeval tv;

	FD_ZERO(&fds);
	FD_SET(socket, &fds);

	tv.tv_sec = timeout;
	tv.tv_usec = 0;

	n = select(socket + 1, &fds, NULL, NULL, &tv);
	if (n == 0)
		return -2; // timeout
	else if (n == -1)
		return -1;

	return recv(socket, buf, len, flags);
}