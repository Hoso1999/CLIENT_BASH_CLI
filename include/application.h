#ifndef APPLICATION_H
#define APPLICATION_H


#include <algorithm>
#include <sys/poll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <poll.h>
#include <cstdlib>
#include <unistd.h>
#include <exception>

#ifdef MAX_CONNECTIONS
#undef MAX_CONNECTIONS
#endif
#define MAX_CONNECTIONS 5

namespace network
{
    class   client;
    class   server;
}

namespace command
{
    class   command;
    class   command_handler;
    struct  invalid_arguments;
}


#endif
