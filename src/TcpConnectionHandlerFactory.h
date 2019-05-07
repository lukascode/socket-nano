#pragma once

#include "TcpConnectionHandler.h"

class TcpConnectionHandler;

class TcpConnectionHandlerFactory
{
public:
	virtual ~TcpConnectionHandlerFactory() {}
	virtual TcpConnectionHandler *createTcpConnectionHandler() = 0;
};