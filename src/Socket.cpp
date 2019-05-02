#include "Socket.h"

Socket Socket::createSocket(int type) 
{
	int socket_descriptor = socket(AF_INET, type, 0);
	if(socket_descriptor < 0) { 
		std::string err(strerror(errno)); 
		throw SocketException("socket: " + err);
	}
	return Socket(socket_descriptor);
}

Socket::Socket(int socket_descriptor) 
{
	setSocket(socket_descriptor);
}

Socket::Socket(const Socket& socket) 
{
	socket_descriptor = socket.socket_descriptor;
}

Socket::~Socket() 
{
	if(isValidDescriptor()) {
		closeSocket();
	}
}

int Socket::getSocket() {
	return socket_descriptor;
}

void Socket::setSocket(int socket_descriptor) 
{
	this->socket_descriptor = socket_descriptor;
	if(!isValidDescriptor()) {
		throw SocketException("Invalid socket descriptor");
	}
}

bool Socket::isValidDescriptor() 
{
	return ( fcntl(socket_descriptor, F_GETFD) != -1 )  || ( errno != EBADF );
}

int Socket::getSocketType() 
{
	int type; 
	socklen_t length = sizeof(int);
	getsockopt(socket_descriptor, SOL_SOCKET, SO_TYPE, &type, &length);
	return type;
}

Address Socket::getRemoteAddress() 
{
	struct sockaddr_in remote_addr;
	socklen_t addrlen = sizeof(struct sockaddr);
	int ret = getpeername(socket_descriptor, (struct sockaddr*)&remote_addr, &addrlen);
	if(ret != -1) {
		return Address(remote_addr);
	} else {
		std::string err(strerror(errno));
		throw SocketException("getpeername: " + err);
	} 
}

void Socket::closeSocket() 
{
	if(close(socket_descriptor) < 0) {
		std::string err(strerror(errno));
		throw SocketException("close: " + err);
	}
}

//-1 error, 0 success
int Socket::sendall(const std::vector<uint8_t>& data, int* sended) 
{
	 return sendall(data.data(), sended);
}

//-1 close, 0 success
int Socket::recvall(std::vector<uint8_t>& data, int size) 
{
	uint8_t* buf = new uint8_t[size];
	int len = size;
	int ret = recvall(buf, &len);
	data.assign(buf, buf + len);
	return ret;
}

//0 - success, -1 - close, -2 - buffer overflow
int Socket::recvuntil(std::vector<uint8_t>& data, const std::vector<uint8_t>& pattern) 
{
	const uint8_t* p = pattern.data();
	uint8_t buf[MAXBSIZE];
	int len = 0;
	int ret = recvuntil(buf, MAXBSIZE, p, pattern.size(), &len);
	data.assign(buf, buf + len);
	return ret;
}

//-1 error, 0 success
int Socket::sendall(const uint8_t* buf, int* len) 
{
	int total = 0;
	int bytesleft = *len;
	int n;

	_send.lock();
	while(total < *len) {
		n = send(socket_descriptor, buf + total, bytesleft, 0);
		if(n == -1) break;
		total += n;
		bytesleft -= n;
	}
	_send.unlock();
	*len = total;
	return n==-1?-1:0;
}

//-1 close, 0 success
int Socket::recvall(uint8_t* buf, int* len) 
{
	int total = 0;
	int bytesleft = *len;
	int n;

	_recv.lock();
	while(total < *len) {
		n = recv(socket_descriptor, buf + total, bytesleft, 0);
		if(n <= 0) { break; }
		total += n;
		bytesleft -= n;
	}
	_recv.unlock();
	*len = total;
	if(n <= 0) return -1;
	return 0;
}

//0 - success, -1 - close, -2 - buffer overflow
int Socket::recvuntil(uint8_t* buf, int maxlen, const uint8_t* pattern, int patternlen, int* len) 
{
	int n;
	uint8_t byte;
	*len = 0;
	if(maxlen <= 0) return -2;
	_recv.lock();
	while(!isContain(buf, *len, pattern, patternlen)) {
		if(*len >= maxlen) { return -2; }
		n = recv(socket_descriptor, &byte, 1, 0);
		if(n <= 0) { return -1; } //socket closed on the other side
		buf[*len] = byte;
		*len += 1;
	}
	_recv.unlock();
	return 0;
}

//1 - yes, 0 - no
int Socket::isContain(const uint8_t* buf, int buflen, const uint8_t* pattern, int patternlen) 
{
	int contains = 0;
	if(buflen < patternlen) return contains;
	else if( (buflen == 0) && (patternlen == 0)) return 1;
	else if(patternlen == 0) return contains;
	int j = 0;
	for(int i=0; i<buflen; ++i) {
		if(buf[i] == pattern[j]) {
			if(j == patternlen-1) { contains = 1; break;}
			else ++j;
		} else j = 0;
	}
	return contains;
}

/* impl from Beej's Guide to Network Programming Using Internet Sockets*/
//-2 - timeout, -1 - error, 0 - connection closed
int Socket::recvtimeout(int s, uint8_t* buf, int len, int timeout) 
{
	fd_set fds;
	int n;
	struct timeval tv;

	//set descriptors
	FD_ZERO(&fds);
	FD_SET(s, &fds);

	//set timeval for timeout
	tv.tv_sec = timeout;
	tv.tv_usec = 0;

	//read for timeout or for data
	n = select(s+1, &fds, NULL, NULL, &tv);
	if(n == 0) return -2; //timeout
	if(n == -1) return -1; //error

	//get received data
	return recv(s, buf, len, 0);
}