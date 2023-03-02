#include "command.h"

namespace command
{
    invalid_arguments::invalid_arguments( const char* message )
        :   m_message(message)
    {}

    const char* invalid_arguments::what() const throw()
    {
        return m_message;
    }

    command::command( std::shared_ptr<network::server> server )
    {
        if ( !server )
            throw std::runtime_error("Server is not found");
        m_server = server;
    }

    void disconnect::disconnect( std::shared_ptr<network::server> server )
        :   command(server)
    {}

    void disconnect::validate( std::shared_ptr<network::client> client, const argument_list& arguments )
    {
        if ( arguments.size() != 1 )
            throw invalid_arguments("Usgae: \n\tdisconnect");
    }


    void disconnect::execute( std::shared_ptr<network::client> client, const argument_list& arguments )
    {
        if ( !client )
            throw std::runtime_error("Client is not found");
        m_server->get_disconnection( client->get_fd() );
        client->reply("Disconnecting...");
    }

    shell::shell( network::server* server )
        :   command(server)
    {}

    void shell::validate( std::shared_ptr<network::client> client, const argument_list& arguments )
    {
        if ( arguments.size() != 2 )
            throw invalid_arguments("Usage: \n\tshell <cmd>");
    }

    void shell::execute( std::shared_ptr<network::client> client, const argument_list& arguments )
    {
        if ( !client )
            throw std::runtime_error("Client is not found");
        system( arguments[0] + " "s + arguments[1] );
    }


}




