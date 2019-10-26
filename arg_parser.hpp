#pragma once

struct ArgData {
    int port;
    int n_threads;
    int timeout_seconds;
};

ArgData ParseArgs(int argc, char* argv[]);
