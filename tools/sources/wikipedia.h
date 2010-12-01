/** 
 @cond
 ############################################################################
 # LGPL License                                                             #
 #                                                                          #
 # This file is part of the Machine Learning Framework.                     #
 # Copyright (c) 2010, Philipp Kraus, <philipp.kraus@flashpixx.de>          #
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
 # along with this program.  If not, see <http://www.gnu.org/licenses/>.    #
 ############################################################################
 @endcond
 **/

#ifdef SOURCES

#ifndef MACHINELEARNING_TOOLS_SOURCES_WIKIPEDIA_H
#define MACHINELEARNING_TOOLS_SOURCES_WIKIPEDIA_H

#include <algorithm>
#include <boost/asio.hpp>
#include <boost/regex.hpp>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>


#include "../../exception/exception.h"
#include "../language/language.h"


namespace machinelearning { namespace tools { namespace sources {

    namespace bip  = boost::asio::ip;
    
    
    /** class for reading Wikipedia article.
     * The data will received over a HTTP socket for each call and uses the nativ C interface of libxml2 for parsing the XML data.
     * There are no simple C++ interfaces for XML data, so we use the C source within the class. 
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
        
            std::string getArticleContent( void ) const;
            std::string getArticleTitle( void ) const;
            std::size_t getArticleRevision( void ) const;
            std::size_t getArticleID( void ) const;
            std::vector<std::string> getArticleLabel( void ) const;
            std::vector<std::string> getArticleAcronym( void ) const;
            bool isAcronym( void ) const;
            bool isArticle( void ) const;
        
            ~wikipedia( void );
        
        
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
            };
        
            /** struct for article content **/
            struct wikiarticle {
                std::string content;
                std::string title;
                std::size_t articleid;
                std::size_t revisionid;
                std::vector<std::string> label;
            };
        
        
            /** default wikipedia properties **/
            const wikiproperties m_defaultproperties;
            /** io service objekt for resolving the server name**/
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

        
            wikiproperties getProperties( const language& ) const;
            unsigned int sendRequest( const std::string&, const std::string&, std::string&, const bool& = true );
            std::string getContentData( void ); 
            void throwHTTPError( const unsigned int& ) const;   
            wikiarticle parseXML( const std::string& ) const;
        
    };
    
    
    /** constructor
     * @param p_lang optional language parameter (default de_DE)
     **/
    inline wikipedia::wikipedia( const language& p_lang ) :
        m_defaultproperties( getProperties(p_lang) ),
        m_io(),
        m_socket(m_io),
        m_lastserver(),
        m_lastendpoint(),
        m_articlefound( false ),
        m_acronymfound( false ),
        m_acronym()
    {}
    
    
    /** destructor for closing the socket **/
    inline wikipedia::~wikipedia( void )
    {
        m_socket.close();
    }
    

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
                l_prop.category         = "Category";
                return l_prop;
                
            case de_DE :
                l_prop.exporturl.host   = "de.wikipedia.org";
                l_prop.exporturl.path   = "/wiki/Spezial:Exportieren/";
                l_prop.randomurl.host   = "de.wikipedia.org";
                l_prop.randomurl.path   = "/wiki/Spezial:Zuf%C3%A4llige_Seite";
                l_prop.acronymref       = "steht für";
                l_prop.category         = "Kategorie";
                return l_prop;

        }
        
