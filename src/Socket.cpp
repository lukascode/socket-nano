#include "Socket.h"

Socket *Socket::CreateSocket(int type)
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
	SetSocket(socket_descriptor);
	boundAddress = nullptr;
	connectedAddress = nullptr;
	timeout = 0;
}

Socket::~Socket()
{
	if (IsValidDescriptor())
	{
		CloseSocket();
	}
	if (boundAddress)
	{
		delete boundAddress;
	}
	if (connectedAddress)
	{
		delete connectedAddress;
	}
}

int Socket::GetSocket()
{
	return socket_descriptor;
}

void Socket::SetSocket(int socket_descriptor)
{
	this->socket_descriptor = socket_descriptor;
	if (!IsValidDescriptor())
	{
		throw SocketException("Invalid socket descriptor");
	}
}

void Socket::EnableTimeout(int timeout)
{
	if (timeout > 0)
	{
		this->timeout = timeout;
	}
}

void Socket::DisableTimeout()
{
	this->timeout = 0;
}

bool Socket::IsValidDescriptor()
{
	return (fcntl(socket_descriptor, F_GETFD) != -1) || (errno != EBADF);
}

int Socket::GetSocketType()
{
	int type;
	socklen_t length = sizeof(int);
	getsockopt(socket_descriptor, SOL_SOCKET, SO_TYPE, &type, &length);
	return type;
}

void Socket::Bind(Address *address)
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
	if (bind(socket_descriptor, (struct sockaddr *)address->GetRawAddress(), sizeof(struct sockaddr)) < 0)
	{
		std::string err(strerror(errno));
		throw SocketException("bind error: " + err);
	}
	this->boundAddress = address;
}

void Socket::Connect(Address *address)
{
	if (!address)
	{
		throw std::invalid_argument("Param address must not be null");
	}
	if (connect(socket_descriptor, (struct sockaddr *)address->GetRawAddress(), sizeof(struct sockaddr)) < 0)
	{
		std::string err(strerror(errno));
		throw SocketException("connect error: " + err);
	}
	this->connectedAddress = address;
}

void Socket::Listen(int backlog)
{
	if (listen(socket_descriptor, backlog) < 0)
	{
		std::string err(strerror(errno));
		throw SocketException("listen error: " + err);
	}
}

Socket *Socket::Accept()
{
	int socket = accept(socket_descriptor, nullptr, nullptr);
	if (socket < 0)
	{
		std::string err(strerror(errno));
		throw SocketException("accept error: " + err);
	}
	return new Socket(socket);
}

Address Socket::GetRemoteAddress()
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

Address *Socket::GetBoundAddress()
{
	return boundAddress;
}

void Socket::CloseSocket()
{
	if (close(socket_descriptor) < 0)
	{
		std::string err(strerror(errno));
		throw SocketException("close: " + err);
	}
}

void Socket::SendAll(const std::string &data)
{
	SendAll(std::vector<uint8_t>(data.begin(), data.end()));
}

void Socket::SendAll(const std::vector<uint8_t> &data)
{
	SendAll(data.data(), data.size());
}

std::vector<uint8_t> Socket::RecvAll(size_t len)
{
	std::vector<uint8_t> data(len);
	RecvAll(data.data(), data.size());
	return data;
}

std::vector<uint8_t> Socket::RecvUntil(const std::string pattern, size_t maxlen)
{
	return RecvUntil(std::vector<uint8_t>(pattern.begin(), pattern.end()), maxlen);
}

std::vector<uint8_t> Socket::RecvUntil(const std::vector<uint8_t> &pattern, size_t maxlen)
{
	size_t len = 0;
	std::vector<uint8_t> data(maxlen);
	RecvUntil(data.data(), data.size(), pattern.data(), pattern.size(), &len);
	data.resize(len);
	return data;
}

void Socket::SendAll(const uint8_t *buf, size_t len)
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

void Socket::SendTo(Address *address, const std::string &data)
{
	SendTo(address, std::vector<uint8_t>(data.begin(), data.end()));
}

void Socket::SendTo(Address *address, const std::vector<uint8_t> &data)
{
	SendTo(address, data.data(), data.size());
}

void Socket::SendTo(Address *address, const uint8_t *buf, size_t len)
{
	struct sockaddr *addr = (struct sockaddr *)address->GetRawAddress();
	if (sendto(socket_descriptor, buf, len, 0, addr, sizeof(*addr)) < 0)
	{
		std::string err(strerror(errno));
		throw SendException("sendto error: " + err);
	}
}

void Socket::RecvAll(uint8_t *buf, size_t len)
{
	size_t total = 0;
	size_t bytesleft = len;
	ssize_t n;

	std::lock_guard<std::mutex> lock(_recv);
	while (bytesleft > 0)
	{
		if ((n = RecvTimeoutWrapper(buf + total, bytesleft, 0)) < 0)
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

void Socket::RecvUntil(uint8_t *buf, size_t buflen, const uint8_t *pattern, size_t patternlen, size_t *len)
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
		if ((n = RecvTimeoutWrapper(buf + total, bytesleft, MSG_PEEK)) < 0)
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
			patternidx = IsContainPattern(buf, total + n, pattern, patternlen);
			if (patternidx < 0)
			{
				RecvAll(buf + total, n);
			}
			else
			{
				n = patternidx - (total - 1);
				RecvAll(buf + total, n);
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

std::vector<uint8_t> Socket::RecvFrom(Address *&address, size_t len)
{
	std::vector<uint8_t> data(len);
	size_t size = RecvFrom(address, data.data(), data.size());
	data.resize(size);
	return data;
}

size_t Socket::RecvFrom(Address *&address, uint8_t *buf, size_t len)
{
	int n;
	struct sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);
	if ((n = recvfrom(socket_descriptor, buf, len, 0, (struct sockaddr *)&addr, &addrlen)) < 0)
	{
		std::string err(strerror(errno));
		throw RecvException("recvfrom error: " + err);
	}
	address = new Address(addr);
	return n;
}

int Socket::IsContainPattern(const uint8_t *buf, size_t len, const uint8_t *pattern, size_t patternlen)
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

int Socket::RecvTimeoutWrapper(void *buf, size_t len, int flags)
{
	ApplyRecvTimeout();
	return recv(socket_descriptor, buf, len, flags);
}

void Socket::ApplyRecvTimeout()
{
	if (timeout > 0)
	{
		fd_set fds;
		int n;
		struct timeval tv;
		FD_ZERO(&fds);
		FD_SET(socket_descriptor, &fds);
		tv.tv_sec = timeout;
		tv.tv_usec = 0;
		n = select(socket_descriptor + 1, &fds, nullptr, nullptr, &tv);
		if (n == 0)
			throw TimeoutException("Waiting time has been exceeded");
		else if (n == -1)
			throw RecvException("select error: " + std::string(strerror(errno)));
	}
}