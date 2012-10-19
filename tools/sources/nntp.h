/** 
 @cond
 ############################################################################
 # LGPL License                                                             #
 #                                                                          #
 # This file is part of the Machine Learning Framework.                     #
 # Copyright (c) 2010-2012, Philipp Kraus, <philipp.kraus@flashpixx.de>     #
 # This program is free software: you can redistribute it and/or modify     #
 # it under the terms of the GNU Lesser General Public License as           #
 # published by the Free Software Foundation, either version 3 of the       #
 # License, or (at your option) any later version.                          #
 #                                                                          #
 # This program is distributed in the hope that it will be useful,          #
 # but WITHOUT ANY WARRANTY; without even the implied warranty of           #
 # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            #
 # GNU Lesser General Public License for more details.                      #
 #                                                                          #
 # You should have received a copy of the GNU Lesser General Public License #
 # along with this program. If not, see <http://www.gnu.org/licenses/>.     #
 ############################################################################
 @endcond
 **/

#ifdef MACHINELEARNING_SOURCES

#ifndef __MACHINELEARNING_TOOLS_SOURCES_NNTP_H
#define __MACHINELEARNING_TOOLS_SOURCES_NNTP_H

#include <string>
#include <iostream>
#include <iterator>
#include <boost/asio.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp> 


#include "../../errorhandling/exception.hpp"
#include "../language/language.h"


namespace machinelearning { namespace tools { namespace sources {
    
    namespace bip  = boost::asio::ip;
    
    
    /** class for creating nntp connection (exspecially for creating distance matrix).
     * The class is a nntp client class without any post functionality (implements the RFC 3977)
     * @see http://tools.ietf.org/html/rfc3977 [old RFC http://www.w3.org/Protocols/rfc977/rfc977]
     * @todo ssl connection
     * @todo authentification
     **/
    class nntp 
    {
        
        public :
        
            enum content
            {
                full    = 0,
                body    = 1,
                header  = 2
            };
                
            nntp( const std::string&, const std::string& = "nntp" );
            std::map<std::string, std::size_t> getGroupList( const std::string& = "" );
            std::vector<std::string> getArticleIDs( const std::string& );
            void setGroup( const std::string& );
            void setContent( const content& );
            content getContent( void ) const;
            std::string getArticle( void );
            std::string getArticle( const std::string& );
            std::string getArticle( const std::string&, const std::string& );
            std::vector<std::string> getArticle( const std::string&, const std::vector<std::string>& );
            std::vector<std::string> getArticle( const std::vector<std::string>& );
            bool isArticleCanceled( void ) const;
            bool existArticle( const std::string& );
            bool existArticle( const std::string&, const std::string& );
            bool nextArticle( void );
            ~nntp( void );
        
        
            /** iterator class for iterating over articles **/
            class iterator : public std::iterator<std::input_iterator_tag, std::string, std::size_t, std::string*, std::string&> {
                
                public :
                
                    iterator( void );
                    iterator( nntp& );
                    bool operator==( const iterator& ) const;
                    bool operator!=( const iterator& ) const;
                    iterator& operator++( int );
                    iterator& operator++( void );
                    nntp& operator*( void );
                    nntp* operator->( void );
                
                private :
                
                    /** pointer to nntp object **/
                    nntp* m_nntp;
                    /** bool for marking the end (empty iterator) **/
                    bool m_end;
                
            };
 
            iterator begin( void );
            iterator end( void );   
        
        
            static void separateHeaderBody( const std::string&, std::string&, std::string& );
        
        
        
        private :
        
            /** io service objekt for resolving the server name**/
            boost::asio::io_service m_io;
            /** socket objekt for send / receive the data **/
            bip::tcp::socket m_socket;  
            /** content option **/
            content m_content;
            /** string data for saving the last returned header for iterators **/
            std::string m_header;
            /** bool for a message that is canceld **/
            bool m_canceled;
            /** regular expression for detecting canceled messages **/
            const boost::regex m_cancelpattern;
        
            unsigned int send( const std::string&, const bool& = true );
            void throwNNTPError( const unsigned int& ) const;
            std::string getResponseData( const std::string& = "\r\n.\r\n" );
            std::map<std::string, std::size_t> extractGroupList( const std::string& );
            std::string getArticleData( void );
        
    };


    
    
