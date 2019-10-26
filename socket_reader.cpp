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
    BOOST_LOG_TRIVIAL(debug) << buffer_.max_size() << ' ' << buffer_.capacity();
    async_read(stream_, buffer_, transfer_exactly(5), [&](const error_code& ec, size_t bytes) {
        if (ec) {
            BOOST_LOG_TRIVIAL(error) << "Error reading from socket";
            session_->Erase();
            return;
        }
        BOOST_LOG_TRIVIAL(debug) << "msg read, msg: " << beast::buffers_to_string(buffer_.data());
        OnRead();
    });
}

void SocketReader::OnRead() {
    session_->SendToServer(std::make_shared<std::string>(beast::buffers_to_string(buffer_.data())));
    Read();
}
