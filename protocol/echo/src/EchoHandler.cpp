#include "EchoHandler.h"
#include "TcpConnection.h"

void EchoHandler::onMessage(TcpConnectionPtr conn)
{
    auto& buffer=conn->getInputBuffer();
    std::string msg = buffer.retrieveAllAsString();
    conn->sendMsg(msg);
}

void EchoHandler::onClose(TcpConnectionPtr conn)
{
        
}
void EchoHandler::onConnection(TcpConnectionPtr conn)
{

}