#include "catch.hpp"
#include "../socknano.h"

TEST_CASE("should get host by name", "[util]")
{
    std::string ip = NetworkUtils::GetHostByName("localhost");
    REQUIRE(ip == "127.0.0.1");

    ip = NetworkUtils::GetHostByName(ip);
    REQUIRE(ip == "127.0.0.1");
}

TEST_CASE("google", "[util]")
{
    std::string ip = NetworkUtils::GetHostByName("google.com");
    REQUIRE(ip == "216.58.215.110");
}