#ifndef CLIENT_H
#define CLIENT_H

#include "NetworkUtils.h"
#include "Address.h"
#include "Socket.h"
#include "ClientRequestHandler.h"
	
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

	
#endif /* CLIENT_H */