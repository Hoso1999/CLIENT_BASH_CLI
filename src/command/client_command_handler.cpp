#include "client_command_handler.h"


namespace command
{

    client_command_handler::client_command_handler( std::shared_ptr<network::connection_socket> client )
        :   command_handler( client )
    {
        m_server = client;
        m_commands["connect"] = std::make_shared<connect>( client );
        m_commands["disconnect"] = std::make_shared<disconnect>( client );
        m_commands["shell"] = std::make_shared<cli_shell>( client );
    }

    void client_command_handler::invoke( std::shared_ptr<network::client> client, 
                                         const std::string&  message, 
                                         bool& connected)
    {

        if ( !client )
            throw std::runtime_error("Client is not found");
        std::string line;
        std::stringstream ssMessage(message);

        while ( std::getline( ssMessage, line) )
        {
            line = line.substr(0, line.length() );

            std::string cmd = line.substr(0, line.find(' ') );
            std::vector<std::string> arguments;
            arguments.push_back(cmd);
            try
            {
                decltype(auto) command = m_commands.at(cmd);
                std::string arg;
                std::stringstream line_stream( line.substr( cmd.length(), line.length() ) );
                while ( line_stream >> arg )
                    arguments.push_back(arg);
                command->validate( client, arguments);
                command->execute( client, arguments, connected );
            }
            catch ( const std::out_of_range& )
            {
                std::stringstream msg;
                msg << "Command \"" << cmd << "\" not supported";
                network::log( msg.str() );
            }
            catch ( const std::exception& ex )
            {
                network::log( std::string( ex.what() ) );
            }
        }

    }

}
