CC = g++
OBJS = 	Socket.o \
		Address.o \
		NetworkUtils.o \
		TcpServer.o \
		TcpConnectionHandler.o \
		UdpServer.o \
		UdpDatagramHandler.o \
		ThreadPool.o

TESTOBJS = ./tests/TestRunner.o \
		   ./tests/AddressTest.o \
		   ./tests/SocketTest.o \
		   ./tests/NetworkUtilsTest.o \
		   ./tests/TcpServerTest.o \
		   ./tests/UdpServerTest.o \
		   ./tests/ThreadPoolTest.o

TESTRUNNER = ./tests/TestRunner

LFLAGS = -pthread
CFLAGS = -ggdb -std=c++14 -Wall -Wextra -pedantic

LIBNAME = libsocknano.a

# CHAT
CHAT_EXAMPLE_CLIENT = ./examples/chat/Client
CHAT_EXAMPLE_CLIENT_OBJ = ./examples/chat/Client.o
CHAT_EXAMPLE_SERVER = ./examples/chat/Server
CHAT_EXAMPLE_SERVER_OBJ = ./examples/chat/Server.o

lib: $(OBJS)
	ar rcs $(LIBNAME) $^

test: lib $(TESTOBJS)
	$(CC) $(TESTOBJS) $(LIBNAME) -o $(TESTRUNNER) $(LFLAGS)
	$(TESTRUNNER)

chat: lib $(CHAT_EXAMPLE_CLIENT_OBJ) $(CHAT_EXAMPLE_SERVER_OBJ)
	$(CC) $(CHAT_EXAMPLE_CLIENT_OBJ) $(LIBNAME) -o $(CHAT_EXAMPLE_CLIENT) $(LFLAGS)
	$(CC) $(CHAT_EXAMPLE_SERVER_OBJ) $(LIBNAME) -o $(CHAT_EXAMPLE_SERVER) $(LFLAGS)

./examples/chat/%.o: ./examples/chat/%.cpp
	$(CC) -c $< $(CFLAGS) -o $@

./tests/%.o: ./tests/%.cpp
	$(CC) -c $< $(CFLAGS) -o $@

%.o: %.cpp
	$(CC) -c $< $(CFLAGS)

NetworkUtils.o: NetworkUtils.h
Address.o: Address.h
Socket.o: Socket.h
TcpServer.o: TcpServer.h
TcpConnectionHandler.o: TcpConnectionHandler.h
UdpServer.o: UdpServer.h
UdpDatagramHandler.o: UdpDatagramHandler.h
UdpServer.o: UdpServer.h
ThreadPool.o: ThreadPool.h

clean:
	rm -f *.o $(LIBNAME)
	rm -f ./tests/*.o ./tests/libsocknano.a $(TESTRUNNER)
	rm -f ./examples/chat/*.o $(CHAT_EXAMPLE_CLIENT) $(CHAT_EXAMPLE_SERVER)

.PHONY: clean test lib chat
