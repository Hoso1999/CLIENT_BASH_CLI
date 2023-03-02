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
        close(m_fd);
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


    void client::reply( const std::string& message )
    {
        std::string buffer = message + "\r\n";
        if ( send( m_fd, buffer.c_str(), buffer.length(), 0 ) < 0)
            throw std::runtime_error("Cannot send message to client.");
    }
}
