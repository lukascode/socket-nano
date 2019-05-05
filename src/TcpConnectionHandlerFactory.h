#ifndef TCP_CONNECTION_HANDLER_FACTORY_H
#define TCP_CONNECTION_HANDLER_FACTORY_H

#include "TcpConnectionHandler.h"

class TcpConnectionHandler;

class TcpConnectionHandlerFactory 
{
public:
	virtual ~TcpConnectionHandlerFactory() {}
	virtual TcpConnectionHandler* createTcpConnectionHandler()=0;
};

#endif /* TCP_CONNECTION_HANDLER_FACTORY_H */