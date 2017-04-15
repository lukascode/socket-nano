CC = g++
OBJS = 	NetworkUtils.o \
		Address.o \
	   	Socket.o \
	   	Server.o \
	   	TcpServer.o \
	   	Client.o \
	   	ServerConnectionHandler.o \
	   	ClientRequestHandler.o \
	   	UdpServer.o

LFLAGS = -pthread
CFLAGS = -ggdb -std=c++11

CLIENT =  klient
SERVER = serwer
TEST = serv
LIBNAME = libsockets.a

all: $(CLIENT) $(SERVER)

lib: $(OBJS)
	ar rcs $(LIBNAME) $^

$(CLIENT): klient.o $(OBJS)
	$(CC) $^ -o $@ $(LFLAGS)

$(SERVER): serwer.o $(OBJS)
	$(CC) $^ -o $@ $(LFLAGS)

$(TEST): serv.o $(OBJS)
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
ClientRequestHandler.o: ClientRequestHandler.h
UdpServer.o: UdpServer.h

clean:
	rm -f *.o

.PHONY: clean all lib
