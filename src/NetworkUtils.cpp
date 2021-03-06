#include "NetworkUtils.h"

std::string NetworkUtils::GetLocalHostName()
{
	char buf[256];
	if (gethostname(buf, 256) < 0)
	{
		std::string err(strerror(errno));
		throw std::runtime_error(("gethostname error: " + err).c_str());
	}
	return std::string(buf);
}

std::string NetworkUtils::GetHostByName(std::string name)
{
	int status, err;
	struct hostent h;
	struct hostent *result;
	char buf[1024];
	status = gethostbyname_r(name.c_str(), &h, buf, sizeof(buf), &result, &err);
	if (status != 0 || !result)
	{
		std::string err(hstrerror(h_errno));
		throw DnsLookupException("gethostbyname_r error (name: ) " + name + "): " + err);
	}
	return std::string(inet_ntoa(*((struct in_addr *)(&h)->h_addr)));
}

void NetworkUtils::PrintStdout(std::string message)
{
	static std::mutex mtx;
	mtx.lock();
	std::cout << message;
	mtx.unlock();
}
