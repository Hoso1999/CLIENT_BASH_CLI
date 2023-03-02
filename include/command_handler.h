#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include "command.h"
#include "client.h"
#include "server.h"


namespace command
{
    /*! class command_handler
     *
     *  structure for invoke commands
     */
    class command_handler
    {
        /*
         * container for store commands
        */
        typedef std::unordered_map<std::string, std::shared_ptr<command>> command_map;

        command_handler( std::shared_ptr<network::server> /* server */ );
        public:
            command_handler( command_handler&  /* other */ ) = delete;
            command_handler( command_handler&& /* other */ ) = delete;
            command_handler& operator=( command_handler&& /* other */ ) = delete;
            command_handler& operator=( command_handler& /* other */ ) = delete;
            void invoke( std::shared_ptr<network::client> /* client */, const std::string& /* message */ );
            static std::shared_ptr<command_handler> get_command_handler( std::shared_ptr<network::server> /* server */ );
            virtual ~command_handler();

    protected:
        static std::shared_ptr<command_handler>     m_command_handler;
        command_map                                 m_commands;
    };

}




#endif
