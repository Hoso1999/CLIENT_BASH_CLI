#ifndef CLIENT_CLI_H
#define CLIENT_CLI_H


#include "connection_socket.h"

namespace network
{
    /*! class client_cli
     *
     *  client cli server
     */
    class client_cli : public connection_socket
    {
        public:
            client_cli( const std::string& /* port */ = "", const std::string& /* host */ = "" );
            client_cli( std::string&& /* port */ = "", std::string&& /* host */ = "" );

            client_cli( client_cli&  /* other */ ) = delete;
            client_cli( client_cli&& /* other */ ) = delete;
            client_cli& operator=( client_cli&& /* other */ ) = delete;
            client_cli& operator=( client_cli& /* other */ ) = delete;

            void                                       get_disconnection( int /* fd */ ) override;
            void                                       connect( const std::string& /* port */, const std::string& /* host */ ) override;
            static std::shared_ptr<connection_socket>  get_server( const std::string& /* port */ = "", const std::string& /* host */ = "" );
            void                                       run() override;

        protected:
            int                                        create_socket() override;


            /*
             * for check if socket connected to server or not
            */
            bool                    m_connected;
            int                     m_serverfd;
            std::shared_ptr<client> m_current;
    };
}



#endif
