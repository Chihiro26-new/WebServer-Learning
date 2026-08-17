#include "EchoHandler.h"
#include "TcpConnection.h"
#include <iostream>
void EchoHandler::onMessage(TcpConnectionPtr conn)
{
    auto& buffer=conn->getInputBuffer();
    std::string msg = buffer.retrieveAllAsString();
    // std::cout<<"size :"<<msg.size()<<std::endl;
    // std::cout << "echo recv: "<< msg<< std::endl;
    conn->sendMsg(msg);
}

void EchoHandler::onClose(TcpConnectionPtr conn)
{
    
}
void EchoHandler::onConnection(TcpConnectionPtr conn)
{

}