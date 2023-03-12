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


    void client::reply( const std::string& message, bool& connected ) const
    {

        auto all_bytes = message.size();
        char        buffer[BUFFER_SIZE];

        bzero(buffer, BUFFER_SIZE);

        std::string msg = message;
        int bytes = 0;
        int sended_bytes = 0;

        /*
         * send data size
        */
        if ( send(m_fd, &all_bytes, sizeof(all_bytes), 0) < 0)
            throw std::runtime_error("Client not connected to server");
        /*
         * send all data
        */
        while ( bytes < all_bytes )
        {
            bzero(buffer, BUFFER_SIZE);
            int n = sprintf( buffer, "%.*s", BUFFER_SIZE - sended_bytes - 1, msg.c_str() );
            msg.erase(0, BUFFER_SIZE - sended_bytes - 1);
            sended_bytes = ::send( m_fd, buffer, BUFFER_SIZE - sended_bytes - 1, 0 );
            if ( sended_bytes < 0 )
                throw std::runtime_error("Client not connected to server");
            bytes += sended_bytes;
        }

    }

    std::string client::recieve( bool& connected ) const
    {
        std::string line;
        if ( connected )
        {
            char buffer[BUFFER_SIZE];

            bzero( buffer, BUFFER_SIZE );
            size_t length;
            
            /*
             * get data size
            */
            ssize_t recieve_bytes = recv( m_fd, &length, sizeof(length), 0 );
            if ( recieve_bytes < 0 )
                throw std::runtime_error("Client not connected to server");
            int bytes = 0;
            /*
             * get all data
            */
            while ( bytes < length )
            {
                bzero( buffer, BUFFER_SIZE );
                ssize_t num_read = recv( m_fd, buffer, BUFFER_SIZE - 1, 0 );
                if ( num_read < 0 )
                {
                    if (errno != EWOULDBLOCK )
                        std::runtime_error("Cannot recieve data from server");
                    break;
                }
                else if ( num_read == 0 )
                {
                    /*
                     * Connection closed by the server
                    */
                    connected = false;
                    throw std::runtime_error("Server closed the connection");
                }
                else
                {
                    bytes += num_read;
                    line.append(buffer);
                }
            }
        }
        return line;
    }
}
