#pragma once

#include<thread>
#include<functional>
#include "Socket.h"
#include "ThreadPool.h"
#include "UdpDatagramHandler.h"

class UdpServer
{
public:

	/// Creates udp server 
	UdpServer(std::function<UdpDatagramHandler*()> datagramHandlerFactory);
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
	ThreadPool* tp;
	int tpSize;
	Socket *socket;
	uint16_t port;
	std::string ip;
	std::function<UdpDatagramHandler*()> datagramHandlerFactory;
	std::atomic<bool> halted;
	std::atomic<bool> listening;
	
	void _Listen();

	void Clean();
};

class UdpServerException: public NanoException
{
public:
	UdpServerException(std::string msg): NanoException(msg) {}
};