#ifndef CLIENT_H
#define CLIENT_H

#include "NetworkUtils.h"
#include "Address.h"
#include "Socket.h"
#include "ClientRequestHandler.h"
#include <exception>
	
class Client {

public:
	Client(Address serverAddress, int type, ClientRequestHandler* reqHandler); //type ~ SOCK_STREAM/SOCK_DGRAM
	~Client();

	int Connect();

	Address* getServerAddress();
	ClientRequestHandler* getReqHandler();
	Socket* getSocket();

private:
	Socket* socket;
	ClientRequestHandler* reqHandler;
	Address* serverAddress;
	int type;
};

class ConnectException : public std::exception {
public:
	ConnectException(std::string msg) {
		this->msg = msg;
	}
	virtual const char* what() const noexcept {
		return msg.c_str();
	}
private:
	std::string msg;
};

	
#endif /* CLIENT_H */