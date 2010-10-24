/** 
 @cond
 #########################################################################
 # GPL License                                                           #
 #                                                                       #
 # This file is part of the Machine Learning Framework.                  #
 # Copyright (c) 2010, Philipp Kraus, <philipp.kraus@flashpixx.de>       #
 # This program is free software: you can redistribute it and/or modify  #
 # it under the terms of the GNU General Public License as published by  #
 # the Free Software Foundation, either version 3 of the License, or     #
 # (at your option) any later version.                                   #
 #                                                                       #
 # This program is distributed in the hope that it will be useful,       #
 # but WITHOUT ANY WARRANTY; without even the implied warranty of        #
 # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
 # GNU General Public License for more details.                          #
 #                                                                       #
 # You should have received a copy of the GNU General Public License     #
 # along with this program.  If not, see <http://www.gnu.org/licenses/>. #
 #########################################################################
 @endcond
 **/


#ifndef MACHINELEARNING_TOOLS_NNTP_H
#define MACHINELEARNING_TOOLS_NNTP_H

#include <string>
#include <iostream>
#include <istream>
#include <ostream>
#include <boost/asio.hpp>

#include "../exception/exception.h"
#include "language/language.h"


namespace machinelearning { namespace tools {
    
    namespace bip  = boost::asio::ip;
    
    
    
    /** class for creating nntp connection (exspecially for creating distance matrix).
     * The class is a nntp client class without any post functionality
     * @todo ssl connection
     **/
    class nntp {
        // http://www.boost.org/doc/libs/1_44_0/doc/html/boost_asio/example/http/client/sync_client.cpp
        
        public :
        
            nntp( const std::string&, const std::string& = "nntp" );
            std::string getServer( void ) const;
            std::string getPortProtocoll( void ) const;
            std::vector<std::string> getGroupList( void );

            ~nntp( void );
        
        
        private :
        
            /** string with server name **/
            const std::string m_server;
            /** port or protocoll - should be "nntp" **/
            const std::string m_portprotocoll;
            /** io service objekt for resolving the server name**/
            boost::asio::io_service m_io;
            /** socket objekt for send / receive the data **/
            bip::tcp::socket m_socket;    
        
            void send( const std::string&, const std::string& = "\r\n" );
            void throwNNTPErrors( const unsigned int& ) const;
    };


    
    
    /** constructor
     * @param p_server servername or adress
     * @param p_portprotocoll port or protocoll name - shoudl be "nntp"
     **/
    inline nntp::nntp( const std::string& p_server, const std::string& p_portprotocoll ) :
        m_server( p_server ),
        m_portprotocoll( p_portprotocoll ),
        m_io(),
        m_socket(m_io)
    {
        // create resolver for server
        bip::tcp::resolver l_resolver(m_io);
        bip::tcp::resolver::query l_query(m_server, m_portprotocoll);
               
        // try to connect the server
        bip::tcp::resolver::iterator l_endpoint = l_resolver.resolve( l_query );
        bip::tcp::resolver::iterator l_endpointend;
        boost::system::error_code l_error       = boost::asio::error::host_not_found;
        
        while (l_error && l_endpoint != l_endpointend) {
            m_socket.close();
            m_socket.connect(*l_endpoint++, l_error);
        }
        
        if (l_error)
            throw exception::parameter(_("cannot connect to news server"));
    }
    
    
    /** destructor for closing the connection **/
    inline nntp::~nntp( void )
    {
        send("quit");
        
        m_socket.close();
    }
    
    
    /** returns the server name
     * @return server name
     **/
    inline std::string nntp::getServer( void ) const
    {
        return m_server;
    }
    
    
    /** returns the protocoll / port 
     * @return protocoll port
     **/
    inline std::string nntp::getPortProtocoll( void ) const
    {
        return m_portprotocoll;
    }
    
    
    /** method for throwing exception if a status code shows an error
     * @param p_status integer
     **/
    void nntp::throwNNTPErrors( const unsigned int& p_status ) const
    {
        switch (p_status) {
                
            // snntp errors
            case 411 : throw exception::parameter(_("no such group")); break;
            case 412 : throw exception::parameter(_("no newsgroup has been selected")); break;
            case 420 : throw exception::parameter(_("no article has been selected")); break;
            case 421 : throw exception::parameter(_("no next article found")); break;
            case 422 : throw exception::parameter(_("no previous article found")); break;
            case 423 : throw exception::parameter(_("no such article number in this group")); break;
            case 430 : throw exception::parameter(_("no such article found")); break;
            case 435 : throw exception::parameter(_("article not wanted - do not send")); break;
            case 436 : throw exception::parameter(_("transfer failed - try again later")); break;
            case 437 : throw exception::parameter(_("article rejected - do not try again")); break;
            case 440 : throw exception::parameter(_("posting not allowed")); break;
            case 441 : throw exception::parameter(_("posting failed")); break;
            
            // default errors
            case 500 : throw exception::parameter(_("command not recognized")); break;
            case 501 : throw exception::parameter(_("command syntax error")); break;
            case 502 : throw exception::parameter(_("access restriction or permission denied")); break;
            case 503 : throw exception::parameter(_("program fault")); break;
        }
        
    }
    
    
    /** sends a command to the nntp server and checks the returing status code
     * @param p_cmd nntp command
     * @param p_end ending seperator often \r\n
     **/
    inline void nntp::send( const std::string& p_cmd, const std::string& p_end )
    {
        // send the command
        boost::asio::streambuf l_request;
        std::ostream l_send( &l_request );
        
        l_send << p_cmd << p_end;
        
        boost::asio::write(m_socket, l_request);
        
        
        // check if we get a status code 200 or 205
        boost::asio::streambuf l_response;
        boost::asio::read_until(m_socket, l_response, p_end );
        
        std::istream response_stream( &l_response );
        
        unsigned int l_status;
        response_stream >> l_status;
        
        throwNNTPErrors( l_status );
    }
    
    
    /** fetchs the group list
     * @return vector with group names
     **/
    inline std::vector<std::string> nntp::getGroupList( void )
    {
        send("list active");

        // read list data
        
        std::vector<std::string> l_groups;
        return l_groups;
    }
    
    

    

};};
#endif
