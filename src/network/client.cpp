#include "client.h"



namespace network
{
    client::client( int fd, const std::string& hostname, int port )
        :   m_fd(fd),
            m_port(port),
            m_host(hostname)
    {}


    client::~client()
    {
    }

    const std::string& client::get_host() const
    {
        return m_host;
    }

    int client::get_port() const
    {
        return m_port;
    }

    int client::get_fd() const
    {
        return m_fd;
    }


    void client::reply( const std::string& message ) const
    {

        auto all_bytes = message.size();
        char        buffer[BUFFER_SIZE];

        bzero(buffer, BUFFER_SIZE);

        std::string msg = message + "\r\n";
        int bytes = 0;
        int sended_bytes = 0;
        while ( bytes < all_bytes )
        {
            bzero(buffer, BUFFER_SIZE);
            int n = sprintf( buffer, "%.*s", BUFFER_SIZE - sended_bytes, msg.c_str() );
            msg.erase(0, BUFFER_SIZE - 1);
            sended_bytes = ::send( m_fd, buffer, BUFFER_SIZE - sended_bytes, 0 );
            if ( sended_bytes < 0 )
                throw std::runtime_error("Client not connected to server");
            bytes += sended_bytes;
        }

    }

    void client::send( int serverfd, const std::string& message ) const
    {
        std::string buffer = message + "\r\n";
        if ( ::send( serverfd, buffer.c_str(), buffer.length(), 0 ) < 0 )
            throw std::runtime_error("Client not connected to server");

    }
}
