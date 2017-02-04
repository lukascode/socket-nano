#include "Socket.h"

Socket::Socket(int type, int* err) {
	*err = 0;
	descriptor = socket(AF_INET, type, 0);
	if(descriptor < 0) { perror("socket"); *err = -1; }
}

Socket::Socket(int socket_descriptor) {
	descriptor = socket_descriptor;
}

int Socket::getDescriptor() {
	return descriptor;
}

void Socket::setDescriptor(int desc) {
	descriptor = desc;
}

Address Socket::getRemoteAddress(int* err) {
	*err = 0;
	struct sockaddr_in remote_addr;
	socklen_t addrlen = sizeof(struct sockaddr);
	int ret = getpeername(descriptor, (struct sockaddr*)&remote_addr, &addrlen);
	if(ret != -1) {
		return Address(remote_addr);
	} else *err = ret;
}

int Socket::closeSocket() {
	return close(descriptor);
}

//-1 error, 0 - success
int Socket::Send(std::string* data) {
	int len = data->length();
	if(sendall(data->c_str(), &len) == -1) {
		return -1;
	}
	return 0;
}

//-1 close, 0 - success
int Socket::Recv(std::string* data, int nbytes) {
	int len = nbytes;
	char* buf = new char[len+1];
	int ret = recvall(buf, &len);
	buf[len] = '\0';
	std::string received(buf);
	delete buf;
	(*data) = received;
	return ret;
}

//-1 error, 0 success
int Socket::sendall(const char* buf, int* len) {
	int total = 0;
	int bytesleft = *len;
	int n;

	while(total < *len) {
		n = send(descriptor, buf + total, bytesleft, 0);
		if(n == -1) break;
		total += n;
		bytesleft -= n;
	}
	*len = total;
	return n==-1?-1:0;
}

//-1 close, 0 success
int Socket::recvall(char* buf, int* len) {
	int total = 0;
	int bytesleft = *len;
	int n;

	while(total < *len) {
		n = recv(descriptor, buf + total, bytesleft, 0);
		if(n <= 0) { break; }
		total += n;
		bytesleft -= n;
	}
	*len = total;
	if(n <= 0) return -1;
	return 0;
}