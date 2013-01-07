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

#ifndef __MACHINELEARNING_TOOLS_SOURCES_WIKIPEDIA_H
#define __MACHINELEARNING_TOOLS_SOURCES_WIKIPEDIA_H

#include <algorithm>
#include <boost/asio.hpp>
#include <boost/regex.hpp>

extern "C" {
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
}

#include "../../errorhandling/exception.hpp"
#include "../language/language.h"


namespace machinelearning { namespace tools { namespace sources {

    namespace bip  = boost::asio::ip;
    
    
    /** class for reading Wikipedia article.
     * The data will received over a HTTP socket for each call and uses the nativ C interface of libxml2 for parsing the XML data.
     * There are no simple C++ interfaces for XML data, so we use the C source within the class. 
     * @see http://tools.ietf.org/html/rfc2616 [old http://tools.ietf.org/html/rfc1945]
     * @todo add proxy support
     * @todo class is not thread-safe, so create mutex for thread-safe calls
     **/
    class wikipedia 
    {
        
        public :
        
            wikipedia( const language::code& = language::EN );
            void getArticle( const std::string& );
            void getArticle( const std::string&, const language::code& );
            void getRandomArticle( const language::code& );
            void getRandomArticle();
        
            std::string getArticleContent( void ) const;
            std::string getArticleTitle( void ) const;
            std::size_t getArticleRevision( void ) const;
            std::size_t getArticleID( void ) const;
            std::vector<std::string> getArticleLabel( void ) const;
            std::vector<std::string> getArticleAcronym( void ) const;
            bool isAcronym( void ) const;
            bool isArticle( void ) const;
            void setHTTPAgent( const std::string& );
        
            ~wikipedia( void );
        
        private :
        
            /** definition for URL **/
            struct url 
            {
                std::string host;
                std::string path;
            };
        
            /** definition for language **/
            struct wikiproperties 
            {
                language::code lang;
                url exporturl;
                url randomurl;
                std::string acronymref;
                std::string acronym;
                std::string category;
            };
        
            /** struct for article content **/
            struct wikiarticle 
            {
                std::string content;
                std::string title;
                std::size_t articleid;
                std::size_t revisionid;
                std::vector<std::string> label;
            };
        
        
            /** default wikipedia properties **/
            const wikiproperties m_defaultproperties;
            /** io service objekt for resolving the server name **/
            boost::asio::io_service m_io;

            /** socket objekt for send / receive the data **/
            bip::tcp::socket m_socket; 
            /** string for caching the server host name **/
            std::string m_lastserver;
            /** IP for caching the server endpoint **/
            bip::tcp::endpoint m_lastendpoint;
            /** bool for founded article **/
            bool m_articlefound;
            /** bool for founded acronym page **/
            bool m_acronymfound;
            /** property for saving the last-call article data **/
            wikiarticle m_article;
            /** acronym vector **/
            std::vector<std::string> m_acronym;
            /** name for the HTTP agent **/
            std::string m_httpagent;

        
            wikiproperties getProperties( const language::code& ) const;
            unsigned int sendRequest( const std::string&, const std::string&, std::string&, const bool& = true );
            std::string getContentData( void ); 
            void throwHTTPError( const unsigned int& ) const;   
            wikiarticle parseXML( const std::string& ) const;
        
            #ifdef NDEBUG
            static void XMLErrorFunction(void*, const char*, ...) {};
            #endif
    };
    
    
    /** constructor
     * @param p_lang optional language parameter (default de_DE)
     **/
    inline wikipedia::wikipedia( const language::code& p_lang ) :
        m_defaultproperties( getProperties(p_lang) ),
        m_io(),
        m_socket(m_io),
        m_lastserver(),
        m_lastendpoint(),
        m_articlefound( false ),
        m_acronymfound( false ),
        m_acronym(),
        m_httpagent("Machine Learning Framework")
    {}
    
    
    /** destructor for closing the socket **/
    inline wikipedia::~wikipedia( void )
    {
        m_socket.close();
    }
    