    /** constructor
     * @param p_server servername or adress
     * @param p_portprotocoll port or protocoll name - shoudl be "nntp"
     **/
    inline nntp::nntp( const std::string& p_server, const std::string& p_portprotocoll ) :
        m_io(),
        m_socket(m_io),
        m_content( body ),
        m_header(),
        m_canceled( false ),
        m_cancelpattern( "Control: cancel", boost::regex_constants::icase )
    {
        // create resolver for server
        bip::tcp::resolver l_resolver(m_io);
        bip::tcp::resolver::query l_query(p_server, p_portprotocoll);
               
        // try to connect the server
        bip::tcp::resolver::iterator l_endpoint = l_resolver.resolve( l_query );
        bip::tcp::resolver::iterator l_endpointend;
        boost::system::error_code l_error       = boost::asio::error::host_not_found;
        
        while (l_error && l_endpoint != l_endpointend) {
            m_socket.close();
            m_socket.connect(*l_endpoint++, l_error);
        }
        
        if (l_error)
            throw exception::runtime(_("can not connect to news server"), *this);
        
        m_header = getResponseData("\r\n");
    }
    
    
    /** destructor for closing the connection **/
    inline nntp::~nntp( void )
    {
        // we send the quit command, but no exception should be thrown,
        // because we don't know the connection status of the server
        // (the server can close the connection before)
        send("quit", false);
        m_socket.close();
    }
    
    
    /** returns the canceld status of an article
     * @return bool for canceld message
     **/
    inline bool nntp::isArticleCanceled( void ) const
    {
        return m_canceled;
    }
    
