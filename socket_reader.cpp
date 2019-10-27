#include "boost.hpp"
#include "socket_reader.hpp"
#include "boost/log/trivial.hpp"
#include "session.hpp"

SocketReader::SocketReader(beast::tcp_stream& stream)
    : stream_(stream) {
}

void SocketReader::SetSession(session_ptr session) {
    session_ = session;
}

void SocketReader::Start() {
    Read();
}

void SocketReader::Read() {
    async_read_until(stream_.socket(), buffer_, '\n', [&](const error_code& ec, size_t bytes) {
        if (ec) {
            BOOST_LOG_TRIVIAL(error) << "Error reading from socket";
            session_ = nullptr;
            return;
        }
        BOOST_LOG_TRIVIAL(debug) << "Message of " << bytes << " size read";
        OnRead();
    });
}

void SocketReader::OnRead() {
    session_->SendToServer(std::make_shared<std::string>(beast::buffers_to_string(buffer_.data())));
    buffer_.consume(buffer_.size());
    Read();
}
