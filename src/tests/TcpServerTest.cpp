#include "catch.hpp"
#include "../socknano.h"
#include <functional>

class TcpConnectionHandlerImpl : public TcpConnectionHandler
{
public:
    virtual void HandleConnection()
    {
        try
        {
            while(server->isListening()) 
            {
                socket->SendAll("Hello world\n");
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            }
        }
        catch (NanoException &e)
        {
            std::cout<<std::string(e.what())<<std::endl;
        }
    }
};

TEST_CASE("tcp server test", "[tcp-server]")
{   
    // std::cout<<"tcp server test"<<std::endl;
    // TcpServer *server = new TcpServer([] { return new TcpConnectionHandlerImpl(); });

    // std::thread t1([server] {
        
    //     std::cout<<"Start listening"<<std::endl;

    //     server->Listen(4444);

    //     std::cout<<"Stop listening"<<std::endl;

    //     std::this_thread::sleep_for(std::chrono::milliseconds(8*1000));

    //     std::cout<<"Start listening again"<<std::endl;

    //     server->Listen(4444);

    // });
    // t1.detach();

    // std::this_thread::sleep_for(std::chrono::milliseconds(15*1000));

    // std::cout<<"Calling server->Stop()"<<std::endl;
    // server->Stop();


    // std::this_thread::sleep_for(std::chrono::milliseconds(180*1000));

}