#ifndef SERVER_H
#define SERVER_H

#include "application.h"
#include "connection_socket.h"
#include "client.h"

namespace network
{
    /*! class server
     *  
     *  server side structure
     */
    class server : public connection_socket
    {
        /*
         * containers for store clients and poll file descriptors
        */
        typedef std::unordered_map<int, std::shared_ptr<client>>    client_map;
        typedef std::vector<pollfd>                                 pollfd_map;
        /*
         * iterators for clients and poll fd
        */
        typedef pollfd_map::iterator                pollfd_iterator;
        typedef client_map::iterator                client_iterator;

        client_map                                  m_clients;
        pollfd_map                                  m_pollfds;

        
        void            get_connection();
        void            get_command( int /* fd */ );


        public:

            server() = delete;
            server( server&  /* other */ ) = delete;
            server( server&& /* other */ ) = delete;
            server& operator=( server&& /* other */ ) = delete;
            server& operator=( server& /* other */ ) = delete;
            static std::shared_ptr<connection_socket>  get_server( const std::string& /* port */, const std::string& /* host */ = "127.0.0.1" );
            virtual void    get_disconnection( int /* fd */ ) override;
            virtual void    run() override;
            int             get_fd() const override;
        protected:

            server( const std::string& /* port */, const std::string& /* host */ = "127.0.0.1" );
            server( std::string&& /* port */, std::string&& /* host */ = "127.0.0.1" );

            virtual int         create_socket() override;
    };
    

}




#endif
