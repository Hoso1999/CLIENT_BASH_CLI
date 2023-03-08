#include "server_command_handler.h"


namespace command
{

    server_command_handler::server_command_handler( std::shared_ptr<network::connection_socket> server )
        :   command_handler( server )
    {
        m_server = server;
        m_commands["shell"] = std::make_shared<shell>( server );
    }

}
