#ifndef CLIENT_COMMAND_HANDLER_H
#define CLIENT_COMMAND_HANDLER_H

#include "command_handler.h"
#include "client_cli.h"


namespace command
{

    /*! class client_command_handler
     *
     *  server side command handler
     */
    class client_command_handler : public command_handler
    {
        client_command_handler( std::shared_ptr<network::connection_socket> /* client */ );
        public:
            client_command_handler( client_command_handler&  /* other */ ) = delete;
            client_command_handler( client_command_handler&& /* other */ ) = delete;
            client_command_handler& operator=( client_command_handler&& /* other */ ) = delete;
            client_command_handler& operator=( client_command_handler& /* other */ ) = delete;

            /*
             * get client side handler function instance
            */
            static std::shared_ptr<command_handler> get_handler( std::shared_ptr<network::connection_socket> client )
            {
                if ( !client )
                    throw std::runtime_error("Client is not found");
                static std::weak_ptr<command_handler> weakInstance;
                static std::mutex mutex;
                std::lock_guard<std::mutex> lock(mutex);
                auto sharedInstance = weakInstance.lock();
                if (!sharedInstance)
                {
                    sharedInstance = std::shared_ptr<command_handler>( new client_command_handler( client ) );
                    weakInstance = sharedInstance;
                }
                return sharedInstance;
            }
            void invoke( std::shared_ptr<network::client> /* current */,
                         const std::string& /* message */, 
                         bool& /* connected */ ) override;
    };
}






#endif
