#include "server.h"



int main(int argc, char *argv[]) try
{
    if (argc != 2)
        throw std::runtime_error("Usage:\n\tserver <PORT>");
    auto server = network::server::get_server(argv[1]);
    server->run();
}
catch ( std::exception& ex )
{
    std::cerr << ex.what() << std::endl;
}
