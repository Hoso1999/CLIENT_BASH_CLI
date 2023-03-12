#ifndef CLIENT_H
#define CLIENT_H

#include "application.h" 

namespace network
{
    /*! class client
     *  client side structure
     */
    class client
    {

        public:
            client( int /* fd */, const std::string& /* hostname */, int /* port */ );
            virtual ~client();
            int                     get_fd() const;
            int                     get_port() const;
            const std::string&      get_host() const;
            /*
             * function for send data to server or client
            */
            void                    reply( const std::string& /* message */, bool& /* connected */ ) const;
            /*
             * function for get data from server
            */
            std::string             recieve( bool& /* connected */ ) const;

        protected:
            int                 m_fd;
            int                 m_port;
            const std::string   m_host;



    };

}


#endif
