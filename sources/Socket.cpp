#include "Socket.h"

Socket::Socket(int type, int notsocketdescriptor) {
	descriptor = socket(AF_INET, type, 0);
	if(descriptor < 0) { 
		std::string err(strerror(errno)); 
		throw "socket: " + err;
	}
}

Socket::Socket(int socket_descriptor) {
	descriptor = socket_descriptor;
}

Socket::~Socket() {
	if(isValid()) { closeSocket(); }
}

int Socket::getDescriptor() {
	return descriptor;
}

void Socket::setDescriptor(int desc) {
	descriptor = desc;
}

int Socket::isValid() {
	return ( fcntl(descriptor, F_GETFD) != -1 )  || ( errno != EBADF );
}

Address Socket::getRemoteAddress() {
	struct sockaddr_in remote_addr;
	socklen_t addrlen = sizeof(struct sockaddr);
	int ret = getpeername(descriptor, (struct sockaddr*)&remote_addr, &addrlen);
	if(ret != -1) {
		return Address(remote_addr);
	} else {
		std::string err(strerror(errno));
		throw ("getpeername: " + err).c_str();
	} 
}

int Socket::closeSocket() {
	return close(descriptor);
}

int Socket::sendall(const std::vector<uint8_t>& data, int* sended) {
	int ret = sendall(data.data(), sended);
	return ret;
}

int Socket::recvall(std::vector<uint8_t>& data, int size) {
	uint8_t* buf = new uint8_t[size];
	int len = size;
	int ret = recvall(buf, &len);
	data.assign(buf, buf + len);
	return ret;
}

int Socket::recvuntil(std::vector<uint8_t>& data, const std::vector<uint8_t>& pattern) {
	const uint8_t* p = pattern.data();
	uint8_t buf[MAXBSIZE];
	int len = 0;
	int ret = recvuntil(buf, MAXBSIZE, p, pattern.size(), &len);
	data.assign(buf, buf + len);
	return ret;
}

//-1 error, 0 success
int Socket::sendall(const uint8_t* buf, int* len) {
	int total = 0;
	int bytesleft = *len;
	int n;

	_send.lock();
	while(total < *len) {
		n = send(descriptor, buf + total, bytesleft, 0);
		if(n == -1) break;
		total += n;
		bytesleft -= n;
	}
	_send.unlock();
	*len = total;
	return n==-1?-1:0;
}

//-1 close, 0 success
int Socket::recvall(uint8_t* buf, int* len) {
	int total = 0;
	int bytesleft = *len;
	int n;

	_recv.lock();
	while(total < *len) {
		n = recv(descriptor, buf + total, bytesleft, 0);
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
int Socket::recvuntil(uint8_t* buf, int maxlen, const uint8_t* pattern, int patternlen, int* len) {
	int n;
	uint8_t byte;
	*len = 0;
	if(maxlen <= 0) return -2;
	_recv.lock();
	while(!isContain(buf, *len, pattern, patternlen)) {
		if(*len >= maxlen) { return -2; }
		n = recv(descriptor, &byte, 1, 0);
		if(n <= 0) { return -1; } //socket closed on the other side
		buf[*len] = byte;
		*len += 1;
	}
	_recv.unlock();
	return 0;
}

//1 - yes, 0 - no
int Socket::isContain(const uint8_t* buf, int buflen, const uint8_t* pattern, int patternlen) {
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
