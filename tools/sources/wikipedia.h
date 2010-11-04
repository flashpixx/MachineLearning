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

#ifdef SOURCES

#ifndef MACHINELEARNING_TOOLS_SOURCES_WIKIPEDIA_H
#define MACHINELEARNING_TOOLS_SOURCES_WIKIPEDIA_H

#include <boost/asio.hpp>
#include <rapidxml/rapidxml.hpp>

#include "../../exception/exception.h"
#include "../language/language.h"


namespace machinelearning { namespace tools { namespace sources {

    namespace bip  = boost::asio::ip;
    
    
    /** class for reading Wikipedia article.
     * The data will received over a HTTP socket for each call and uses RapidXML library for parsing the XML structur
     * @see http://rapidxml.sourceforge.net/
     * @see http://tools.ietf.org/html/rfc2616 [old http://tools.ietf.org/html/rfc1945]
     **/
    class wikipedia {
        
        public :
        
            enum language {
                en_EN   = 0,
                de_DE   = 1
            };
        
            wikipedia( const language& = de_DE );
            void getArticle( const std::string&, const language& = de_DE );
            void getRandomArticle( const language& = de_DE );
        
        
        private :
        
            /** definition for URL **/
            struct url {
                std::string host;
                std::string path;
            };
        
            /** definition for language **/
            struct wikiproperties {
                language lang;
                url exporturl;
                url randomurl;
                std::string acronymref;
                std::string acronym;
                std::string category;
                std::string titleadd;
            };
        
        
            /** default wikipedia properties **/
            const wikiproperties m_defaultproperties;
            /** io service objekt for resolving the server name**/
            boost::asio::io_service m_io;
            /** socket objekt for send / receive the data **/
            bip::tcp::socket m_socket; 
        
            wikiproperties getProperties( const language& ) const;
            std::string getArticleXMLData( const std::string&, const std::string& );
            void throwHTTPError( const unsigned int& ) const;
            unsigned int send( const std::string&, const std::string&, const bool& = true );
            std::string getResponseData( const std::string& = "\r\n" );
        
    };
    
    
    /** constructor
     * @apram p_lang optional language parameter (default de_DE)
     **/
    inline wikipedia::wikipedia( const language& p_lang ) :
        m_defaultproperties( getProperties(p_lang) ),
        m_io(),
        m_socket(m_io)
    {}
    

