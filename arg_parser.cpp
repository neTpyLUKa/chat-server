#include <iostream>
#include <boost/program_options.hpp>
#include <string>

#include "arg_parser.hpp"

namespace po = boost::program_options;

constexpr int DEF_PORT = 5000;
constexpr int DEF_THREADS = 1;
constexpr int DEF_TIMEOUT = 10;

ArgData ParseArgs(int argc, char* argv[]) {
    ArgData data = {DEF_PORT, DEF_THREADS, DEF_TIMEOUT};
    
    po::options_description desc("Options");
    std::string port_msg = "Specify server port, default value is " + std::to_string(DEF_PORT);
    std::string threads_msg = "Specify number of threads, default value is " + std::to_string(DEF_THREADS);
    std::string timeout_msg = "Specify timeout before client disconnection (in seconds), default value is " + std::to_string(DEF_TIMEOUT);
    desc.add_options()
        ("help,h", "Show help")
        ("port,p", po::value<int>(&data.port), port_msg.data())
        ("threads", po::value<int>(&data.n_threads), threads_msg.data())
        ("timeout", po::value<int>(&data.timeout_seconds), timeout_msg.data())
        ;
    
    po::variables_map vm;
    po::parsed_options parsed = po::command_line_parser(argc, argv).options(desc).allow_unregistered().run();
    po::store(parsed, vm);
    po::notify(vm);
    if (vm.count("help")) {
        std::cout << desc;
        exit(1);
    }
    return data;
}
