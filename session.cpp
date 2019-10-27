#include <string>
#include <memory>

#include "boost.hpp"
#include "session.hpp"
#include "message_parser.hpp"
#include "server.hpp"

Session::Session(io_context& service, Server* server)
    : stream_(service),
      reader_(stream_),
      writer_(stream_),
      server_(server) {
}

void Session::Start() {
    auto me = shared_from_this();
    reader_.SetSession(me);
    reader_.Start();
}

void Session::SendToServer(MsgData msg) {
    BOOST_LOG_TRIVIAL(debug) << "sending ms to broadcast, size: " << msg.GetMsg().size();
    server_->Send(msg, this);
}

void Session::SendToClient(MsgData msg) {
    writer_.Push(msg);
}

tcp::socket& Session::Socket() {
    return stream_.socket();
}
