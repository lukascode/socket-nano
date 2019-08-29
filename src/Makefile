CC = g++
OBJS = 	Socket.o \
		Address.o \
		NetworkUtils.o \
		TcpServer.o \
		TcpConnectionHandler.o \
		UdpServer.o \
		UdpDatagramHandler.o \
		ThreadPool.o

LFLAGS = -pthread
CFLAGS = -ggdb -std=c++11 -Wall -Wextra -pedantic

LIBNAME = libsocknano.a

lib: $(OBJS)
	ar rcs $(LIBNAME) $^

devtest: main.o $(OBJS)
	$(CC) $^ -o $@ $(LFLAGS)

%.o: %.cpp
	$(CC) -c $< $(CFLAGS)

NetworkUtils.o: NetworkUtils.h
Address.o: Address.h
Socket.o: Socket.h
Server.o: Server.h
TcpServer.o: TcpServer.h
TcpConnectionHandler.o: TcpConnectionHandler.h
UdpServer.o: UdpServer.h
UdpDatagramHandler.o: UdpDatagramHandler.h
Client.o: Client.h
ServerConnectionHandler.o: ServerConnectionHandler.h
ClientConnectionHandler.o: ClientConnectionHandler.h
UdpServer.o: UdpServer.h
ThreadPool.o: ThreadPool.h

clean:
	rm -f *.o
	rm -f socknano.a
	rm -f devtest

.PHONY: clean all lib devtest
