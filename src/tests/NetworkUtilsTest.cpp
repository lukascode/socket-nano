#include "catch.hpp"
#include "../socknano.h"

TEST_CASE("should get host by name", "[util]")
{
    std::string ip = NetworkUtils::GetHostByName("localhost");
    REQUIRE(ip == "127.0.0.1");

    ip = NetworkUtils::GetHostByName(ip);
    REQUIRE(ip == "127.0.0.1");
}