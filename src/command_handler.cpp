#include "command_handler.h"


namespace command
{
    command_handler* command_handler::m_command_handler = nullptr;
    command_handler::command_handler( network::server* server )
    {
        m_commands["shell"] = new shell(server);
        m_commands["disconnect"] = new disconnect(server);
    }

    command_handler::~command_handler()
    {
        for ( auto& cmd : m_commands )
            delete cmd.second;
        if ( m_command_handler )
            delete m_command_handler;
    }

    command_handler* command_handler::get_command_handler( network::server* server )
    {
        if ( !server )
            throw std::runtime_error("Server is not found");
        if ( !m_command_handler )
            m_command_handler = new command_handler(server);
        return m_command_handler;
    }

    void command_handler::invoke( network::client* client, const std::string&  message ) try
    {
        if ( !client )
            throw std::runtime_error("Client is not found");
        std::string line;
        std::stringstream ssMessage(message);

        while ( std::getline( ssMessage, line) )
	    {
            line = line.substr(0, line[ line.length() - 1 ] == '\r' ? line.length() - 1 : line.length() );

            std::string cmd = line.substr(0, line.find(' ') );
            command *command = m_commands.at(name);
            std::vector<std::string> arguments;
            
            std::string arg;
			std::stringstream line_stream( line.substr( cmd.length(), line.length() ) );
            while ( line_stream >> arg )
                arguments.push_back(arg);
            command->validate(client, arguments);
            command->execute(client, arguments);
        }
    }
    catch ( const std::out_of_range& ) {}
}
