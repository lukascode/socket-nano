#ifndef TCP_CONNECTION_HANDLER_H
#define TCP_CONNECTION_HANDLER_H

#include "NetworkUtils.h"
#include "Socket.h"

class TcpServer;

class TcpConnectionHandler 
{
public:
	virtual ~TcpConnectionHandler() {}
	virtual void handleConnection()=0;
	void setSocket(Socket* socket);
	void setContext(TcpServer* context);
protected:
	Socket* socket;
	TcpServer* context;
};

#endif /* TCP_CONNECTION_HANDLER_H */