#pragma once

#include "TcpConnectionHandler.h"

class TcpConnectionHandler;

class TcpConnectionHandlerFactory
{
public:
	virtual ~TcpConnectionHandlerFactory() {}

	/// Responsible for creating new tcp connection handlers
	virtual TcpConnectionHandler *CreateTcpConnectionHandler() = 0;
};