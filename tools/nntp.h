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
#include <boost/lexical_cast.hpp>
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
     * @see http://tools.ietf.org/html/rfc3977 [old RFC http://www.w3.org/Protocols/rfc977/rfc977]
     * @todo ssl connection
     **/
    class nntp {
        
        public :
        
            enum content
            {
                full    = 0,
                body    = 1,
                header  = 2
            };
                
            nntp( const std::string&, const std::string& = "nntp" );
            std::string getServer( void ) const;
            std::string getPortProtocoll( void ) const;
            std::vector<std::string> getGroupList( void );
            std::vector<std::string> getArticleIDs( const std::string& );
            std::string getArticle( const std::string&, const content& = body );
            std::string getArticle( const std::string&, const std::string&, const content& = body );
            std::vector<std::string> getArticle( const std::string&, const std::vector<std::string>&, const content& = body );
            std::vector<std::string> getArticle( const std::vector<std::string>&, const content& = body );
        
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
        
            void send( const std::string& );
        
        
        
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
                
                    /** close filter **/
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
     **/
    inline void nntp::send( const std::string& p_cmd )
    {
        // send the command
        boost::asio::streambuf l_request;
        std::ostream l_send( &l_request );
        
        l_send << p_cmd << "\r\n" ;
        
        boost::asio::write(m_socket, l_request);
 
        
        // read the first line
        boost::asio::streambuf l_response;
        std::istream l_response_stream( &l_response );

        boost::asio::read_until(m_socket, l_response, "\r\n" );
        
        // copy the return value into a string and seperates the status code
        unsigned int l_status = 0;
        std::string l_returnline;
        bio::filtering_ostream  l_out( std::back_inserter(l_returnline) );
        bio::copy( l_response, l_out );
  
        
        try {
            l_status = boost::lexical_cast<unsigned int>( l_returnline.substr(0,3) );
        } catch (...) {}
 
        
        switch (l_status) {
            case 0   : throw exception::parameter(_("error while reading socket data"));            break;
                
            // nntp errors
            case 411 : throw exception::parameter(_("no such group"));                              break;
            case 412 : throw exception::parameter(_("no newsgroup has been selected"));             break;
            case 420 : throw exception::parameter(_("no article has been selected"));               break;
            case 421 : throw exception::parameter(_("no next article found"));                      break;
            case 422 : throw exception::parameter(_("no previous article found"));                  break;
            case 423 : throw exception::parameter(_("no such article number in this group"));       break;
            case 430 : throw exception::parameter(_("no such article found"));                      break;
            case 435 : throw exception::parameter(_("article not wanted - do not send"));           break;
            case 436 : throw exception::parameter(_("transfer failed - try again later"));          break;
            case 437 : throw exception::parameter(_("article rejected - do not try again"));        break;
            case 440 : throw exception::parameter(_("posting not allowed"));                        break;
            case 441 : throw exception::parameter(_("posting failed"));                             break;
                
            // default errors
            case 500 : throw exception::parameter(_("command not recognized"));                     break;
            case 501 : throw exception::parameter(_("command syntax error"));                       break;
            case 502 : throw exception::parameter(_("access restriction or permission denied"));    break;
            case 503 : throw exception::parameter(_("program fault"));                              break;
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
     * @return vector with IDs of the articles
     **/
    inline std::vector<std::string> nntp::getArticleIDs( const std::string& p_group )
    {
        send("listgroup "+p_group);
        
        // read data into response after the last entry is a "dot CR/LR"
        boost::asio::streambuf l_response;
        std::istream l_response_stream( &l_response );
        
        boost::asio::read_until(m_socket, l_response, "\r\n.\r\n");
        
        
        // seperates the IDs
        std::string l_string;
        bio::filtering_ostream  l_out( std::back_inserter(l_string) );
        bio::copy( l_response, l_out );

        
        // seperates the string data (remove fist and last element)
        std::vector<std::string> l_id;
        boost::split( l_id, l_string, boost::is_any_of("\n") );
        l_id.erase( l_id.begin(), l_id.begin()+1 );
        l_id.erase( l_id.end()-2, l_id.end() );
        
        return l_id;
    }

    
    /** returns an article
     * @param p_group newsgroup
     * @param p_articleid article ID (not message id)
     * @param p_content switch for reading full article, head or body only (default body only)
     * @return message
     **/
    inline std::string nntp::getArticle( const std::string& p_group, const std::string& p_articleid, const content& p_content )
    {
        send("group "+p_group);
        
        switch (p_content) {
            case full   :   send("article "+p_articleid);   break;
            case body   :   send("body "+p_articleid);      break;
            case header :   send("head "+p_articleid);      break;
        }
        
        // read data into response after the last entry is a "dot CR/LR"
        boost::asio::streambuf l_response;
        std::istream l_response_stream( &l_response );
        
        boost::asio::read_until(m_socket, l_response, "\r\n.\r\n");

        
        // convert stream data into string and remove the end seperator and the first line
        std::string l_article;
        bio::filtering_ostream  l_out( std::back_inserter(l_article) );
        bio::copy( l_response, l_out );

        l_article.erase( 0, l_article.find("\r\n")+2 );
        l_article.erase( l_article.end()-5, l_article.end() );
                
        return l_article;
        
    }
    
    
    /** reads grouparticles
     * @param p_group string with group name
     * @param p_articleid std::vector with article IDs within the group (not message id)
     * @param p_content switch for reading full article, head or body only (default body only)
     * @return std::vector with string content
     **/
    std::vector<std::string> nntp::getArticle( const std::string& p_group, const std::vector<std::string>& p_articleid, const content& p_content )
    {
        send("group "+p_group);
        
        std::string l_cmd;
        switch (p_content) {
            case full   :   l_cmd = "article";   break;
            case body   :   l_cmd = "body";      break;
            case header :   l_cmd = "head";      break;
        }
        
        std::vector<std::string> l_data;
        for(std::size_t i=0; i < p_articleid.size(); ++i) {
            send(l_cmd + " " + p_articleid[i]);
            
            // read data into response after the last entry is a "dot CR/LR"
            boost::asio::streambuf l_response;
            std::istream l_response_stream( &l_response );
            
            boost::asio::read_until(m_socket, l_response, "\r\n.\r\n");

            
            // convert stream data into string and remove the end seperator and the first line
            std::string l_article;
            bio::filtering_ostream  l_out( std::back_inserter(l_article) );
            bio::copy( l_response, l_out );
            
            l_article.erase( 0, l_article.find("\r\n")+2 );
            l_article.erase( l_article.end()-5, l_article.end() );
        
            l_data.push_back( l_article );
        }
        
        return l_data;
    }
    
    
    /** reads a news article
     * @param p_messageid message ID
     * @param p_content switch for reading full article, head or body only (default body only)
     * @return string with article
     **/
    inline std::string nntp::getArticle( const std::string& p_messageid, const content& p_content )
    {
        switch (p_content) {
            case full   :   send("article "+ p_messageid);   break;
            case body   :   send("body "+ p_messageid);      break;
            case header :   send("head "+ p_messageid);      break;
        }
        
        // read data into response after the last entry is a "dot CR/LR"
        boost::asio::streambuf l_response;
        std::istream l_response_stream( &l_response );
        
        boost::asio::read_until(m_socket, l_response, "\r\n.\r\n");
        
        
        // convert stream data into string and remove the end seperator and the first line
        std::string l_article;
        bio::filtering_ostream  l_out( std::back_inserter(l_article) );
        bio::copy( l_response, l_out );
        
        l_article.erase( 0, l_article.find("\r\n")+2 );
        l_article.erase( l_article.end()-5, l_article.end() );
        
        return l_article;        
    }    
    
    
    /** reads an article list witn their messages IDs
     * @param p_messageid std::vector with a list of message IDs
     * @param p_content switch for reading full article, head or body only (default body only)
     * @return std::vector with messages
     **/
    inline std::vector<std::string> nntp::getArticle( const std::vector<std::string>& p_messageid, const content& p_content )
    {
        std::vector<std::string> l_data;
        
        for (std::size_t i=0; i < p_messageid.size(); ++i)
            l_data.push_back( getArticle(p_messageid[i], p_content) );
        
        
        return l_data;
    }

};};
#endif
