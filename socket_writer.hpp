#pragma once

#include <queue>
#include <mutex>

#include "boost.hpp"

class Session;
class MsgData;

using session_ptr = std::shared_ptr<Session>;

class SocketWriter {
public:
    SocketWriter(beast::tcp_stream& stream);

    void Push(MsgData data);

private:
    void Write();

    void OnWrite(error_code ec, size_t bytes);

    bool is_writing_ = false;

    beast::tcp_stream& stream_;

    std::queue<MsgData> q_;

    std::mutex q_mutex_;
};
