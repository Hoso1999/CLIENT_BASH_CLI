#include "application.h"
#include "client_cli.h"
#include "client_command_handler.h"
#include "client.h"
#include <thread>
#include <atomic>


namespace network
{
    client_cli::client_cli( const std::string& port, const std::string& host  )
        :   connection_socket( port, host ),
            m_connected ( false ),
            m_serverfd ( -1 )
    {

        m_socket = create_socket();
    }

    client_cli::client_cli( std::string&& port, std::string&& host  )
        :   connection_socket( std::move( port ), std::move( host ) )
    {

        int sockfd = socket(AF_INET, SOCK_STREAM, 0);

        if ( sockfd < 0 )
            throw std::runtime_error("Cannot open socket.");
        m_socket = sockfd;
    }

    int client_cli::create_socket()
    {

        int sockfd = socket(AF_INET, SOCK_STREAM, 0);

        if ( sockfd < 0 )
            throw std::runtime_error("Cannot open socket.");
        return sockfd;
    }

    void client_cli::connect( const std::string& port, const std::string& host )
    {
        if ( !m_connected )
        {
            if ( m_socket > 0 )
                close( m_socket );
            m_socket = create_socket();
        }
        m_port = port;
        m_host = host;
        struct sockaddr_in serv_addr;
        int server_addr_len;
        bzero( &serv_addr, sizeof( serv_addr ) );

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons( std::stoi( m_port.c_str() ) );
        serv_addr.sin_addr.s_addr = htonl( inet_addr( m_host.c_str() ) );
        if ( inet_pton( AF_INET, m_host.c_str(), &serv_addr.sin_addr ) <= 0 )
            throw std::runtime_error("Cannot convert IPv4 and IPv6 addresses");
        if ( ::connect( m_socket, ( struct sockaddr* )&serv_addr, sizeof(serv_addr) ) < 0 )
             if ( errno != EINPROGRESS )
                throw std::runtime_error("Cannot connect to server");
        m_connected = true;
    }


    std::shared_ptr<connection_socket>  client_cli::get_server( const std::string& port, const std::string& host )
    {
        static std::weak_ptr<connection_socket> weakInstance;
        static std::mutex mutex;
        std::lock_guard<std::mutex> lock(mutex);
        auto sharedInstance = weakInstance.lock();
        if ( !sharedInstance )
        {
            sharedInstance = std::shared_ptr<connection_socket>( new client_cli( port, host ) );
            weakInstance = sharedInstance;
        }
        return sharedInstance;
    }

    void        client_cli::get_disconnection( int fd )
    {
        close(fd);
    }

    void client_cli::run() 
    {
        auto read_line = []()
        {
            std::string line;
            std::getline(std::cin, line);
            if ( std::cin.eof() )
                throw "eof";
            return line + "\r\n";
        };



        struct sockaddr_in serv_addr;
        socklen_t len = sizeof( serv_addr );
        struct sockaddr_in curr_client_addr{};
        curr_client_addr.sin_family = AF_INET;
        curr_client_addr.sin_port = 0;
        socklen_t addr_len = sizeof(curr_client_addr);
        if ( getsockname( m_socket, (struct sockaddr*) &curr_client_addr, &addr_len ) < 0)
            throw std::runtime_error("Cannot get currect port");
        m_current = ( std::shared_ptr<client>(new client( m_socket, m_host, ntohs(curr_client_addr.sin_port) ) ) );
        if (fcntl(m_socket, F_SETFL, O_NONBLOCK) < 0)
            throw std::runtime_error("Cannot set F_SETFL flag");
        while ( true )
        {

            try
            {
                std::cout << "Client> ";
                int servfd = getpeername( m_socket, (struct sockaddr*)&serv_addr, &len );
                if ( servfd == -1 )
                    m_connected = false;
                else
                    m_connected = true;
                
                command::command_handler::get_handler<command::client_command_handler>( get_server() )->invoke(m_current, read_line(), m_connected);

            }
            catch ( const std::exception& ex )
            {
                log( ex.what() );
            }
        }

        //FD_CLR(m_socket, &write_fds);
        //FD_CLR(m_socket, &read_fds);
    }

}
