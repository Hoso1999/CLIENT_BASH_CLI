#include "connection_socket.h"



namespace network
{

    connection_socket::connection_socket( const std::string& port, const std::string& host )
        :   m_port( port ),
            m_host( host ),
            m_socket(-1)
    {}

    connection_socket::~connection_socket()
    {
        if ( m_socket > 0 )
            close(m_socket);
    }

    connection_socket::connection_socket( std::string&& port, std::string&& host )
        :   m_port( std::move( port ) ),
            m_host( std::move( host ) ),
            m_socket(-1)
    {}

    void    connection_socket::get_disconnection( int /* fd */ ) {}

    void    connection_socket::connect( const std::string& /* port */, const std::string& /* host */ ) {}
}
