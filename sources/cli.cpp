#include <iostream>

#include "Client.h"
#include "ClientRequestHandler.h"
#include <thread>

class TcpRequestHandler : public ClientRequestHandler {
public:
	virtual int handleConnection() {
		std::vector<uint8_t> req_data;
		std::thread rec(&TcpRequestHandler::receive, this);
		char buf[BSIZE];
		while(1) {
			fgets(buf, BSIZE, stdin);
			int len = strlen(buf);
			req_data.assign(buf, buf + len);
			socket->sendall(req_data, &len);
		}
		return 0;
	}
	void receive() {
		std::vector<uint8_t> data;
		while(1) {
			int ret = socket->recvall(data, 64);
			if(ret < 0) {
				printf("Server closed connection\n");
				exit(0);
			}
			printf("%s", data.data());
		}
	}
};

int main(int argc, char* argv[]) {

	if(argc != 3) {
		printf("Incorrect number of parameters\n");
		printf("%s <host> <port>\n", argv[0]);
		exit(1);
	}

	int port = atoi(argv[2]);

	TcpRequestHandler* handler = new TcpRequestHandler();
	Client* client;

	try {
		client = new Client(Address(argv[1], port), 
								SOCK_STREAM, handler);
		client->Connect();
	} catch(const std::exception& e) {
		printf("%s\n", e.what());
	}

	return 0;
}