        throw exception::runtime(_("language is not kwon"));
    }
    
    
    /** reads an article
     * @param p_search keyword for searching
     * @param p_lang optional language
     **/
    inline void wikipedia::getArticle( const std::string& p_search, const language& p_lang )
    {
        m_articlefound = false;
        m_acronym.clear();
        
        wikiproperties l_prop = m_defaultproperties;
        if (l_prop.lang != p_lang)
            l_prop = getProperties( p_lang );
        
        
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
        
        // check if exists redirect in the content data, run a new request
        if (m_article.content.find("#redirect") != std::string::npos) {
            getArticle(m_article.content.substr(12, m_article.content.size()-14), p_lang);
            return;
        }

        
        // parse content data (category, acronyms, ...)
        boost::smatch l_what;
        std::string::const_iterator it    = m_article.content.begin();
        std::string::const_iterator l_end = m_article.content.end();

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
        
        // remove languages (short codes are found in ISO 639-1) and labels (at the end of the expression we set a [[:space:]]* for removing CR or LF)
        m_article.content = boost::regex_replace(m_article.content, l_categorypattern, "");
        
        const boost::regex l_langpattern( "\\[\\[(aa|ab|af|am|ar|as|ay|az|ba|be|bg|bh|bi|bn|bo|br|ca|co|cs|cy|da|de|dz|el|en|eo|es|et|eu|fa|fi|fj|fo|fr|fy|ga|gd|gl|gn|gu|ha|hi|he|hr|hu|hy|ia|id|ie|ik|in|is|it|iu|iw|ja|ji|jw|ka|kk|kl|km|kn|ko|ks|ku|ky|la|ln|lo|lt|lv|mg|mi|mk|ml|mn|mo|mr|ms|mt|my|na|ne|nl|no|oc|om|or|pa|pl|ps|pt|qu|rm|rn|ro|ru|rw|sa|sd|sg|sh|si|sk|sl|sm|sn|so|sq|sr|ss|st|su|sv|sw|ta|te|tg|th|ti|tk|tl|tn|to|tr|ts|tt|tw|ug|uk|ur|uz|vi|vo|wo|xh|yi|yo|za|zh|zu):(.*?)\\]\\][[:space:]]*", boost::regex_constants::perl );
        m_article.content = boost::regex_replace(m_article.content, l_langpattern, "");
        
        m_articlefound = true;    
    }
    
    
    /** reads an random article
     * @param p_lang optional language
     **/
    inline void wikipedia::getRandomArticle( const language& p_lang )
    {
        m_articlefound = false;
        
        wikiproperties l_prop = m_defaultproperties;
        if (l_prop.lang != p_lang)
            l_prop = getProperties( p_lang );
        
        std::string l_header;
        unsigned int l_status = sendRequest( l_prop.randomurl.host, l_prop.randomurl.path, l_header, false );
        m_socket.close();
        if (l_status != 302)
            throwHTTPError(l_status);
        
        // read the location header tag and extract the URL
        std::size_t l_found = l_header.find("Location:");
        if (l_found == std::string::npos)
            throw exception::runtime(_("can not find information within the http header"));
        
        // extract Location URL
        const std::string l_url = l_header.substr(l_found+10, l_header.find("\r\n", l_found)-l_found-10);
        
        // extract document of the URL
        l_found = l_url.rfind("/");
        if (l_found == std::string::npos)
            throw exception::runtime(_("can not find wikipedia document within the URL"));
        
        // get the article
        getArticle( l_url.substr(l_found+1), l_prop.lang );
    }
    
    
    
    /** returns the article content
     * @return content
     **/
    inline std::string wikipedia::getArticleContent( void ) const
    {
        if (!m_articlefound)
            throw exception::runtime(_("no article is loaded"));
        
        return m_article.content;
    }
    
    /** returns the article title
     * @return title
     **/
    inline std::string wikipedia::getArticleTitle( void ) const
    {
        if (!m_articlefound)
            throw exception::runtime(_("no article is loaded"));
        
        return m_article.title;
    }
    
    /** return revision id
     * @return article revision id
     **/
    inline std::size_t wikipedia::getArticleRevision( void ) const
    {
        if (!m_articlefound)
            throw exception::runtime(_("no article is loaded"));
        
        return m_article.revisionid;
    }
    
    /** return article id
     * @return article id
     **/
    inline std::size_t wikipedia::getArticleID( void ) const
    {
        if (!m_articlefound)
            throw exception::runtime(_("no article is loaded"));
        
        return m_article.articleid;
    }
    
    /** return article labels
     * @return label
     **/
    inline std::vector<std::string> wikipedia::getArticleLabel( void ) const
    {
        if (!m_articlefound)
            throw exception::runtime(_("no article is loaded"));
        
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
        wikiarticle l_data;
        bool l_error = false;
        
        // convert std::string into char array for memory parsing
        const char* l_xmlcontent = p_xml.c_str();
        xmlDocPtr l_xml           = xmlParseMemory( l_xmlcontent, strlen(l_xmlcontent) );
        if (l_xml == NULL)
            throw exception::runtime(_("XML data can not be parsed"));
        
        // extract the namespace 
        xmlNodePtr l_node = xmlDocGetRootElement( l_xml );
        std::string l_namespace( (char*)xmlGetProp(l_node, (xmlChar*)"schemaLocation") );
        std::size_t l_found = l_namespace.find(" ");
        if ( (l_namespace.empty()) || (l_found == std::string::npos) ) {
            xmlFreeDoc( l_xml );
            xmlCleanupParser();
            
            throw exception::runtime(_("can not detect namespace"));
        }
        l_namespace = l_namespace.substr(0, l_found );
  
        
        
        // create XPath context and namespace (create a prefix "wiki" for searching the XML tree)
        xmlXPathContextPtr l_tree = xmlXPathNewContext( l_xml );
        xmlXPathRegisterNs(l_tree, (xmlChar*)"wiki", (xmlChar*)l_namespace.c_str());
        xmlXPathObjectPtr l_result;

        // extract the content data, search with XPath the node and extract the content
        
        // /mediawiki/page/title              => article title
        // /mediawiki/page/id                 => article id
        // /mediawiki/page/revision/text      => article data
        // /mediawiki/page/revision/id        => revision id

        l_result = xmlXPathEvalExpression( (xmlChar*)"/wiki:mediawiki/wiki:page/wiki:title", l_tree );
        if ( (!l_error) && (!xmlXPathNodeSetIsEmpty(l_result->nodesetval)) && (l_result->nodesetval->nodeNr == 1) )
            l_data.title = std::string(  (char*)xmlNodeListGetString(l_xml, l_result->nodesetval->nodeTab[0]->xmlChildrenNode, 1)  );
        else
            l_error = true;
        xmlXPathFreeObject(l_result);
        
        
        l_result = xmlXPathEvalExpression( (xmlChar*)"/wiki:mediawiki/wiki:page/wiki:id", l_tree );
        if ( (!l_error) && (!xmlXPathNodeSetIsEmpty(l_result->nodesetval)) && (l_result->nodesetval->nodeNr == 1) )
            try {
                l_data.articleid = boost::lexical_cast<std::size_t>( (char*)xmlNodeListGetString(l_xml, l_result->nodesetval->nodeTab[0]->xmlChildrenNode, 1) );
            } catch (...) {
                l_error = true;
            }
        else
            l_error = true;
        xmlXPathFreeObject(l_result);
        
        
        l_result = xmlXPathEvalExpression( (xmlChar*)"/wiki:mediawiki/wiki:page/wiki:revision/wiki:text", l_tree );
        if ( (!l_error) && (!xmlXPathNodeSetIsEmpty(l_result->nodesetval)) && (l_result->nodesetval->nodeNr == 1) )
            l_data.content = std::string(  (char*)xmlNodeListGetString(l_xml, l_result->nodesetval->nodeTab[0]->xmlChildrenNode, 1)  );
        else
            l_error = true;
        xmlXPathFreeObject(l_result);

        
        l_result = xmlXPathEvalExpression( (xmlChar*)"/wiki:mediawiki/wiki:page/wiki:revision/wiki:id", l_tree );
        if ( (!l_error) && (!xmlXPathNodeSetIsEmpty(l_result->nodesetval)) && (l_result->nodesetval->nodeNr == 1) )
            try {
                l_data.revisionid = boost::lexical_cast<std::size_t>( (char*)xmlNodeListGetString(l_xml, l_result->nodesetval->nodeTab[0]->xmlChildrenNode, 1) );
            } catch (...) {
                l_error = true;
            }
        else
            l_error = true;
        xmlXPathFreeObject(l_result);
        
        // clear libxml structure
        xmlXPathFreeContext( l_tree );
        xmlFreeDoc( l_xml );
        xmlCleanupParser();
           
        if (l_error)
            throw exception::runtime(_("XML data can not be parsed"));

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
        
        while (boost::asio::read(m_socket, l_response, boost::asio::transfer_at_least(1), l_error));
        
        if (l_error != boost::asio::error::eof)
            throw exception::runtime(_("data can not be received"));
        
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
        if (p_server != m_lastserver) {
            m_lastserver = p_server;
            
            // create resolver for server
            bip::tcp::resolver l_resolver(m_io);
            bip::tcp::resolver::query l_query(p_server, "http");
        
            // try to connect the server
            bip::tcp::resolver::iterator l_endpoint = l_resolver.resolve( l_query );
            bip::tcp::resolver::iterator l_endpointend;
  
            while (l_error && l_endpoint != l_endpointend) {
                m_socket.close();
                m_lastendpoint = *l_endpoint;
                m_socket.connect(*l_endpoint++, l_error);
            }

        } else
            m_socket.connect(m_lastendpoint, l_error);

            
        if (l_error)
            throw exception::runtime(_("can not connect to wikipedia server"));
        
        
        
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
    
        
        // read the complet HTTP header "double CR/LR"
        boost::asio::streambuf l_response;
        boost::asio::read_until(m_socket, l_response, "\r\n\r\n");
        
        // convert stream data into string and remove the end seperator
        std::istream l_response_stream( &l_response );
        std::string l_header( (std::istreambuf_iterator<char>(l_response_stream)), std::istreambuf_iterator<char>());
        l_header.erase( l_header.end()-4, l_header.end() );

        // copy the return value into a string and seperates the status code
        unsigned int l_status = 0;
        try {
            l_status = boost::lexical_cast<unsigned int>( l_header.substr( l_header.find(" ")+1,3) );
        } catch (...) {}
        
        if ( p_throw )
            throwHTTPError( l_status );
        
        p_header = l_header;
        return l_status;
    }

    
    /** create an exception on the status code
     * @param p_status status code
     **/
    inline void wikipedia::throwHTTPError( const unsigned int& p_status ) const
    {
        switch (p_status) {
            case 0      : throw exception::runtime(_("error while reading socket data"));     
                
            case 203    : throw exception::runtime(_("Non-Authoritative Information"));
            case 204    : throw exception::runtime(_("No Content"));
            case 205    : throw exception::runtime(_("Reset Content"));
            case 206    : throw exception::runtime(_("Partial Content"));
            case 300    : throw exception::runtime(_("Multiple Choices"));
            case 301    : throw exception::runtime(_("Moved Permanently"));
            case 302    : throw exception::runtime(_("Moved Temporarily"));
            case 303    : throw exception::runtime(_("See Other"));
            case 304    : throw exception::runtime(_("Not Modified"));
            case 305    : throw exception::runtime(_("Use Proxy"));
            case 307    : throw exception::runtime(_("Temporary Redirect"));
            case 400    : throw exception::runtime(_("Bad Request"));
            case 401    : throw exception::runtime(_("Unauthorized"));
            case 402    : throw exception::runtime(_("Payment Required"));
            case 403    : throw exception::runtime(_("Forbidden"));
            case 404    : throw exception::runtime(_("Not Found"));
            case 405    : throw exception::runtime(_("Method Not Allowed"));
            case 406    : throw exception::runtime(_("Not Acceptable"));
                
            case 407    : throw exception::runtime(_("Proxy Authentication Required"));
            case 408    : throw exception::runtime(_("Request Time-out"));
            case 409    : throw exception::runtime(_("Conflict"));
            case 410    : throw exception::runtime(_("Gone"));
            case 411    : throw exception::runtime(_("Length Required"));
            case 412    : throw exception::runtime(_("Precondition Failed"));
            case 413    : throw exception::runtime(_("Request Entity Too Large"));
            case 414    : throw exception::runtime(_("Request-URI Too Large"));
            case 415    : throw exception::runtime(_("Unsupported Media Type"));
            case 416    : throw exception::runtime(_("Requested range not satisfiable"));
            case 417    : throw exception::runtime(_("Expectation Failed"));
            case 500    : throw exception::runtime(_("Internal Server Error"));
            case 501    : throw exception::runtime(_("Not Implemented"));
            case 502    : throw exception::runtime(_("Bad Gateway"));
            case 503    : throw exception::runtime(_("Service Unavailable"));
            case 504    : throw exception::runtime(_("Gateway Time-out"));
            case 505    : throw exception::runtime(_("HTTP Version not supported"));
        }
    }
    
    
};};};


#endif
#endif