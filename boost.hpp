#pragma once

#include <boost/log/trivial.hpp>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <memory>
#include <string>

using namespace boost::asio;
using boost::asio::ip::tcp;
using boost::system::error_code;
namespace beast = boost::beast;

using str_ptr = std::shared_ptr<std::string>;
