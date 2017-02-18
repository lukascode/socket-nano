#ifndef NETWORKUTILS_H
#define NETWORKUTILS_H

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

#define BSIZE 256
#define MAXHDRSIZE 8192

class NetworkUtils {

public:

	static std::string getLocalHostName();

	static int getHostByName(std::string name, std::string* ip_str);

	static void print_stdout(std::string message);


};

#endif /* NETWORKUTILS_H */