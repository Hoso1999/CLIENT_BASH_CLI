#include "server.h"


namespace network
{
    server* server::m_server = nullptr;

    server::server( const std::string& port )
        :   m_port( port ),
            m_host( std::string("127.0.0.1") ),
            m_command_handler( command::command_handler::get_command_handler(this) )
    {
        m_socket = create_socket();
    }

    int server::create_socket()
    {
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);

        if ( sockfd < 0 )
            throw std::runtime_error("Cannot open socket.");
        try
        {
            /*
             * set socket options
            */
            int val = 1;

            if ( setsockopt( sockfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val) ) )
                throw std::runtime_error("Cannot set socket options.");
            if ( fcntl(sockfd, F_SETFL, O_NONBLOCK) == -1 )
                throw std::runtime_error("Cannot set socket to NON-BLOCKING.");

            struct sockaddr_in serv_address = {};

            bzero((char *) &serv_address, sizeof(serv_address));
            serv_address.sin_family = AF_INET;
            serv_address.sin_addr.s_addr = INADDR_ANY;
            /*
             * set port
            */
            serv_address.sin_port = htons(std::stoi(m_port));
            if ( bind( sockfd, (struct sockaddr *) &serv_address, sizeof(serv_address) ) < 0 )
                throw std::runtime_error("Cannot bind socket.");
            if ( listen(sockfd, MAX_CONNECTIONS) < 0 )
                throw std::runtime_error("Cannot listen socket.");
            return sockfd;
        }
        catch (...)
        {
            close(sockfd);
            throw;
        }
        return -1;
    }

    void    server::run()
    {
        pollfd server_fd = {m_socket, POLLIN, 0};

    	m_pollfds.push_back(server_fd);
        log("Server running...");
        while ( true )
        {
            if ( poll( m_pollfds.begin().base(), m_pollfds.size(), -1 ) < 0 )
                throw std::runtime_error("Cannot poll from fd.");
            for ( auto& curr_poll : m_pollfds )
            {
                if ( curr_poll.revents )
                    continue;
                
                if ( ( curr_poll.revents & POLLHUP ) == POLLHUP )
                {
                    get_disconnection(curr_poll.fd);
                    break;
                }

                if ( (curr_poll.revents & POLLIN) == POLLIN )
                {
                    if ( curr_poll.fd == m_socket  )
                    {
                        get_connection();
                        break;
                    }
                    get_command(curr_poll.fd);            
                }
            }
        }
    }


    void    server::get_command( int fd ) try
    {
        client *client = m_clients.at(fd);

        auto read_line = [&fd]( int fd )
        {
            std::string line;
            char        buffer[100];

            bzero(buffer, 100);

            while ( !std::strstr(buffer, "\n") )
            {
                bzero(buffer, 100);
                if ( recv(fd, buffer, 100, 0) < 0)
                    if (errno != EWOULDBLOCK)
                        throw std::runtime_error("Cannot read buffer from client.");
                line.append(buffer);
            }
            return line;
        };
		m_commandHandler->invoke( client, std::invoke(read_line, fd);
    }
    catch ( const std::out_of_range&  ) {}

    void    get_disconnection( int fd ) try
    {

        client *client = m_clients.at(fd);
        client->disconnect();

        char message[1000];

        sprintf( message, "%s:%d has disconnected.", client->get_host().c_str(), client->get_port() );
        log(message);

        m_clients.erase(fd);

        std::remove_if(m_pollfds.begin(), m_pollfds.end(), 
        [&fd]( auto curr_poll ) -> pollfd
        {
            return curr_poll.fd == fd;
        });
        close(fd);
        delete client;
    }
    catch ( const std::out_of_range& ) {}

    void    get_connection()
    {
        int fd;

        sockaddr_in s_address = {};
        socklen_t s_size = sizeof(s_address);
        fd = accept( m_socket, (sockaddr *) &s_address, &s_size );
        if ( fd < 0 )
            throw std::runtime_error("Cannot accept new client.");
        try
        {
            pollfd pollfd = {fd, POLLIN, 0};
            m_pollfds.push_back(pollfd);
            char hostname[NI_MAXHOST];

            if ( getnameinfo( (struct sockaddr *) &s_address, sizeof(s_address), hostname, NI_MAXHOST, NULL, 0, NI_NUMERICSERV ) != 0 )
                throw std::runtime_error("Cannot get hostname on new client.");
            client *client = new client( fd, hostname, ntohs( s_address.sin_port ) );
            m_clients.insert( std::make_pair( fd, client ) );

            char message[1000];

            sprintf( message, "%s:%d has connected.", client->get_host().c_str(), client->get_port() );
            log(message);
        }
        catch (...)
        {
            close(fd);
            throw;
        }

    }

    server::~server()
    {
        if ( m_server )
            delete m_server;
    }

    server::server( std::string&& port )
        :   m_port( std::move(port) ),
            m_host( "127.0.0.1" ),
            m_command_handler( command::command_handler::get_command_handler(this) )
    {
        m_socket = create_socket();
    }

    server* server::get_server( const std::string& port )
    {
        if ( !m_server )
            m_server = new server(port);
        return m_server;
    }


    server* server::get_server( std::string&& port )
    {
        if ( !m_server )
            m_server = new server( std::move(port) );
        return m_server;
    }

}


