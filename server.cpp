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
    static std::vector<Session*> to_rm;
 
    BOOST_LOG_TRIVIAL(debug) << "Sending...";

    std::vector<std::weak_ptr<Session>> v;

    {
        std::lock_guard<std::mutex> lock(write_mutex_);
        for (auto p : to_rm) {
            sessions_.erase(p);
        }
        v.reserve(sessions_.size());
        for(auto& [p, id] : sessions_) {
            if (p != exclude) {
                v.emplace_back(p->weak_from_this());
            }
        }
    }

    for(auto const& wp : v) {
        if (auto sp = wp.lock()) {
            BOOST_LOG_TRIVIAL(debug) << "Send called";
            sp->SendToClient(msg);
        } else {
            to_rm.push_back(sp.get());
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
    sessions_[client.get()] = id_++;
    client->Start();
    Accept();
}
