#include "server.h"
#include "server_command_handler.h"
#include <thread>
#include <mutex>


namespace network
{


    server::server( const std::string& port, const std::string& host )
        :   connection_socket( port, host )
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

    int server::get_fd() const
    {
        return m_socket;
    }
    void    server::run()
    {
        pollfd server_fd = {m_socket, POLLIN, 0};

    	m_pollfds.push_back(server_fd);
        log("Server running...");
        while ( true )
        {
            int status = poll( m_pollfds.begin().base(), m_pollfds.size(), -1 );
            if ( status < 0 )
                throw std::runtime_error("Cannot poll from fd.");
            for ( auto& curr_poll : m_pollfds )
            {

                if ( !curr_poll.revents )
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
        auto client = m_clients.at(fd);

        int recieve_bytes = 0;
        size_t length;

        recieve_bytes = recv(fd, &length, sizeof(length), 0);
        if ( recieve_bytes < 0 )
            throw std::runtime_error("Client not connected to server");
        auto read_line = [&]( int fd )
        {
            std::string line;
            char        buffer[BUFFER_SIZE];

            bzero(buffer, BUFFER_SIZE);
            int bytes = 0;
            while ( bytes < length )
            {
                bzero(buffer, BUFFER_SIZE);
                ssize_t r_bytes = recv(fd, buffer, BUFFER_SIZE - 1, 0);
                if ( r_bytes < 0)
                    if (errno != EWOULDBLOCK)
                        throw std::runtime_error("Client not connected to server");
                bytes += r_bytes;
                line.append(buffer);
            }
            return line;
        };
        bool connected = true;
        command::command_handler::get_handler<command::server_command_handler>( get_server( m_port, m_host ) )->invoke( client, std::invoke(read_line, fd), connected );
    }
    catch ( const std::out_of_range&  ) {
        std::cerr << "Command not found" << std::endl;
    }

    void    server::get_disconnection( int fd ) try
    {

        auto client = m_clients.at(fd);

        char message[BUFFER_SIZE];
        
        bzero(message, BUFFER_SIZE);
        sprintf( message, "%s:%d has disconnected.", client->get_host().c_str(), client->get_port() );
        log(message);

        m_clients.erase(fd);

        std::remove_if(m_pollfds.begin(), m_pollfds.end(), 
        [&fd]( auto curr_poll )
        {
            return curr_poll.fd == fd;
        });
        close(fd);
    }
    catch ( const std::out_of_range& ) {}

    void    server::get_connection()
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
            std::shared_ptr<client> new_client( new client( fd, hostname, ntohs( s_address.sin_port ) ) );
            m_clients.insert( std::make_pair( fd, new_client ) );

            char message[BUFFER_SIZE];

            bzero(message, BUFFER_SIZE);
            sprintf( message, "%s:%d has connected.", new_client->get_host().c_str(), new_client->get_port() );
            log(message);
        }
        catch (...)
        {
            close(fd);
            throw;
        }

    }

    server::server( std::string&& port, std::string&& host )
        :   connection_socket( std::move( port ), std::move( host ) )
    {
        m_socket = create_socket();
    }

    std::shared_ptr<connection_socket> server::get_server( const std::string& port, const std::string& host )
    {
        static std::weak_ptr<connection_socket> weakInstance;
        static std::mutex mutex;
        std::lock_guard<std::mutex> lock(mutex);
        auto sharedInstance = weakInstance.lock();
        if ( !sharedInstance )
        {
            sharedInstance = std::shared_ptr<connection_socket>( new server( port, host ) );
            weakInstance = sharedInstance;
        }
        return sharedInstance;
    }

    void log( const std::string& message )
    {
        std::time_t rawtime;
        struct tm *timeinfo;
        char buffer[BUFFER_SIZE];

        std::time(&rawtime);
        timeinfo = std::localtime(&rawtime);

        bzero(buffer, BUFFER_SIZE);
        std::strftime(buffer, sizeof(buffer) - 1, "%d-%m-%Y %H:%M:%S", timeinfo);
        std::string str(buffer);
        std::cout << "[" << str << "] " << message << std::flush << std::endl;
    }

}


