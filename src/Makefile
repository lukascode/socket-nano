CC = g++
OBJS = 	NetworkUtils.o \
		Address.o \
	   	Socket.o \
	   	Server.o \
	   	TcpServer.o \
	   	Client.o \
	   	ServerConnectionHandler.o \
	   	ClientConnectionHandler.o \
	   	UdpServer.o

LFLAGS = -pthread
CFLAGS = -ggdb -std=c++11 -Wall -Wextra -pedantic

CLIENT =  cli
SERVER = serv
LIBNAME = libsockets.a

all: $(CLIENT) $(SERVER)

lib: $(OBJS)
	ar rcs $(LIBNAME) $^

$(CLIENT): cli.o $(OBJS)
	$(CC) $^ -o $@ $(LFLAGS)

$(SERVER): serv.o $(OBJS)
	$(CC) $^ -o $@ $(LFLAGS)

devtest: main.o Socket.o Address.o NetworkUtils.o
	$(CC) $^ -o $@ $(LFLAGS)

%.o: %.cpp
	$(CC) -c $< $(CFLAGS)

NetworkUtils.o: NetworkUtils.h
Address.o: Address.h
Socket.o: Socket.h
Server.o: Server.h
TcpServer.o: TcpServer.h
Client.o: Client.h
ServerConnectionHandler.o: ServerConnectionHandler.h
ClientConnectionHandler.o: ClientConnectionHandler.h
UdpServer.o: UdpServer.h

clean:
	rm -f *.o
	rm -f cli serv
	rm -f libsockets.a
	rm -f devtest

.PHONY: clean all lib devtest
