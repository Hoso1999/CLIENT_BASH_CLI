#ifndef SERVER_command_handler_H
#define SERVER_command_handler_H

#include "connection_socket.h"
#include "command_handler.h"


namespace command
{
    /*! class server_command_handler
     *
     *  server side command handler
     */
    class server_command_handler : public command_handler
    {
        server_command_handler( std::shared_ptr<network::connection_socket> /* server */ );
        public:
            server_command_handler( server_command_handler&  /* other */ ) = delete;
            server_command_handler( server_command_handler&& /* other */ ) = delete;
            server_command_handler& operator=( server_command_handler&& /* other */ ) = delete;
            server_command_handler& operator=( server_command_handler& /* other */ ) = delete;

            static std::shared_ptr<command_handler> get_handler( std::shared_ptr<network::connection_socket> server )
            {
                if ( !server )
                    throw std::runtime_error("Server is not found");
                static std::weak_ptr<command_handler> weakInstance;
                static std::mutex mutex;
                std::lock_guard<std::mutex> lock(mutex);
                auto sharedInstance = weakInstance.lock();
                if (!sharedInstance)
                {
                    sharedInstance = std::shared_ptr<command_handler>( new server_command_handler( server  ) );
                    weakInstance = sharedInstance;
                }
                return sharedInstance;
            }
            using command_handler::invoke;
    };

}




#endif