    /** creates the properties of a language 
     * @return struct with Wikipedia properties for extraction article information
     **/
    inline wikipedia::wikiproperties wikipedia::getProperties( const language::code& p_lang ) const
    {
        wikiproperties l_prop;
        
        l_prop.lang = p_lang;
        switch (p_lang) {
            
            case language::EN :
                l_prop.exporturl.host   = "en.wikipedia.org";
                l_prop.exporturl.path   = "/wiki/Special:Export/";
                l_prop.randomurl.host   = "en.wikipedia.org";
                l_prop.randomurl.path   = "/wiki/Special:Random";
                l_prop.acronymref       = "can refer to";
                l_prop.category         = "Category";
                return l_prop;
                
            case language::DE :
                l_prop.exporturl.host   = "de.wikipedia.org";
                l_prop.exporturl.path   = "/wiki/Spezial:Exportieren/";
                l_prop.randomurl.host   = "de.wikipedia.org";
                l_prop.randomurl.path   = "/wiki/Spezial:Zuf%C3%A4llige_Seite";
                l_prop.acronymref       = "steht für";
                l_prop.category         = "Kategorie";
                return l_prop;

            default:
                throw exception::runtime(_("language has no option values"), *this);
                
        }
    }
    
    
    /** reads an article
     * @param p_search keyword for searching
     **/
    inline void wikipedia::getArticle( const std::string& p_search )
    {
        getArticle( p_search, m_defaultproperties.lang );
    
    }
    
