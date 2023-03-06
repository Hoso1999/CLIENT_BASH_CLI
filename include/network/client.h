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
            void                    reply( const std::string& /* message */ ) const;
            void                    send( int /* serverfd */, const std::string& /* message */ ) const;

        protected:
            int                 m_fd;
            int                 m_port;
            const std::string   m_host;



    };

}


#endif
