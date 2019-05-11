#pragma once

#include "TcpConnectionHandler.h"

class TcpConnectionHandler;

class TcpConnectionHandlerFactory
{
public:
	virtual ~TcpConnectionHandlerFactory() {}
	virtual TcpConnectionHandler *CreateTcpConnectionHandler() = 0;
};