#include "server.hpp"
#include "boost.hpp"
#include <boost/log/trivial.hpp>

Server::Server(io_context& service, int port) 
    : service_(service),
      a_(service),
      ep_(ip::address::from_string("127.0.0.1"), port) {
    }

void Server::Start() {
    a_.open(ep_.protocol());
    a_.set_option(tcp::acceptor::reuse_address(true));
    a_.bind(ep_);
    a_.listen();
    Accept();
}

void Server::Send(MsgData msg, Session* exclude) {
    static std::vector<int> to_rm;
 
    {
        std::lock_guard<std::mutex> guard(clear_mutex_);
        for (int id : to_rm) {
            sessions_.erase(id);
        }
        to_rm.clear();
    }

    BOOST_LOG_TRIVIAL(debug) << "Sending...";
    
    for(auto const& [id, wp] : sessions_) {
        if (auto sp = wp.lock()) {
            if (sp.get() != exclude) {
                BOOST_LOG_TRIVIAL(debug) << "Send called, id = " << id;
                sp->SendToClient(msg);
            }
        } else {
            to_rm.push_back(id);
        }
    }
}

void Server::Accept() {
    auto client = std::make_shared<Session>(service_, this);
    a_.async_accept(client->Socket(), [&, client](const error_code &ec) {
        if (ec) {
            BOOST_LOG_TRIVIAL(error) << "Error accepting new connection: " << ec.message();
        } else {
            BOOST_LOG_TRIVIAL(info) << "Accepted new connection";
            HandleNewConnection(client); 
        }
    });
}

void Server::HandleNewConnection(session_ptr client) {
    sessions_[id_++] = std::weak_ptr<Session>(client);
    client->Start();
    Accept();
}
