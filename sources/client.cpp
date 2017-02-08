#include <iostream>

#include "Client.h"
#include "ClientRequestHandler.h"

class TcpRequestHandler : public ClientRequestHandler {
public:
	virtual int handleConnection() {
		while(1) {
			std::string message;
			std::getline(std::cin, message);
			socket->Send(&message);
		}
		return 0;
	}
};

int main(int argc, char* argv[]) {

	TcpRequestHandler* handler = new TcpRequestHandler();
	Client* client = new Client(Address("127.0.0.1", 8080, IP), SOCK_STREAM, handler);

	int ret = client->Connect();
	if(ret < 0) {
		printf("Connecting to server failed\n");
	}


	return 0;
}