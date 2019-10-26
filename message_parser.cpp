#include "message_parser.hpp"
#include "boost.hpp"

MsgData::MsgData(str_ptr orig)
    : orig_(orig), msg_(orig->data()) {
    BOOST_LOG_TRIVIAL(debug) << "parsing message, start size: " << orig_->size();    
}

string_view MsgData::GetMsg() {
    return msg_;
}