    /** creates the properties of a language 
     * @return struct with Wikipedia properties for extraction article information
     **/
    inline wikipedia::wikiproperties wikipedia::getProperties( const language& p_lang ) const
    {
        wikiproperties l_prop;
        
        l_prop.lang = p_lang;
        switch (p_lang) {
            
           case en_EN :
                l_prop.exporturl.host   = "en.wikipedia.org";
                l_prop.exporturl.path   = "/wiki/Special:Export/";
                l_prop.randomurl.host   = "en.wikipedia.org";
                l_prop.randomurl.path   = "/wiki/Special:Random";
                l_prop.acronymref       = "can refer to";
                l_prop.acronym          = "Acronyms";
                l_prop.category         = "Category";
                l_prop.titleadd         = "- Wikipedia, the free encyclopedia";
                return l_prop;
                
            case de_DE :
                l_prop.exporturl.host   = "de.wikipedia.org";
                l_prop.exporturl.path   = "/wiki/Spezial:Exportieren/";
                l_prop.randomurl.host   = "de.wikipedia.org";
                l_prop.randomurl.path   = "/wiki/Spezial:Zuf%C3%A4llige_Seite";
                l_prop.acronymref       = "steht für";
                l_prop.acronym          = "Abkürzung";
                l_prop.category         = "Kategorie";
                l_prop.titleadd         = "Wikipedia";
                return l_prop;

        }
        
        throw exception::parameter(_("language is not kwon"));
    }
    
    
    /** reads an article
     * @param p_search keyword for searching
     * @param p_lang optional language
     **/
    inline void wikipedia::getArticle( const std::string& p_search, const language& p_lang )
    {
        wikiproperties l_prop = m_defaultproperties;
        if (l_prop.lang != p_lang)
            l_prop = getProperties( p_lang );
        
        getArticleXMLData( l_prop.exporturl.host, l_prop.exporturl.path + p_search );
    }
    
    
    /** reads an random article
     * @param p_lang optional language
     **/
    inline void wikipedia::getRandomArticle( const language& p_lang )
    {
        wikiproperties l_prop = m_defaultproperties;
        if (l_prop.lang != p_lang)
            l_prop = getProperties( p_lang );
        
        getArticleXMLData( l_prop.exporturl.host, l_prop.exporturl.path );
    }
    
    
    /** reads the response of the socket
     * @param p_separator seperator for detecting the end (default CRLF DOT CRLF)
     * @return response via string
     **/
    inline std::string wikipedia::getResponseData( const std::string& p_separator )
    {
        // read data into response after the last entry is a "dot CR/LR"
        boost::asio::streambuf l_response;
        boost::asio::read_until(m_socket, l_response, p_separator);
        
        
        // convert stream data into string and remove the end seperator
        std::istream l_response_stream( &l_response );
        std::string l_data( (std::istreambuf_iterator<char>(l_response_stream)), std::istreambuf_iterator<char>());
        l_data.erase( l_data.end()-p_separator.size(), l_data.end() );
        
        return l_data;
    }
    
    
    /** sends the HTTP request to the Wikipedua server and receives the status code
     * @param p_server server adress
     * @param p_path path to the document
     * @param p_throw bool for throwing error
     * @return status code
     **/
    inline unsigned int wikipedia::send( const std::string& p_server, const std::string& p_path, const bool& p_throw )
    {
        // create HTTP request and send them over the socket
        // we need a field for the user-agent, because Wikipedia will blocked the IP
        boost::asio::streambuf l_request;
        std::ostream l_request_stream(&l_request);
        l_request_stream << "GET " << p_path << " HTTP/1.1\r\n";
        l_request_stream << "Host: " << p_server << "\r\n";
        l_request_stream << "Accept: */*\r\n";
        l_request_stream << "User-Agent: Machine-Learning-Toolbox\r\n";
        l_request_stream << "Connection: close\r\n\r\n";
        
        boost::asio::write( m_socket, l_request );
    
    
        // read the first line
        const std::string l_returnline = getResponseData();
        
        // copy the return value into a string and seperates the status code
        unsigned int l_status = 0;
        try {
            l_status = boost::lexical_cast<unsigned int>( l_returnline.substr( l_returnline.find(" ")+1,3) );
        } catch (...) {}
        
        if ( p_throw )
            throwHTTPError( l_status );
        
        return l_status;
    }
    
    
    /** sends the request and returns the XML data
     * @param p_server server adress
     * @param p_path path to the document
     * @return 
     **/
    inline std::string wikipedia::getArticleXMLData( const std::string& p_server, const std::string& p_path )
    {
        // create resolver for server
        bip::tcp::resolver l_resolver(m_io);
        bip::tcp::resolver::query l_query(p_server, "http");
        
        // try to connect the server
        bip::tcp::resolver::iterator l_endpoint = l_resolver.resolve( l_query );
        bip::tcp::resolver::iterator l_endpointend;
        boost::system::error_code l_error       = boost::asio::error::host_not_found;
        
        while (l_error && l_endpoint != l_endpointend) {
            m_socket.close();
            m_socket.connect(*l_endpoint++, l_error);
        }
        
        if (l_error)
            throw exception::parameter(_("cannot connect to wikipedia server"));
        
        send(p_server, p_path);
        
        std::string l_xml;
        return l_xml;
        
    }
    
    
    /** create an exception on the status code
     * @param p_status status code
     **/
    inline void wikipedia::throwHTTPError( const unsigned int& p_status ) const
    {
        switch (p_status) {
            case 0      : throw exception::parameter(_("error while reading socket data"));     
                
            case 203    : throw exception::parameter(_("Non-Authoritative Information"));
            case 204    : throw exception::parameter(_("No Content"));
            case 205    : throw exception::parameter(_("Reset Content"));
            case 206    : throw exception::parameter(_("Partial Content"));
            case 300    : throw exception::parameter(_("Multiple Choices"));
            case 301    : throw exception::parameter(_("Moved Permanently"));
            case 302    : throw exception::parameter(_("Found"));
            case 303    : throw exception::parameter(_("See Other"));
            case 304    : throw exception::parameter(_("Not Modified"));
            case 305    : throw exception::parameter(_("Use Proxy"));
            case 307    : throw exception::parameter(_("Temporary Redirect"));
            case 400    : throw exception::parameter(_("Bad Request"));
            case 401    : throw exception::parameter(_("Unauthorized"));
            case 402    : throw exception::parameter(_("Payment Required"));
            case 403    : throw exception::parameter(_("Forbidden"));
            case 404    : throw exception::parameter(_("Not Found"));
            case 405    : throw exception::parameter(_("Method Not Allowed"));
            case 406    : throw exception::parameter(_("Not Acceptable"));
                
            case 407    : throw exception::parameter(_("Proxy Authentication Required"));
            case 408    : throw exception::parameter(_("Request Time-out"));
            case 409    : throw exception::parameter(_("Conflict"));
            case 410    : throw exception::parameter(_("Gone"));
            case 411    : throw exception::parameter(_("Length Required"));
            case 412    : throw exception::parameter(_("Precondition Failed"));
            case 413    : throw exception::parameter(_("Request Entity Too Large"));
            case 414    : throw exception::parameter(_("Request-URI Too Large"));
            case 415    : throw exception::parameter(_("Unsupported Media Type"));
            case 416    : throw exception::parameter(_("Requested range not satisfiable"));
            case 417    : throw exception::parameter(_("Expectation Failed"));
            case 500    : throw exception::parameter(_("Internal Server Error"));
            case 501    : throw exception::parameter(_("Not Implemented"));
            case 502    : throw exception::parameter(_("Bad Gateway"));
            case 503    : throw exception::parameter(_("Service Unavailable"));
            case 504    : throw exception::parameter(_("Gateway Time-out"));
            case 505    : throw exception::parameter(_("HTTP Version not supported"));
        }
    }
    
    
};};};


#endif
#endif