    /** reads an article
     * @param p_search keyword for searching
     * @param p_lang optional language
     * @note remove of languages codes are incomplet
     **/
    inline void wikipedia::getArticle( const std::string& p_search, const language::code& p_lang )
    {
        if (p_search.empty())
            throw exception::runtime(_("search query need not be empty"), *this);
        
        m_articlefound = false;
        m_acronym.clear();
        
        wikiproperties l_prop = getProperties( p_lang );
        
        // whitespace with underscore replace
        std::string l_search( p_search );
        std::replace( l_search.begin(), l_search.end(), ' ', '_' );
        
        // send request 
        std::string l_header;
        sendRequest( l_prop.exporturl.host, l_prop.exporturl.path + l_search, l_header );
        const std::string l_xml = getContentData();
        m_socket.close();
        
        // get native XML data and analyse content
        m_article = parseXML( l_xml );

        
        // parse content data (category, acronyms, redirect...)
        boost::smatch l_what;
        std::string::const_iterator it    = m_article.content.begin();
        std::string::const_iterator l_end = m_article.content.end();
        
        
        // check if exists redirect in the content data, run a new request (we use a case-insensitive search)
        const boost::regex l_redirectpattern( "#redirect \\[\\[.*?\\]\\]", boost::regex_constants::icase | boost::regex_constants::perl );
        if (boost::regex_search(m_article.content, l_what, l_redirectpattern)) {
            const std::string l_pattern(l_what[0]);
            getArticle(l_pattern.substr(12, l_pattern.size()-14), p_lang);
            return;
        }

        
        // check if the content is acronym page, than extract the acronyms (hint: non-greedy excepted)
        const boost::regex l_acronympattern( "'''" + m_article.title + "'''[[:space:]]+"+l_prop.acronymref+":", boost::regex_constants::perl );
        if (boost::regex_search(m_article.content, l_what, l_acronympattern)) {
            const boost::regex l_acronyms( "\\*[[:space:]]*\\[\\[(.*?)\\]\\]", boost::regex_constants::perl );

            while (boost::regex_search(it, l_end, l_what, l_acronyms)) {
                m_acronym.push_back( l_what[1] );
                it = l_what[0].second;
            }
            
            m_acronymfound = true;
            return;
        }

        
        // extract category with regular expression \[\[<category name>:(.*?)\]\] (hint: non-greedy excepted)
        const boost::regex l_categorypattern( "\\[\\[" + l_prop.category + ":(.*?)\\]\\][[:space:]]*", boost::regex_constants::icase | boost::regex_constants::perl );
        while (boost::regex_search(it, l_end, l_what, l_categorypattern)) {
            std::string l_label(l_what[1]);
            m_article.label.push_back( l_label.substr(0, l_label.find("|")) );
            it = l_what[0].second;
        }
        
        // remove languages and labels (at the end of the expression we set a [[:space:]]* for removing CR or LF)
        m_article.content = boost::regex_replace(m_article.content, l_categorypattern, "");
        std::vector<std::string> l_codes = tools::language::getCodeList( tools::language::iso639_1 );
        std::string l_list;
        for(std::size_t i=0; i < l_codes.size(); ++i)
            l_list += "|" + l_codes[i];
        l_list = l_list.substr(1);
        
        const boost::regex l_langpattern( "\\[\\[("+l_list+"):(.*?)\\]\\][[:space:]]*", boost::regex_constants::icase | boost::regex_constants::perl );
        m_article.content = boost::regex_replace(m_article.content, l_langpattern, "");
        
        m_articlefound = true;    
    }
    
    
    /** reads an random article with default language **/
    inline void wikipedia::getRandomArticle()
    {
        getRandomArticle( m_defaultproperties.lang );
    }
    
    
    /** reads an random article
     * @param p_lang optional language
     **/
    inline void wikipedia::getRandomArticle( const language::code& p_lang )
    {
        m_articlefound              = false;
        const wikiproperties l_prop = getProperties( p_lang );
        
        std::string l_header;
        unsigned int l_status = sendRequest( l_prop.randomurl.host, l_prop.randomurl.path, l_header, false );
        m_socket.close();
        if (l_status != 302)
            throwHTTPError(l_status);
        
        // read the location header tag and extract the URL
        std::size_t l_found = l_header.find("Location:");
        if (l_found == std::string::npos)
            throw exception::runtime(_("can not find information within the http header"), *this);
        
        // extract Location URL
        const std::string l_url = l_header.substr(l_found+10, l_header.find("\r\n", l_found)-l_found-10);
        
        // extract document of the URL
        l_found = l_url.rfind("/");
        if (l_found == std::string::npos)
            throw exception::runtime(_("can not find wikipedia document within the URL"), *this);
        
        // get the article
        getArticle( l_url.substr(l_found+1), l_prop.lang );
    }
    
    
    /** sets the name of the HTTP agent
     * @param p_agent name
     **/
    inline void wikipedia::setHTTPAgent( const std::string& p_agent) 
    {
        if (p_agent.empty())
            throw exception::runtime(_("HTTP agent name need not be empty"), *this);
        m_httpagent = p_agent;
    }
    
    
    /** returns the article content
     * @return content
     **/
    inline std::string wikipedia::getArticleContent( void ) const
    {
        if (!m_articlefound)
            throw exception::runtime(_("no article is loaded"), *this);
        
        return m_article.content;
    }
    
    /** returns the article title
     * @return title
     **/
    inline std::string wikipedia::getArticleTitle( void ) const
    {
        if (!m_articlefound)
            throw exception::runtime(_("no article is loaded"), *this);
        
        return m_article.title;
    }
    
    /** return revision id
     * @return article revision id
     **/
    inline std::size_t wikipedia::getArticleRevision( void ) const
    {
        if (!m_articlefound)
            throw exception::runtime(_("no article is loaded"), *this);
        
        return m_article.revisionid;
    }
    
    /** return article id
     * @return article id
     **/
    inline std::size_t wikipedia::getArticleID( void ) const
    {
        if (!m_articlefound)
            throw exception::runtime(_("no article is loaded"), *this);
        
        return m_article.articleid;
    }
    
    /** return article labels
     * @return label
     **/
    inline std::vector<std::string> wikipedia::getArticleLabel( void ) const
    {
        if (!m_articlefound)
            throw exception::runtime(_("no article is loaded"), *this);
        
        return m_article.label;
    }
    
    /** return a std::vector with acronyms
     * @return vector
     **/
    inline std::vector<std::string> wikipedia::getArticleAcronym( void ) const
    {
        return m_acronym;
    }
    
