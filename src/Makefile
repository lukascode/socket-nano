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
		   ./tests/TestCase1.o

TESTRUNNER = ./tests/TestRunner

LFLAGS = -pthread
CFLAGS = -ggdb -std=c++11 -Wall -Wextra -pedantic

LIBNAME = libsocknano.a

lib: $(OBJS)
	ar rcs $(LIBNAME) $^

test: lib $(TESTOBJS)
	$(CC) $(LIBNAME) $(TESTOBJS) $(OBJS) -o $(TESTRUNNER) $(LFLAGS)
	$(TESTRUNNER)

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

.PHONY: clean test lib