    /** sets the content option
     * @param p_content content option
     **/
    inline void nntp::setContent( const content& p_content )
    {
        m_content = p_content;
    }
    
    
    /** returns the content option
     * @return content option
     **/
    inline nntp::content nntp::getContent( void ) const
    {
        return m_content;
    }
    
    
    /** create an exception on the status code
     * @param p_status status code
     **/
    inline void nntp::throwNNTPError( const unsigned int& p_status ) const
    {
        switch (p_status) {
            case 0   : throw exception::runtime(_("error while reading socket data"), *this);     
                
            // nntp errors
            case 411 : throw exception::runtime(_("no such group"), *this);                       
            case 412 : throw exception::runtime(_("no newsgroup has been selected"), *this);      
            case 420 : throw exception::runtime(_("no article has been selected"), *this);          
            case 421 : throw exception::runtime(_("no next article found"), *this);               
            case 422 : throw exception::runtime(_("no previous article found"), *this);           
            case 423 : throw exception::runtime(_("no such article number in this group"), *this); 
            case 430 : throw exception::runtime(_("no such article found"), *this);                
            case 435 : throw exception::runtime(_("article not wanted - do not send"), *this);     
            case 436 : throw exception::runtime(_("transfer failed - try again later"), *this);    
            case 437 : throw exception::runtime(_("article rejected - do not try again"), *this);  
            case 440 : throw exception::runtime(_("posting not allowed"), *this);                  
            case 441 : throw exception::runtime(_("posting failed"), *this);                       
                
            // default errors
            case 500 : throw exception::runtime(_("command not recognized"), *this);               
            case 501 : throw exception::runtime(_("command syntax error"), *this);                 
            case 502 : throw exception::runtime(_("access restriction or permission denied"), *this); 
            case 503 : throw exception::runtime(_("service is not available"), *this);                           
        }
    }
    
    
    /** sends a command to the nntp server and checks the returing status code
     * @param p_cmd nntp command
     * @param p_throw bool for throwing error
     * @return status code
     **/
    inline unsigned int nntp::send( const std::string& p_cmd, const bool& p_throw )
    {
        m_canceled = false;
        
        // send the command
        boost::asio::streambuf l_request;
        std::ostream l_send( &l_request );
        
        l_send << p_cmd << "\r\n" ;
        boost::asio::write(m_socket, l_request);
 
        // read the headerline
        m_header = getResponseData("\r\n");

        // copy the return value into a string and seperates the status code
        unsigned int l_status = 0;
        try {
            l_status = boost::lexical_cast<unsigned int>( m_header.substr(0, 3) );
        } catch (...) {}
 
        if ( p_throw )
            throwNNTPError( l_status );
        
        return l_status;
    }

    
    /** reads the response of the socket
     * @param p_separator seperator for detecting the end (default CRLF DOT CRLF)
     * @return response via string
     **/
    inline std::string nntp::getResponseData( const std::string& p_separator )
    {
        // read data into response after the last entry
        boost::asio::streambuf l_response;
        boost::asio::read_until(m_socket, l_response, p_separator);
        
        
        // convert stream data into string and remove the end seperator
        std::istream l_response_stream( &l_response );
        std::string l_data( (std::istreambuf_iterator<char>(l_response_stream)), std::istreambuf_iterator<char>());
        l_data.erase( l_data.end()-p_separator.size(), l_data.end() );
                
        return l_data;
    }
    
    
    /** read the number of articles within a group
     * @param p_group name of the group (wildcat allowed)
     * @return number of articles
    **/
    inline std::map<std::string, std::size_t> nntp::getGroupList( const std::string& p_group )
    {
        send("list active "+p_group);
        std::string l_data;
        if (p_group.empty())
            l_data = getResponseData();
        else
            l_data = m_header;
        
        // seperates the string data (remove fist and last element)
        std::vector<std::string> l_list;
        boost::split( l_list, l_data, boost::is_any_of("\n") );
        l_list.erase( l_list.begin(), l_list.begin()+1 );
        
        if (p_group.empty())
            l_list.erase( l_list.end()-2, l_list.end() );
        else
            l_list.erase( l_list.end()-1, l_list.end() );
        
        // create group list with number of articles
        std::map<std::string, std::size_t> l_groups;
        for(std::size_t i=0; i < l_list.size(); ++i) {
            std::vector<std::string> l_data;
            boost::split( l_data, l_list[i], boost::is_any_of(" ") );
            
            if (l_data.size() > 1) {
                std::size_t l_num = 0;
                try {
                    l_num = boost::lexical_cast<unsigned int>( l_data[1] );
                } catch (...) {}
                
                l_groups[l_data[0]] = l_num;
            }
        }
        
        return l_groups;        
    }

    
    /** return the number of articles within a group
     * @param p_group group name
     * @return vector with IDs of the articles
     **/
    inline std::vector<std::string> nntp::getArticleIDs( const std::string& p_group )
    {
        send("listgroup "+p_group);
        const std::string l_data = getResponseData();
        
        // seperates the string data (remove fist and last element)
        std::vector<std::string> l_id;
        boost::split( l_id, l_data, boost::is_any_of("\n") );
        l_id.erase( l_id.begin(), l_id.begin()+1 );
        l_id.erase( l_id.end()-2, l_id.end() );
        
        return l_id;
    }

    
    /** seperate article data and set internal status falgs
     * @return std::String with article data
     **/
    inline std::string nntp::getArticleData( void )
    {
        // read full article and seperate header and body
        std::string l_header, l_body;
        const std::string l_full = getResponseData();
        
        nntp::separateHeaderBody( l_full, l_header, l_body );
        
        // check if the article is canceled
        boost::match_results<std::string::const_iterator> l_what; 
        m_canceled = boost::regex_search(l_header, l_what, m_cancelpattern);

        // return the content
        switch (m_content) {
            case full   :   return l_full;
            case body   :   return l_body;
            case header :   return l_header;
        }
        
        return std::string("");
    }
    
    
    /** returns an article
     * @param p_group newsgroup
     * @param p_articleid article ID (not message id)
     * @return message
     **/
    inline std::string nntp::getArticle( const std::string& p_group, const std::string& p_articleid )
    {
        send("group "+p_group);
        send("article "+p_articleid);

        return getArticleData();
    }
    
    
    /** returns an article data
     * @return message
     **/
    inline std::string nntp::getArticle( void )
    {
        send("article");
        return getArticleData();
    }
    
    
    /** reads grouparticles
     * @param p_group string with group name
     * @param p_articleid std::vector with article IDs within the group (not message id)
     * @return std::vector with string content
     **/
    inline std::vector<std::string> nntp::getArticle( const std::string& p_group, const std::vector<std::string>& p_articleid )
    {
        send("group "+p_group);
        
        std::string l_cmd;
        switch (m_content) {
            case full   :   l_cmd = "article";   break;
            case body   :   l_cmd = "body";      break;
            case header :   l_cmd = "head";      break;
        }
        
        std::vector<std::string> l_data;
        for(std::size_t i=0; i < p_articleid.size(); ++i) {
            send(l_cmd + " " + p_articleid[i]);
            l_data.push_back( getResponseData() );
        }
        
        return l_data;
    }
    
    
    /** reads a news article
     * @param p_messageid message ID
     * @return string with article
     **/
    inline std::string nntp::getArticle( const std::string& p_messageid )
    {
        send("article "+ p_messageid); 
        return getArticleData();
    }    
    
    
    /** reads an article list witin their messages IDs
     * @param p_messageid std::vector with a list of message IDs
     * @return std::vector with messages
     **/
    inline std::vector<std::string> nntp::getArticle( const std::vector<std::string>& p_messageid )
    {
        std::vector<std::string> l_data;
        
        for (std::size_t i=0; i < p_messageid.size(); ++i)
            l_data.push_back( getArticle(p_messageid[i]) );

        return l_data;
    }
    
    
    /** check if article exists
     * @param p_messageid message ID
     * @return bool if exists
     **/
    inline bool nntp::existArticle( const std::string& p_messageid )
    {
        const unsigned int l_stat = send("stat "+p_messageid, false);
        switch (l_stat) {
            case 223 :  return true;
            case 430 :  return false;
            default  :  throwNNTPError(l_stat);
        }
        
        return false;
    }
    
