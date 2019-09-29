#pragma once

#include <thread>
#include <algorithm>
#include "Address.h"
#include "TcpConnectionHandler.h"
#include "ThreadPool.h"
#include <functional>
#include "NanoException.h"

class TcpServer : public std::enable_shared_from_this<TcpServer>
{
public:
	/// Creates tcp server
	static std::shared_ptr<TcpServer> Create(std::function<std::shared_ptr<TcpConnectionHandler>()> connHandlerFactory);

	~TcpServer();

	/// Bind to all interfaces on the provided port and listen on incoming connections
	void Listen(uint16_t port);

	/// Bind to interface provided by ip on the provided port
	void Listen(std::string ip, uint16_t port);

	/// Check whether the server is already in listen mode
	bool IsListening();

	/// Disconnect client socket
	bool Disconnect(std::shared_ptr<Socket> client);

	/// Sends data to all clients
	void Broadcast(std::string &data) const;

	/// Sends data to all clients except provided socket
	void Broadcast(std::string &data, std::shared_ptr<Socket> socket) const;

	/// Sets the number of threads in the pool which are used for handling incoming connections
	void SetThreadPoolSize(int size);

	/// Gets the number of tcp connections
	size_t GetNumberOfConnections();

	/// Stops tcp server
	void Stop();

private:
	static const int defaultThreadPoolSize = 20;
	int tpSize;
	std::shared_ptr<ThreadPool> tp;
	std::shared_ptr<Socket> socket;
	std::vector<std::shared_ptr<Socket>> clients;
	std::function<std::shared_ptr<TcpConnectionHandler>()> connHandlerFactory;
	uint16_t port;
	std::string ip;
	std::atomic<bool> halted;
	std::atomic<bool> listening;

	void _Listen();

	void Clean();

	TcpServer(std::function<std::shared_ptr<TcpConnectionHandler>()> connHandlerFactory);
};

class TcpServerException : public NanoException
{
public:
	TcpServerException(std::string msg) : NanoException(msg) {}
};