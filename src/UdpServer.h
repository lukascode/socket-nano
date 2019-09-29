#pragma once

#include <thread>
#include <functional>
#include "Socket.h"
#include "ThreadPool.h"
#include "UdpDatagramHandler.h"

class UdpServer : public std::enable_shared_from_this<UdpServer>
{
public:
	/// Creates udp server
	static std::shared_ptr<UdpServer> Create(std::function<std::shared_ptr<UdpDatagramHandler>()> datagramHandlerFactory);

	~UdpServer();

	/// Bind to all interfaces on the provided port and listen on incoming datagrams
	void Listen(uint16_t port);

	/// Bind to interface provided by ip on the provided port
	void Listen(std::string ip, uint16_t port);

	/// Check whether the server is already in listen mode
	bool IsListening();

	/// Sets number of threads in the pool which are used for handling incoming datagrams
	void SetThreadPoolSize(int size);

	void Stop();

private:
	static const int defaultThreadPoolSize = 20;
	std::shared_ptr<ThreadPool> tp;
	std::shared_ptr<Socket> socket;
	std::function<std::shared_ptr<UdpDatagramHandler>()> datagramHandlerFactory;
	int tpSize;
	uint16_t port;
	std::string ip;
	std::atomic<bool> halted;
	std::atomic<bool> listening;

	void _Listen();

	void Clean();

	UdpServer(std::function<std::shared_ptr<UdpDatagramHandler>()> datagramHandlerFactory);
};

class UdpServerException : public NanoException
{
public:
	UdpServerException(std::string msg) : NanoException(msg) {}
};