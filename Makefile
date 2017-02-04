
CC = g++
OBJS = main.o NetworkUtils.o Address.o Socket.o Server.o TcpServer.o Client.o

EXEC = main.out

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ -pthread

%.o: %.cpp
	$(CC) -c -std=c++11 $<

NetworkUtils.o: NetworkUtils.h
Address.o: Address.h
Socket.o: Socket.h
Server.o: Server.h
TcpServer.o: TcpServer.h
Client.o: Client.h

clean:
	rm -f *.o

.PHONY: clean