    /** check if article exists within a group
     * @param p_group group
     * @param p_articleid article ID
     * @return bool if exists
     **/
    inline bool nntp::existArticle( const std::string& p_group, const std::string& p_articleid )
    {
        send("group "+p_group);
        
        const unsigned int l_stat = send("stat "+p_articleid, false);
        switch (l_stat) {
            case 223 :  return true;
            case 423 :  return false;
            default  :  throwNNTPError(l_stat);
        }
        
        return false;
    }
    
    
    /** sets the group 
     * @param p_group group name
     **/
    inline void nntp::setGroup( const std::string& p_group )
    {
        send("group "+p_group);
    }
    
    /** try to set the next article
     * @return bool, if the set is correct
     **/
    inline bool nntp::nextArticle( void )
    {
        const unsigned int l_stat = send("next", false);
        
        if (l_stat == 421)
            return false;
        else
            throwNNTPError(l_stat);
    
        return true;
    }
    
    
    /** returns the begin iterator to an article
     * @return nntp iterator
     **/
    inline nntp::iterator nntp::begin( void )
    {
        return nntp::iterator( *this );
    }
    
    
    /** return the end iterator 
     * @return nntp iterator
     **/
    inline nntp::iterator nntp::end( void )
    {
        return nntp::iterator();
    }
    
    
    /** static method for separating body and header. The separator is the first double CRLF
     * @param p_article complete article
     * @param p_header string reference in that the header will be filled
     * @param p_body string reference in that the body will be filled
     **/
    inline void nntp::separateHeaderBody( const std::string& p_article, std::string& p_header, std::string& p_body )
    {
        std::size_t l_found = p_article.find("\r\n\r\n");
        if (l_found != std::string::npos) {
            p_header = p_article.substr(0, l_found);
            p_body   = p_article.substr(l_found+4);
        } else {
            p_body   = p_article;
            p_header = "";
        }
    }
    

    //======= Iterator ==================================================================================================================================

    /** constructor
     * @param p_nntp pointer to the nntp object
     **/
    inline nntp::iterator::iterator( nntp& p_nntp ) :
        m_nntp( &p_nntp ),
        m_end( false )
    {}
    
    
    /** default constructor
     **/
    inline nntp::iterator::iterator( void ) :
        m_nntp( NULL ),
        m_end( true )
    {}
    
    
    /** increment operator (iterator++)
     * @return pointer of iterator
     **/
    inline nntp::iterator& nntp::iterator::operator++( int )
    {
        if (m_nntp)
            m_end = !m_nntp->nextArticle();
        
        return *this;
    }
    
    
    /** increment operator (++iterator)
     * @return pointer of iterator
     **/
    inline nntp::iterator& nntp::iterator::operator++( void )
    {
        return (*this)++;
    }
    
    
    /** dereference operator
     * @return object
     **/
    inline nntp& nntp::iterator::operator*( void )
    {
        return *m_nntp;
    }
    
    
    /** pointer operator
     * @return pointer
     **/
    inline nntp* nntp::iterator::operator->( void )
    {
        return m_nntp;
    }
    
    
    /** equal operator
     * @param p_it iterator
     * @return bool for equality
     **/
    inline bool nntp::iterator::operator==( const nntp::iterator& p_it ) const
    {
        if (m_nntp && p_it.m_nntp)
            return m_nntp->m_header == p_it.m_nntp->m_header;
        
        return m_end == p_it.m_end;
    }
    

    /** inequal operator
     * @param p_it iterator
     * @return bool for inequality
     **/
    inline bool nntp::iterator::operator!=( const nntp::iterator& p_it ) const
    {
        if (m_nntp && p_it.m_nntp)
            return m_nntp->m_header != p_it.m_nntp->m_header;
        
        return m_end != p_it.m_end;
    }
    
}}}
#endif
#endif
