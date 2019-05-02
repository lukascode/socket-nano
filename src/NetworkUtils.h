#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>
#include <dirent.h>
#include <pthread.h>
#include <math.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#include <string>
#include <iostream>
#include <sstream>
#include <mutex>

#define BSIZE 256
#define MAXBSIZE 8192
#define MAXHDRSIZE 8192

class NetworkUtils 
{
public:
	static std::string getLocalHostName();
	static std::string getHostByName(std::string name);
	static void print_stdout(std::string message);
};

class DnsLookupException: public std::runtime_error {
public:
	DnsLookupException(std::string msg): std::runtime_error(msg) {}
};

#endif /* NETWORK_UTILS_H */