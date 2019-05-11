#pragma once

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
#include <cstdint>

#include <string>
#include <iostream>
#include <sstream>
#include <mutex>

class NetworkUtils
{
public:
	static std::string GetLocalHostName();
	static std::string GetHostByName(std::string name);
	static void PrintStdout(std::string message);
};

class DnsLookupException : public std::runtime_error
{
public:
	DnsLookupException(std::string msg) : std::runtime_error(msg) {}
};