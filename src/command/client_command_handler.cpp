#include "client_command_handler.h"


namespace command
{

    client_command_handler::client_command_handler( std::shared_ptr<network::connection_socket> client )
        :   command_handler( client )
    {
        m_server = client;
        m_commands["connect"] = std::make_shared<connect>( client );
        m_commands["disconnect"] = std::make_shared<disconnect>( client );
    }

    void client_command_handler::invoke( std::shared_ptr<network::client> client, 
                                         const std::string&  message, 
                                         bool& connected)
    {

        auto send_line = [&connected]( int fd, std::string msg )
        {
            if ( connected )
            {
                char        buffer[100];

                bzero(buffer, 100);

                while ( msg.size() )
                {
                    bzero(buffer, 100);
                    int n = sprintf(buffer, "%.*s", 99, msg.c_str());
                    msg.erase(0, 99);
                    if ( send(fd, buffer, 99, 0) < 0)
                        throw std::runtime_error("Cannot send data to server");
                }
            }
        };

        auto get_line = [&connected]( int fd )
        {
            std::string line;
            if ( connected )
            {
                char buffer[100];

                bzero( buffer, 100 );

                while ( !std::strstr(buffer, "\n") )
                {
                    bzero( buffer, 100 );
                    ssize_t num_read = recv( fd, buffer, 99, 0 );
                    if ( num_read < 0 )
                    {
                        if (errno != EWOULDBLOCK )
                            std::runtime_error("Cannot recieve data from server");
                    }
                    else if ( num_read == 0 )
                    {
                        // Connection closed by the server
                        std::cout << ("Server closed the connection");
                        connected = false;
                    }
                    else
                        line.append(buffer);
                }
            }
            return line;
        };

        if ( !client )
            throw std::runtime_error("Client is not found");
        std::string line;
        std::stringstream ssMessage(message);

        while ( std::getline( ssMessage, line) )
        {
            line = line.substr(0, line[ line.length() - 1 ] == '\r' ? line.length() - 1 : line.length() );

            std::string cmd = line.substr(0, line.find(' ') );
            try
            {
                if ( !connected && cmd != "connect" )
                    throw std::runtime_error("Client not connected to server");
                decltype(auto) command = m_commands.at(cmd);
                std::vector<std::string> arguments;
                std::string arg;
                std::stringstream line_stream( line.substr( cmd.length(), line.length() ) );
                while ( line_stream >> arg )
                    arguments.push_back(arg);
                command->validate( client, arguments);
                command->execute( client, arguments, connected );
            }
            catch ( const std::out_of_range& )
            {
                send_line( client->get_fd(), message );
                auto recieved_msg = get_line( client->get_fd() );
                std::cout << recieved_msg;
                sync();
            }
            catch ( const std::exception& ex )
            {
                network::log( std::string( ex.what() ) );
            }
        }

    }

}