    /** return bool if article is acronym page
     * @return bool
     **/
    inline bool wikipedia::isAcronym( void ) const
    {
        return m_acronymfound;
    }
    
    /** return bool if article is found
     * @return bool
     **/
    inline bool wikipedia::isArticle( void ) const
    {
        return m_articlefound;
    }
    
    /** method for parsing the XML data with libxml
     * @param p_xml string with XML data
     * @return struct with article native data
     **/
    inline wikipedia::wikiarticle wikipedia::parseXML( const std::string& p_xml ) const
    {
        if (p_xml.empty())
            throw exception::runtime(_("XML data can not be parsed"), *this);
        
        xmlResetLastError();
        wikiarticle l_data;
        bool l_error = false;
        
        #ifdef NDEBUG
        // set for XML parsing a empty static member function, so no messages will be shown
        
        // note: we need a "void(**)" see http://xmlsoft.org/html/libxml-xmlerror.html#xmlGenericErrorFunc and
        // http://xmlsoft.org/html/libxml-xmlerror.html#initGenericErrorDefaultFunc so we create this with a
        // help structure for correct a correct reference
        xmlGenericErrorFunc l_fptr = wikipedia::XMLErrorFunction;
        initGenericErrorDefaultFunc( &l_fptr );
        #endif
        
        // convert std::string into char array for memory parsing
        const char* l_xmlcontent = p_xml.c_str();
        xmlDocPtr l_xml          = xmlParseMemory( l_xmlcontent, p_xml.length()+1 );
        if ((!l_xml) || (xmlGetLastError())) {
            if (l_xml)
                xmlFreeDoc( l_xml );
            xmlCleanupParser();
            throw exception::runtime(_("XML data can not be parsed"), *this);
        }
        
        // extract the namespace 
        xmlNodePtr l_node = xmlDocGetRootElement( l_xml );
        std::string l_namespace( (char*)xmlGetProp(l_node, (xmlChar*)"schemaLocation") );
        std::size_t l_found = l_namespace.find(" ");
        if ( (l_namespace.empty()) || (l_found == std::string::npos) ) {
            xmlFreeDoc( l_xml );
            xmlCleanupParser();
            
            throw exception::runtime(_("can not detect namespace"), *this);
        }
        l_namespace = l_namespace.substr(0, l_found );

        
        // create XPath context and namespace (create a prefix "wiki" for searching the XML tree)
        xmlXPathContextPtr l_tree = xmlXPathNewContext( l_xml );
        xmlXPathRegisterNs(l_tree, (xmlChar*)"wiki", (xmlChar*)l_namespace.c_str());

        // extract the content data, search with XPath the node and extract the content
        
        // /mediawiki/page/title              => article title
        // /mediawiki/page/id                 => article id
        // /mediawiki/page/revision/text      => article data
        // /mediawiki/page/revision/id        => revision id

        if (!l_error) {
            xmlXPathObjectPtr l_result = xmlXPathEvalExpression( (xmlChar*)"/wiki:mediawiki/wiki:page/wiki:title", l_tree );
            if ( (!xmlXPathNodeSetIsEmpty(l_result->nodesetval)) && (l_result->nodesetval->nodeNr == 1) )
                l_data.title = std::string(  (char*)xmlNodeListGetString(l_xml, l_result->nodesetval->nodeTab[0]->xmlChildrenNode, 1)  );
            else
                l_error = true;
            xmlXPathFreeObject(l_result);
        }
        
        if (!l_error) {
            xmlXPathObjectPtr l_result = xmlXPathEvalExpression( (xmlChar*)"/wiki:mediawiki/wiki:page/wiki:id", l_tree );
            if ( (!xmlXPathNodeSetIsEmpty(l_result->nodesetval)) && (l_result->nodesetval->nodeNr == 1) )
                try {
                    l_data.articleid = boost::lexical_cast<std::size_t>( (char*)xmlNodeListGetString(l_xml, l_result->nodesetval->nodeTab[0]->xmlChildrenNode, 1) );
                } catch (...) {
                    l_error = true;
                }
            else
                l_error = true;
            xmlXPathFreeObject(l_result);
        }
        
        if (!l_error) {
            xmlXPathObjectPtr l_result = xmlXPathEvalExpression( (xmlChar*)"/wiki:mediawiki/wiki:page/wiki:revision/wiki:text", l_tree );
            if ( (!xmlXPathNodeSetIsEmpty(l_result->nodesetval)) && (l_result->nodesetval->nodeNr == 1) )
                l_data.content = std::string(  (char*)xmlNodeListGetString(l_xml, l_result->nodesetval->nodeTab[0]->xmlChildrenNode, 1)  );
            else
                l_error = true;
            xmlXPathFreeObject(l_result);
        }
        
        if (!l_error) {
            xmlXPathObjectPtr l_result = xmlXPathEvalExpression( (xmlChar*)"/wiki:mediawiki/wiki:page/wiki:revision/wiki:id", l_tree );
            if ( (!xmlXPathNodeSetIsEmpty(l_result->nodesetval)) && (l_result->nodesetval->nodeNr == 1) )
                try {
                    l_data.revisionid = boost::lexical_cast<std::size_t>( (char*)xmlNodeListGetString(l_xml, l_result->nodesetval->nodeTab[0]->xmlChildrenNode, 1) );
                } catch (...) {
                    l_error = true;
                }
            else
                l_error = true;
            xmlXPathFreeObject(l_result);
        }
        
        // clear libxml structure
        xmlXPathFreeContext( l_tree );
        xmlFreeDoc( l_xml );
        xmlCleanupParser();
           
        if (l_error)
            throw exception::runtime(_("XML data can not be parsed"), *this);

        return l_data;
        
    }
    
    
    /** reads the whole data until socket is EOF
     * @return string data
     **/
    inline std::string wikipedia::getContentData( void )
    {
        // header is extract in the send command and only the data is now present in the socket, so we read it until EOF
        boost::system::error_code l_error;
        boost::asio::streambuf l_response;
        std::istream l_response_stream(&l_response);
        
        while (boost::asio::read(m_socket, l_response, boost::asio::transfer_at_least(1), l_error))
            ;
        
        if (l_error != boost::asio::error::eof)
            throw exception::runtime(_("data can not be received"), *this);
        
        std::string l_data( (std::istreambuf_iterator<char>(l_response_stream)), std::istreambuf_iterator<char>());        
        
        return l_data;
    }
    
    
    /** sends the HTTP request to the Wikipedia server and receives the header
     * @param p_server server adress
     * @param p_path path to the document
     * @param p_header returning HTTP header
     * @param p_throw bool for throwing error
     * @return status code
     **/
    inline unsigned int wikipedia::sendRequest( const std::string& p_server, const std::string& p_path, std::string& p_header, const bool& p_throw )
    {
        // check if changed the server name, if not we use the cached IP
        boost::system::error_code l_error = boost::asio::error::host_not_found;
        if (p_server == m_lastserver)
            m_socket.connect(m_lastendpoint, l_error);
        else {
            m_lastserver = p_server;
            
            // create resolver for server
            bip::tcp::resolver l_resolver(m_io);
            bip::tcp::resolver::query l_query(p_server, "http");
            
            // try to connect the server
            for(bip::tcp::resolver::iterator l_endpoint = l_resolver.resolve( l_query ); (l_error && l_endpoint != bip::tcp::resolver::iterator()); l_endpoint++) {
                m_socket.close();
                m_lastendpoint = *l_endpoint;
                m_socket.connect(*l_endpoint, l_error);
            }
        }
            
        if (l_error)
            throw exception::runtime(_("can not connect to wikipedia server"), *this);
        
        
        
        // create HTTP request and send them over the socket
        // we need a field for the user-agent, because Wikipedia will blocked the IP otherwise
        boost::asio::streambuf l_request;
        std::ostream l_request_stream(&l_request);
        l_request_stream << "GET " << p_path << " HTTP/1.1\r\n";
        l_request_stream << "Host: " << p_server << "\r\n";
        l_request_stream << "Accept: */*\r\n";
        l_request_stream << "User-Agent: " << m_httpagent << "\r\n";
        l_request_stream << "Connection: close\r\n\r\n";
        
        boost::asio::write( m_socket, l_request );
    
        // read first header line and extract HTTP status data
        boost::asio::streambuf l_response;
        boost::asio::read_until(m_socket, l_response, "\r\n");
        std::istream l_response_stream(&l_response);
        
        std::string l_http_version;
        unsigned int l_status;
        std::string l_status_message;
        
        l_response_stream >> l_http_version;
        l_response_stream >> l_status;
        
        std::getline(l_response_stream, l_status_message);
        if (!l_response_stream || l_http_version.substr(0, 5) != "HTTP/")
            throw exception::runtime(_("invalid response"), *this);
        if (p_throw)
            throwHTTPError( l_status );
        
        // read rest header
        boost::asio::read_until(m_socket, l_response, "\r\n\r\n");
        std::string l_header( (std::istreambuf_iterator<char>(l_response_stream)), std::istreambuf_iterator<char>());
        l_header.erase( l_header.end()-4, l_header.end() );
        p_header = l_header;
        
        return l_status;
    }

    
    /** create an exception on the status code
     * @param p_status status code
     **/
    inline void wikipedia::throwHTTPError( const unsigned int& p_status ) const
    {
        switch (p_status) {
            case 0      : throw exception::runtime(_("error while reading socket data"), *this);     
                
            case 203    : throw exception::runtime(_("Non-Authoritative Information"), *this);
            case 204    : throw exception::runtime(_("No Content"), *this);
            case 205    : throw exception::runtime(_("Reset Content"), *this);
            case 206    : throw exception::runtime(_("Partial Content"), *this);
            case 300    : throw exception::runtime(_("Multiple Choices"), *this);
            case 301    : throw exception::runtime(_("Moved Permanently"), *this);
            case 302    : throw exception::runtime(_("Moved Temporarily"), *this);
            case 303    : throw exception::runtime(_("See Other"), *this);
            case 304    : throw exception::runtime(_("Not Modified"), *this);
            case 305    : throw exception::runtime(_("Use Proxy"), *this);
            case 307    : throw exception::runtime(_("Temporary Redirect"), *this);
            case 400    : throw exception::runtime(_("Bad Request"), *this);
            case 401    : throw exception::runtime(_("Unauthorized"), *this);
            case 402    : throw exception::runtime(_("Payment Required"), *this);
            case 403    : throw exception::runtime(_("Forbidden"), *this);
            case 404    : throw exception::runtime(_("Not Found"), *this);
            case 405    : throw exception::runtime(_("Method Not Allowed"), *this);
            case 406    : throw exception::runtime(_("Not Acceptable"), *this);
                
            case 407    : throw exception::runtime(_("Proxy Authentication Required"), *this);
            case 408    : throw exception::runtime(_("Request Time-out"), *this);
            case 409    : throw exception::runtime(_("Conflict"), *this);
            case 410    : throw exception::runtime(_("Gone"), *this);
            case 411    : throw exception::runtime(_("Length Required"), *this);
            case 412    : throw exception::runtime(_("Precondition Failed"), *this);
            case 413    : throw exception::runtime(_("Request Entity Too Large"), *this);
            case 414    : throw exception::runtime(_("Request-URI Too Large"), *this);
            case 415    : throw exception::runtime(_("Unsupported Media Type"), *this);
            case 416    : throw exception::runtime(_("Requested range not satisfiable"), *this);
            case 417    : throw exception::runtime(_("Expectation Failed"), *this);
            case 500    : throw exception::runtime(_("Internal Server Error"), *this);
            case 501    : throw exception::runtime(_("Not Implemented"), *this);
            case 502    : throw exception::runtime(_("Bad Gateway"), *this);
            case 503    : throw exception::runtime(_("Service Unavailable"), *this);
            case 504    : throw exception::runtime(_("Gateway Time-out"), *this);
            case 505    : throw exception::runtime(_("HTTP Version not supported"), *this);
        }
    }
    
    
}}}
#endif
#endif
