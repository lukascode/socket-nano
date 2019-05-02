#include <iostream>
#include "Client.h"
#include <thread>

class TcpClientHandler : public ClientConnectionHandler 
{
public:
	virtual int handleConnection() 
	{
		std::vector<uint8_t> req_data;
		std::thread rec(&TcpClientHandler::receive, this);
		char buf[BSIZE];
		while(1) {
			fgets(buf, BSIZE, stdin);
			int len = strlen(buf);
			req_data.assign(buf, buf + len);
			socket->sendall(req_data, &len);
		}
		return 0;
	}
	void receive() 
	{
		std::vector<uint8_t> data;
		int type = socket->getSockType();
		if(type == SOCK_STREAM) {
			printf("SOCK_STREAM socket\n");
		} else if(type == SOCK_DGRAM) {
			printf("SOCK_DGRAM socket\n");
		}
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

int main(int argc, char* argv[]) 
{

	if(argc != 3) {
		printf("Incorrect number of parameters\n");
		printf("%s <host> <port>\n", argv[0]);
		exit(1);
	}

	int port = atoi(argv[2]);

	TcpClientHandler* handler = new TcpClientHandler();
	Client* client;

	try {
		client = new Client(Address(argv[1], port), 
								SOCK_DGRAM, handler);
		client->Connect();
	} catch(const std::exception& e) {
		printf("%s\n", e.what());
	}

	return 0;
}
