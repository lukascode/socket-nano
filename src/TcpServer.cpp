#include "TcpServer.h"

static void handleConnection(TcpConnectionHandler* handler);
static void joinFinishedThreads(std::vector<std::thread*>* connections);

TcpServer::TcpServer(short port, TcpConnectionHandlerFactory* connHandlerFactory) 
{
	this->ip = "";
	this->port = port;
	this->connHandlerFactory = connHandlerFactory;
}

TcpServer::TcpServer(std::string ip, short port, TcpConnectionHandlerFactory* connHandlerFactory) 
{
	this->ip = ip;
	this->port = port;
	this->connHandlerFactory = connHandlerFactory;
}

TcpServer::~TcpServer()
{
	if(connHandlerFactory) delete connHandlerFactory;
	if(socket) delete socket;

	for(size_t i=0; i<clients.size(); ++i) {
		if(clients[i]) delete clients[i];
	} clients.clear();
}

void TcpServer::Listen()
{
	socket = Socket::createSocket(SOCK_STREAM);
	Address* address = ip.empty() ? new Address(port) : new Address(ip, port);
	socket->_bind(address);
	socket->_listen(20);

	// join connection threads
	std::thread _wait(joinFinishedThreads, &connections);
	for(;;) {
		Socket* client_socket = socket->_accept();
		clients.push_back(client_socket);
		TcpConnectionHandler* handler = connHandlerFactory->createTcpConnectionHandler();
		handler->setSocket(client_socket);
		handler->setContext(this);

		// handle connection in std::thread
		std::thread* thread = new std::thread(handleConnection, handler);
		connections.push_back(thread); 
	}
	_wait.join();
}

bool TcpServer::removeClient(Socket* client) 
{
	auto it = std::find(clients.begin(), clients.end(), client);
	if(it != clients.end()) {
		clients.erase(it);
		delete client;
		return true;
	}
	return false;
}

static void handleConnection(TcpConnectionHandler* handler) 
{
	handler->handleConnection();
	delete handler;
}

static void joinFinishedThreads(std::vector<std::thread*>* connections) 
{
	for(;;) {
		for(size_t i=0; i<connections->size(); ++i) {
			(*connections)[i]->join();
			delete (*connections)[i];
			connections->erase(connections->begin() + i);
		}
		sleep(1);
	}
}