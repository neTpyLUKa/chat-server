#include "arg_parser.hpp"
#include "boost.hpp"
#include "server.hpp"

int parent_id;

int main(int argc, char* argv[]) {
    ArgData data = ParseArgs(argc, argv);
    
    parent_id = getuid();
    
    io_context service;
    Server server(service, data.port);
    server.Start();
    
    signal_set signals(service, SIGINT, SIGTERM);
    signals.async_wait([&](boost::system::error_code const&, int) {
        BOOST_LOG_TRIVIAL(info) << "\nTerminating..." << std::endl;
        service.stop();
    });

    std::vector<std::thread> threads;
    threads.reserve(data.n_threads - 1);
    for(auto i = data.n_threads - 1; i > 0; --i) {
        threads.emplace_back([&]() {
            service.run();
        });
    }
    service.run();
    for(auto& t : threads) {
        t.join();
    }
}
