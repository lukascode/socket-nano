#include "catch.hpp"
#include "../socknano.h"

TEST_CASE("should create address with port provided", "[address]") {

    Address addr(8080);

    REQUIRE(addr.GetPort() == 8080);
    REQUIRE(addr.GetIP() == "0.0.0.0");
    REQUIRE(addr.ToString() == "0.0.0.0:8080"); 
}

TEST_CASE("should create address with ip and port provided", "[address]") {
    
    Address addr("192.168.1.1", 80);

    REQUIRE(addr.GetPort() == 80);
    REQUIRE(addr.GetIP() == "192.168.1.1");
    REQUIRE(addr.ToString() == "192.168.1.1:80"); 
}

TEST_CASE("should create address by hostname and port provided", "[address]") {
    Address addr("localhost", 80);

    REQUIRE(addr.GetPort() == 80);
    REQUIRE(addr.GetIP() == "127.0.0.1");
    REQUIRE(addr.ToString() == "127.0.0.1:80"); 
}

TEST_CASE("should create address by copy constructor", "[address]") {

    Address addr1(1234);

    Address addr2(addr1);

    REQUIRE(addr2.GetPort() == 1234);
    REQUIRE(addr2.GetIP() == "0.0.0.0");
    REQUIRE(addr2.ToString() == "0.0.0.0:1234"); 
    REQUIRE(addr1.GetRawAddress() != addr2.GetRawAddress());
}