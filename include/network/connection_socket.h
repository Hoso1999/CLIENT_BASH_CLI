#ifndef CONNECTION_SOCKET_H
#define CONNECTION_SOCKET_H

#include "application.h"


namespace network
{
    /*! class connection_socket
     *
     * interface for server and client side socket connections
     */
    class connection_socket
    {
        public:
            virtual ~connection_socket();
            connection_socket() = delete;
            connection_socket( connection_socket&  /* other */ ) = delete;
            connection_socket( connection_socket&& /* other */ ) = delete;
            connection_socket& operator=( connection_socket&& /* other */ ) = delete;
            connection_socket& operator=( connection_socket& /* other */ ) = delete;
            virtual void    get_disconnection( int /* fd */ );
            virtual void    run() = 0;
            virtual void    connect( const std::string& /* port */, const std::string& /* host */ );
            virtual int     get_fd() const = 0;

        protected:

            connection_socket( const std::string& /* port */, const std::string& /* host */ = "127.0.0.1" );
            connection_socket( std::string&& /* port */, std::string&& /* host */ = "127.0.0.1" );

            virtual int         create_socket() = 0;
            std::string         m_port;
            std::string         m_host;
            int                 m_socket;
    };
}


#endif
