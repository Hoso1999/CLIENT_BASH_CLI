#ifndef COMMAND_H
#define COMMAND_H

#include "application.h"
#include "client.h"
#include "connection_socket.h"

namespace command
{
    struct invalid_arguments : std::exception
    {
        const char* m_message;
        invalid_arguments(const char* /* message */ );
        const char* what() const noexcept;
    };

    /*! class command
     *
     *  structure for commands
     *  every command should inherit from this structure
     */
    class command
    {
        protected:
            typedef std::vector<std::string> argument_list;

        public:
            explicit command( std::shared_ptr<network::connection_socket> /* server */ );
            /*
             * function for validate specified command
            */
            virtual void validate( std::shared_ptr<network::client> /* client */, const argument_list& /* arguments */ ) = 0;
            /*
             * function for execute specified command
             * not works if validation failed
            */
            virtual void execute( std::shared_ptr<network::client> /* client */, const argument_list& /* arguments */, bool& /* connected */ ) = 0;

        protected:
            std::shared_ptr<network::connection_socket> m_server;
    };


    class disconnect : public command
    {
        public:
            disconnect( std::shared_ptr<network::connection_socket> /* server */ );
            void validate( std::shared_ptr<network::client> /* client */, const argument_list& /* arguments */ ) final;
            void execute( std::shared_ptr<network::client> /* client */, const argument_list& /* arguments */, bool& /* connected */ ) final;
    };

    class shell : public command
    {
        public:
            shell( std::shared_ptr<network::connection_socket> /* server */ );
            void validate( std::shared_ptr<network::client> /* client */, const argument_list& /* arguments */ ) final;
            void execute( std::shared_ptr<network::client> /* client */, const argument_list& /* arguments */, bool& /* connected */ ) final;
    };

    class connect : public command
    {
        public:
            connect( std::shared_ptr<network::connection_socket> /* server */ );
            void validate( std::shared_ptr<network::client> /* client */, const argument_list& /* arguments */ ) final;
            void execute( std::shared_ptr<network::client> /* client */, const argument_list& /* arguments */, bool& /* connected */ ) final;

    };

}





#endif
