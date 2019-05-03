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

    Socket* serv_socket = Socket::createSocket(SOCK_STREAM); 
    serv_socket->_bind(new Address(port));
    serv_socket->_listen(5);

    std::cout<<"Listening on "<<port<<"..."<<std::endl;

    while(1) {
        Socket* client_socket = serv_socket->_accept();
        std::cout<<"New connection from: " + client_socket->getRemoteAddress().getIP()<<std::endl;
        sleep(5);
        delete client_socket;
    }

    delete serv_socket;

    return 0;
}