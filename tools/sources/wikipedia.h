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
     * @todo optimizing server connection: If article receives, a socket is exists. The Wikipedia server is not changed, if there is no change
     * on the language, so the socket can be open until the language is change
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
            bool isAcronym( void ) const;
            bool isArticle( void ) const;
        
        
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
            /** bool for founded article **/
            bool m_articlefound;
            /** bool for founded acronym page **/
            bool m_acronymfound;
            /** property for saving the last-call article data **/
            wikiarticle m_article;
        
            wikiproperties getProperties( const language& ) const;
            unsigned int sendRequest( const std::string&, const std::string&, std::string&, const bool& = true );
            unsigned int send( const std::string&, const std::string&, std::string&, const bool& = true );
            std::string getContentData( void ); 
            void throwHTTPError( const unsigned int& ) const;   
            wikiarticle parseXML( const std::string& ) const;
        
    };
    
    
    /** constructor
     * @apram p_lang optional language parameter (default de_DE)
     **/
    inline wikipedia::wikipedia( const language& p_lang ) :
        m_defaultproperties( getProperties(p_lang) ),
        m_io(),
        m_socket(m_io),
        m_articlefound( false ),
        m_acronymfound( false )
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
                return l_prop;
                
            case de_DE :
                l_prop.exporturl.host   = "de.wikipedia.org";
                l_prop.exporturl.path   = "/wiki/Spezial:Exportieren/";
                l_prop.randomurl.host   = "de.wikipedia.org";
                l_prop.randomurl.path   = "/wiki/Spezial:Zuf%C3%A4llige_Seite";
                l_prop.acronymref       = "steht für";
                l_prop.acronym          = "Abkürzung";
                l_prop.category         = "Kategorie";
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
        m_articlefound = false;
        
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
        
        // check if the content is acronym page, than extract the acronyms
        const boost::regex l_acronympattern( "'''" + m_article.title + "'''", boost::regex_constants::perl );
        if (boost::regex_search(m_article.content, l_what, l_acronympattern)) {
            m_acronymfound = true;
            return;
        }

        
        /*
         % String für Kommata der Übersicht halber in eigene Variable
         % der Suchberiff steht in '''
         lc = '''''''';
         lc = regexpi(this.gxLastArticle.content, strcat(lc,pcSearch,lc,'.*',this.gxLang.(pcLang).acronymref), 'match');
         
         rl = not(isempty(lc));
        */
        
        // extract category with regular expression \[\[<category name>:(.*?)\]\] (hint: non-greedy excepted)
        const boost::regex l_categorypattern( "\\[\\[" + l_prop.category + ":(.*?)\\]\\]", boost::regex_constants::icase | boost::regex_constants::perl );

        std::string::const_iterator it    = m_article.content.begin();
        std::string::const_iterator l_end = m_article.content.end();
        
        while (boost::regex_search(it, l_end, l_what, l_categorypattern)) {
            std::string l_label(l_what[1]);
            m_article.label.push_back( l_label.substr(0, l_label.find("|")) );
            it = l_what[0].second;
        }
        
        m_articlefound = true;    
    }
    
    
    /** returns the article content
     * @return content
     **/
    inline std::string wikipedia::getArticleContent( void ) const
    {
        if (!m_articlefound)
            throw exception::parameter(_("no article is loaded"));
        
        return m_article.content;
    }
    
    /** returns the article title
     * @return title
     **/
    inline std::string wikipedia::getArticleTitle( void ) const
    {
        if (!m_articlefound)
            throw exception::parameter(_("no article is loaded"));
        
        return m_article.title;
    }
    
    /** return revision id
     * @return article revision id
     **/
    inline std::size_t wikipedia::getArticleRevision( void ) const
    {
        if (!m_articlefound)
            throw exception::parameter(_("no article is loaded"));
        
        return m_article.revisionid;
    }
    
    /** return article id
     * @return article id
     **/
    inline std::size_t wikipedia::getArticleID( void ) const
    {
        if (!m_articlefound)
            throw exception::parameter(_("no article is loaded"));
        
        return m_article.articleid;
    }
    
    /** return article labels
     * @return label
     **/
    inline std::vector<std::string> wikipedia::getArticleLabel( void ) const
    {
        if (!m_articlefound)
            throw exception::parameter(_("no article is loaded"));
        
        return m_article.label;
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
            throw exception::parameter(_("can not find information within the http header"));
        
        // extract Location URL
        const std::string l_url = l_header.substr(l_found+10, l_header.find("\r\n", l_found)-l_found-10);
        
        // extract document of the URL
        l_found = l_url.rfind("/");
        if (l_found == std::string::npos)
            throw exception::parameter(_("can not find wikipedia document within the URL"));
        
        // get the article
        getArticle( l_url.substr(l_found+1), l_prop.lang );
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
            throw exception::parameter(_("XML data can not be parsed"));
        
        // extract the namespace 
        xmlNodePtr l_node = xmlDocGetRootElement( l_xml );
        std::string l_namespace( (char*)xmlGetProp(l_node, (xmlChar*)"schemaLocation") );
        std::size_t l_found = l_namespace.find(" ");
        if ( (l_namespace.empty()) || (l_found == std::string::npos) ) {
            xmlFreeDoc( l_xml );
            xmlCleanupParser();
            
            throw exception::parameter(_("can not detect namespace"));
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
            throw exception::parameter(_("XML data can not be parsed"));

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
            throw exception::parameter(_("data can not received"));
        
        std::string l_data( (std::istreambuf_iterator<char>(l_response_stream)), std::istreambuf_iterator<char>());        
        
        return l_data;
    }
    
    
    /** sends the HTTP request to the Wikipedua server and receives the header
     * @param p_server server adress
     * @param p_path path to the document
     * @param p_header returning HTTP header
     * @param p_throw bool for throwing error
     * @return status code
     **/
    inline unsigned int wikipedia::send( const std::string& p_server, const std::string& p_path, std::string& p_header, const bool& p_throw )
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

    
    /** create DNS and HTTP request and returns the status code and the HTTP header
     * @note method open / creates a socket within the member variable, so after all is done, the socket must be closed manually
     * @param p_server server adress
     * @param p_path path to the document
     * @param p_header returning HTTP header
     * @param p_throw bool for throwing error
     * @return status code
     **/
    inline unsigned int wikipedia::sendRequest( const std::string& p_server, const std::string& p_path, std::string& p_header, const bool& p_throw )
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
            throw exception::parameter(_("can not connect to wikipedia server"));
        
        return send(p_server, p_path, p_header, p_throw);
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
            case 302    : throw exception::parameter(_("Moved Temporarily"));
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