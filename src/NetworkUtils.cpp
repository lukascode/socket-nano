#include "NetworkUtils.h"

std::string NetworkUtils::getLocalHostName()
{
	char buf[256];
	if (gethostname(buf, 256) < 0)
	{
		std::string err(strerror(errno));
		throw std::runtime_error(("gethostname error: " + err).c_str());
	}
	return std::string(buf);
}

std::string NetworkUtils::getHostByName(std::string name)
{
	hostent *h = gethostbyname(name.c_str());
	if (!h)
	{
		std::string err(hstrerror(h_errno));
		throw DnsLookupException("gethostbyname error: " + err);
	}
	return std::string(inet_ntoa(*((struct in_addr *)h->h_addr)));
}

void NetworkUtils::print_stdout(std::string message)
{
	static std::mutex mtx;
	mtx.lock();
	std::cout << message;
	mtx.unlock();
}
