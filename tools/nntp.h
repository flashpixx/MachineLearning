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
#include <boost/iostreams/copy.hpp>
#include <boost/algorithm/string.hpp> 
#include <boost/iostreams/concepts.hpp>
#include <boost/iostreams/operations.hpp> 
#include <boost/iostreams/filtering_stream.hpp>


#include "../exception/exception.h"
#include "language/language.h"


namespace machinelearning { namespace tools {
    
    namespace bip  = boost::asio::ip;
    namespace bio  = boost::iostreams;

    
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
            std::size_t getArticleNumber( const std::string& );

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
        
        
        
        
            /** private class for filtering nntp data (filtering newsgroup list) **/
            class groupfilter : public bio::output_filter {
                
                public:
                
                    /** constructor for initialisation skip variable **/
                    groupfilter( void ) : m_skip( false ) {}
                
                    /** put method
                     * @overload
                     * @param p_dest destination stream
                     * @param p_char char for writing
                     * @return bool for writing
                     **/
                    template<typename Sink> bool put(Sink& p_dest, int p_char)
                    {
                        if (m_skip) {
                            if (p_char == '\n') {
                                m_skip = false;
                                return bio::put( p_dest, p_char );
                            }
                            return true;
                            
                        } else
                            if ( (p_char == ' ') || (p_char == '\t') ) {
                                m_skip = true;
                                return true;
                            }
 
                        return bio::put( p_dest, p_char );
                    }
                
                    /** close filter
                     * @param Source close stream
                     **/
                    template<typename Source> void close(Source&) { m_skip = false; }
            
                
                private:
                    
                    /** bool for skipping characters **/
                    bool m_skip;
            };
        
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
    
    
    /** sends a command to the nntp server and checks the returing status code
     * @param p_cmd nntp command
     * @param p_end ending seperator often CR/LR
     **/
    inline void nntp::send( const std::string& p_cmd, const std::string& p_end )
    {
        // send the command
        boost::asio::streambuf l_request;
        std::ostream l_send( &l_request );
        
        l_send << p_cmd << p_end;
        
        boost::asio::write(m_socket, l_request);
        
        
        // check check the returning status code
        boost::asio::streambuf l_response;
        boost::asio::read_until(m_socket, l_response, p_end );
        
        std::istream l_response_stream( &l_response );
        
        unsigned int l_status;
        l_response_stream >> l_status;
        
        switch (l_status) {
                
            // nntp errors
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
    
    
    /** fetchs the active group list
     * @return vector with group names
     **/
    inline std::vector<std::string> nntp::getGroupList( void )
    {
        send("list active");

        
        // read data into response after the last entry is a "CR/LR dot CR/LR"
        boost::asio::streambuf l_response;
        std::istream l_response_stream( &l_response );
        
        boost::asio::read_until(m_socket, l_response, "\r\n.\r\n");

        
        // response hold the data, we filter the data stream, so that only group names are hold in
        std::string l_filtergroup;
        bio::filtering_ostream  l_out( std::back_inserter(l_filtergroup) );
        bio::filtering_streambuf< bio::output > l_filter;
        l_filter.push( groupfilter() );
        l_filter.push( l_out );
        
        bio::copy( l_response, l_filter );
        
        
        // seperates the string data (remove fist and last element)
        std::vector<std::string> l_groups;
        boost::split( l_groups, l_filtergroup, boost::is_any_of("\n") );
        l_groups.erase( l_groups.begin(), l_groups.begin()+1 );
        l_groups.erase( l_groups.end()-2, l_groups.end() );
        
        return l_groups;
    }
    
    
    /** return the number of articles within a group
     * @param p_group group name
     * @return number of articles
     **/
    inline std::size_t nntp::getArticleNumber( const std::string& p_group )
    {
        send("listgroup "+p_group);
        /*
        // read data into response after the last entry is a "CR/LR dot CR/LR"
        boost::asio::streambuf l_response;
        std::istream l_response_stream( &l_response );
        
        boost::asio::read_until(m_socket, l_response, "\r\n.\r\n");
        
        std::cout << l_response_stream << std::endl;
        */
        return 0;
    }

    

};};
#endif
