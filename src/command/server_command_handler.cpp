#include "server_command_handler.h"


namespace command
{

    server_command_handler::server_command_handler( std::shared_ptr<network::connection_socket> server )
        :   command_handler( server )
    {
        m_server = server;
        m_commands["shell"] = std::make_shared<shell>( server );
    }

    //void server_command_handler::invoke( std::shared_ptr<network::client> client, const std::string&  message )
    //{

    //    if ( !client )
    //        throw std::runtime_error("Client is not found");
    //    std::string line;
    //    std::stringstream ssMessage(message);

    //    while ( std::getline( ssMessage, line) )
    //    {
    //        line = line.substr(0, line[ line.length() - 1 ] == '\r' ? line.length() - 1 : line.length() );

    //        std::string cmd = line.substr(0, line.find(' ') );
    //        try
    //        {
    //            decltype(auto) command = m_commands.at(cmd);
    //            std::vector<std::string> arguments;
    //            std::string arg;
    //            std::stringstream line_stream( line.substr( cmd.length(), line.length() ) );
    //            while ( line_stream >> arg )
    //                arguments.push_back(arg);
    //            command->validate( client, arguments);
    //            command->execute( client, arguments );
    //        }
    //        catch ( const std::out_of_range& )
    //        {
    //            client->reply( std::string("Command ") + cmd + std::string(" not supported") );
    //        }
    //        catch ( const std::exception& ex )
    //        {
    //            client->reply( std::string( ex.what() ) );
    //        }
    //    }

    //}
}
