#include "Socket.h"
#include "Address.h"
#include "NetworkUtils.h"
#include <cstdlib>
#include<iostream>

int main(int argc, char** argv) {

    if(argc != 2) {
        printf("Incorrect number of parameters. Provide port number\n");
        exit(1);
    }

    const short port = atol(argv[1]);

    // Socket serv_socket = Socket::createSocket(SOCK_STREAM); 

    // Address serv_addr = Address("0.0.0.0", port);

    // bind(serv_socket.getSocket(), )


    std::string lname = NetworkUtils::getLocalHostName();

    std::cout<<lname<<std::endl;

    Address address("wp123xyz.pl", 80);


    return 0;
}