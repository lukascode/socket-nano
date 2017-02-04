#include <iostream>
#include "TcpServer.h"
#include "ServerConnectionHandler.h"
#include "NetworkUtils.h"
#include <algorithm>

class TcpConnectionHandler : public ServerConnectionHandler {
public:
	virtual int handleConnection(Socket* socket, Server* context) {
		int err;
		NetworkUtils::print_stdout("New client connected. " + socket->getRemoteAddress(&err).toString() + "\n");
		while(1) {
			std::string data;
			int ret = socket->Recv(&data, 10);
			if(ret == -1) {
				//client close the connection
				NetworkUtils::print_stdout(socket->getRemoteAddress(&err).toString() + ", client close the connections\n");
				context->removeClient(socket);
				pthread_exit(NULL);
			}
			std::string m = socket->getRemoteAddress(&err).toString() + " message: " + data + " l: " + std::to_string(data.length()) + "\n";
			NetworkUtils::print_stdout(m);
		}
	}
};

int main(int argc, char* argv[]) {

	ServerConnectionHandler* handler = new TcpConnectionHandler();
	TcpServer* server = new TcpServer(Address(8080), handler);

	int ret = server->Listen();

	if(ret < 0) {
		printf("server listen error\n");
		exit(1);
	}

	
	return 0;
}