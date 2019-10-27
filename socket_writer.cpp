#include "socket_reader.hpp"
#include "boost.hpp"
#include "session.hpp"

SocketWriter::SocketWriter(beast::tcp_stream& stream)
    : stream_(stream) {
}

void SocketWriter::Push(MsgData data) {
    std::lock_guard<std::mutex> guard(q_mutex_);
    q_.push(data);
    if (!is_writing_) {
        Write();
    }
}

void SocketWriter::Write() {
    is_writing_ = true;
    async_write(stream_, buffer(q_.front().GetMsg()), [&](error_code ec, size_t bytes) {
        OnWrite(ec, bytes);
    });
}

void SocketWriter::OnWrite(error_code ec, size_t bytes) {
    if (ec) {
        BOOST_LOG_TRIVIAL(error) << "error delivering msg";
        stream_.socket().shutdown(tcp::socket::shutdown_send, ec);
        return;
    } else {
        assert(q_.front().GetMsg().size() == bytes);
        BOOST_LOG_TRIVIAL(debug) << "msg delivered, size: " << q_.front().GetMsg().size();
        
    }
    std::lock_guard<std::mutex> guard(q_mutex_);
    q_.pop();
    if (!q_.empty()) {
        async_write(stream_, buffer(q_.front().GetMsg()), [&](error_code ec, size_t bytes) {
            OnWrite(ec, bytes);
        });
    } else {
        is_writing_ = false;
    }
}
