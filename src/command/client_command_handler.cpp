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
                char        buffer[BUFFER_SIZE];

                bzero(buffer, BUFFER_SIZE);

                auto line_size = msg.size();

                int bytes = 0;
                if ( send(fd, &line_size, sizeof(line_size), 0) < 0)
                    throw std::runtime_error("Cannot send data to server");
                size_t ping;
                if ( recv(fd, &ping, sizeof(ping), 0) < 0 )
                    throw std::runtime_error("Cannot recv data to server");
                std::cout << "ping: " << (ping == 0x01) << "\n";
                while ( bytes < line_size )
                {
                    bzero(buffer, BUFFER_SIZE);
                    int n = sprintf(buffer, "%.*s", BUFFER_SIZE - 1, msg.c_str());
                    msg.erase(0, BUFFER_SIZE - 1);
                    auto send_bytes = send(fd, buffer, BUFFER_SIZE - 1, 0 );
                    if ( send_bytes < 0 )
                        throw std::runtime_error("Cannot send data to server");
                    bytes += send_bytes;
                }
            }
        };

        auto get_line = [&connected]( int fd )
        {
            std::string line;
            if ( connected )
            {
                char buffer[BUFFER_SIZE];

                bzero( buffer, BUFFER_SIZE );
                size_t length;
                
                ssize_t recieve_bytes = recv(fd, &length, sizeof(length), 0);
                std::cout << "len: " <<  length << "\n";
                if ( recieve_bytes < 0 )
                    throw std::runtime_error("Client not connected to server");
                int bytes = 0;
                while ( bytes < length )
                {
                    bzero( buffer, BUFFER_SIZE );
                    ssize_t num_read = recv( fd, buffer, BUFFER_SIZE - 1, 0 );
                    if ( num_read < 0 )
                    {
                        if (errno != EWOULDBLOCK )
                            std::runtime_error("Cannot recieve data from server");
                        break;
                    }
                    else if ( num_read == 0 )
                    {
                        // Connection closed by the server
                        connected = false;
                        throw std::runtime_error("Server closed the connection");
                    }
                    else
                    {
                        bytes += num_read;
                        line.append(buffer);
                    }
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
            }
            catch ( const std::exception& ex )
            {
                network::log( std::string( ex.what() ) );
            }
        }

    }

}
