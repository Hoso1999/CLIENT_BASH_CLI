#ifndef command_handler_H
#define command_handler_H

#include "command.h"
#include "client.h"
#include "server.h"
#include "connection_socket.h"
#include <atomic>


namespace command
{
    /*! class command_handler
     *
     *  structure for invoke commands
     */
    class command_handler
    {
        protected:

            /*
             * container for store commands
            */
            typedef std::unordered_map<std::string, std::shared_ptr<command>> command_map;


        public:

            command_handler( std::shared_ptr<network::connection_socket> /* server */ ) {}

            virtual void invoke( std::shared_ptr<network::client> client, const std::string& message, bool& connected )
            {
                if ( !client )
                    throw std::runtime_error("Client is not found");
                std::string line;
                std::stringstream ssMessage(message);

                while ( std::getline( ssMessage, line ) )
                {
                    line = line.substr(0, line.length() );

                    std::string cmd = line.substr(0, line.find(' ') );
                    try
                    {
                        /*
                         * get command by command name
                        */
                        decltype(auto) curr_command = m_commands.at(cmd);
                        std::vector<std::string> arguments;
                        std::string arg;
                        std::stringstream line_stream( line.substr( cmd.length(), line.length() ) );
                        while ( line_stream >> arg )
                            arguments.push_back(arg);
                        curr_command->validate( client, arguments );
                        curr_command->execute( client, arguments, connected );
                    }
                    catch ( const std::out_of_range& )
                    {
                        bool connected = true;
                        client->reply( std::string("Command \"") + cmd + std::string("\" not supported\n"), connected );
                    }
                    catch ( const std::exception& ex )
                    {
                        client->reply( std::string( ex.what() ), connected );
                    }
                }

            }

            template <typename handler>
            static std::shared_ptr<command_handler> get_handler( std::shared_ptr<network::connection_socket> server )
            {
                if ( !std::is_same<handler, command_handler>::value )
                    return handler::get_handler( server );
            }

    protected:

        command_map                                     m_commands;
        std::shared_ptr<network::connection_socket>     m_server;
    };

}




#endif
