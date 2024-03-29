#pragma once

#include <string>
#include <memory>

#include "boost.hpp"

const int BUF_SIZE = 10000;

class Session;

using session_ptr = std::shared_ptr<Session>;

class SocketReader {
public:
    SocketReader(beast::tcp_stream& stream);

    void SetSession(session_ptr session);

    void Start();

private:
    void Read();

    void OnRead();

    beast::tcp_stream& stream_;

    session_ptr session_;

    streambuf buffer_;
};
