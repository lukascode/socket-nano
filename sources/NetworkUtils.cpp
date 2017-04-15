#include "NetworkUtils.h"

std::string NetworkUtils::getLocalHostName() {
	char buf[BSIZE];
	gethostname(buf, BSIZE);
	std::string lname(buf);
	return lname;
}

int NetworkUtils::getHostByName(std::string name, std::string* ip_str) {
	struct hostent* h;
	if( (h = gethostbyname(name.c_str())) == NULL ) { return -1; }
	*ip_str = inet_ntoa(*((struct in_addr*)h->h_addr));
	return 0;
}

void NetworkUtils::print_stdout(std::string message) {
	static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_lock(&mutex);
	std::cout<<message;
	pthread_mutex_unlock(&mutex);
}