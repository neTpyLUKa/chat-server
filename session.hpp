#pragma once

#include <string>
#include <memory>

#include "boost.hpp"
#include "socket_reader.hpp"
#include "socket_writer.hpp"
#include "message_parser.hpp"

class Server;

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(io_context& service, Server* server);

    void Start();
    
    void SendToServer(MsgData msg);

    void SendToClient(MsgData msg);

    tcp::socket& Socket();

    void Erase();

private:
    beast::tcp_stream stream_;

    SocketReader reader_;

    SocketWriter writer_;

    Server* server_;
};
