#pragma once

#include <string>
#include <memory>
#include <string_view>
#include <optional>

#include "boost.hpp"

enum class Type {
    Ping, N_connections, Send
};

class MsgData {
public:
    MsgData(str_ptr orig);

    string_view GetMsg();
private:
    str_ptr orig_;
    string_view msg_;
    Type type_;
};
