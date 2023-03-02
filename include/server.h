#ifndef SERVER_H
#define SERVER_H

#include "application.h"
#include "command_handler.h"
#include "client.h"

namespace network
{
    /*! class server
     *  
     *  server side structure
     */
    class server
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

        const std::string                           m_port;
        const std::string                           m_host;
        int                                         m_socket;
        client_map                                  m_clients;
        pollfd_map                                  m_pollfds;
        std::shared_ptr<command::command_handler>   m_command_handler;

        server( const std::string& /* port */ );
        server( std::string&& /* port */ );
        
        int     create_socket();
        void    get_connection();
        void    get_command( int /* fd */ );


        public:

            server( server&  /* other */ ) = delete;
            server( server&& /* other */ ) = delete;
            server& operator=( server&& /* other */ ) = delete;
            server& operator=( server& /* other */ ) = delete;
            static std::shared_ptr<server>  get_server( const std::string& /* port */ );
            static std::shared_ptr<server>  get_server( std::string&& /* port */ );
            void                            get_disconnection( int /* fd */ );
            void                            run();
            virtual ~server();

        protected:
            static server*  m_server;


    };
    
    /*
     * function for print messages for server and client
    */
    void log(const std::string& /* message */ );

}




#endif
