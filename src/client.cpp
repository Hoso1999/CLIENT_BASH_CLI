#include "client_cli.h"

int main() try
{
    try
    {
        auto cli = network::client_cli::get_server();
        cli->run();
    }
    catch ( const char* ex )
    {
        return 0;
    }
}
catch ( const std::exception& ex )
{
    std::cerr << ex.what() << std::endl;
}



