#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "boost.hpp"
#include "session.hpp"

class Server {
public:
    Server(io_context& service, int port);
    
    void Start();
 
    void Send(MsgData data, Session* exclude);

private:
    void Accept();
    
    void HandleNewConnection(session_ptr client);

    io_context& service_;

    tcp::acceptor a_;
    
    tcp::endpoint ep_;
       
    std::mutex write_mutex_;
    
    std::unordered_map<int, std::weak_ptr<Session>> sessions_;

    int id_ = 0;

    std::mutex clear_mutex_;
};
