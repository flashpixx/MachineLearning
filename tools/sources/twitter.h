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

#if defined(MACHINELEARNING_SOURCES) && defined(MACHINELEARNING_SOURCES_TWITTER)

#ifndef __MACHINELEARNING_TOOLS_SOURCES_TWITTER_H
#define __MACHINELEARNING_TOOLS_SOURCES_TWITTER_H

#include <limits>
#include <locale>
#include <sstream>
#include <iostream>
#include <json/json.h>
#include <boost/asio.hpp>
#include <boost/regex.hpp> 
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/date_time/time_facet.hpp>
#include <boost/date_time/gregorian/gregorian.hpp> 
#include <boost/date_time/local_time/local_time.hpp>

#include "../../errorhandling/exception.hpp"
#include "../language/language.h"
#include "../function.hpp"


namespace machinelearning { namespace tools { namespace sources {
    
    namespace bip  = boost::asio::ip;
    
    /** class for using Twitter
     * @see https://dev.twitter.com/docs
     * @todo create thread-safe structure
     * @todo adding support for reading timelines (see https://dev.twitter.com/docs/api#timelines )
     **/
    class twitter 
    {
        
        public :
        
            /** inner class for representation of a search tweet **/
            class searchtweet 
            {
                
                public :

                    searchtweet( const unsigned long long&, const boost::local_time::local_date_time&, const std::string&, const language::code&,
                      const std::string&, const unsigned long long&, const std::string&, const unsigned long long&,
                      const ublas::vector<double>&
                    );
                
                    unsigned long long getMessageID( void ) const;
                    boost::local_time::local_date_time getCreateAt( void ) const;
                    std::string getText( void ) const;
                    language::code getLanguage( void ) const;
                    std::string getFromUserName( void ) const;
                    unsigned long long getFromUserID( void ) const;
                    std::string getToUserName( void ) const;
                    unsigned long long getToUserID( void ) const;
                    ublas::vector<double> getGeoPosition( void ) const;
                    std::vector<std::string> getHashtags( void ) const;
                
                    friend std::ostream& operator<< ( std::ostream&, const searchtweet& );
                
                private :
                
                    /** message id **/
                    unsigned long long m_msgid;
                    /** create at **/
                    boost::local_time::local_date_time m_createat;
                    /** message text **/
                    std::string m_text;
                    /** message language **/
                    language::code m_lang;
                    /** from user text **/
                    std::string m_fromuser;
                    /** from user id **/
                    unsigned long long m_fromuserid;
                    /** to user name **/
                    std::string m_touser;
                    /** to user id **/
                    unsigned long long m_touserid;
                    /** geo poisition with a ublas::vector **/
                    ublas::vector<double> m_geoposition;
                    /** vector with hashtags **/
                    std::vector<std::string> m_hashtags;
            };
        
        
            /** inner class of the timeline tweets **/
            class timelinetweet 
            {
                
                public :
                
                    timelinetweet( const unsigned long long&, const boost::local_time::local_date_time&, const std::string&, const ublas::vector<double>&,
                                  const std::vector<std::string>&, const std::vector<std::string>&, const unsigned long long&, const std::string&, const language::code& );
                    unsigned long long getMessageID( void ) const;
                    boost::local_time::local_date_time getCreateAt( void ) const;
                    std::string getText( void ) const;
                    ublas::vector<double> getGeoPosition( void ) const;
                    std::vector<std::string> getHashtags( void ) const;
                    std::vector<std::string> getURLs( void ) const;
                    unsigned long long getUserID( void ) const;
                    std::string getUserName( void ) const;
                    language::code getUserLanguage( void ) const;
                
                    friend std::ostream& operator<< ( std::ostream&, const timelinetweet& );
                
                private :
                
                    /** message id **/
                    unsigned long long m_msgid;
                    /** create at **/
                    boost::local_time::local_date_time m_createat;
                    /** message text **/
                    std::string m_text;
                    /** geo poisition with a ublas::vector **/
                    ublas::vector<double> m_geoposition;
                    /** vector with hashtags **/
                    std::vector<std::string> m_hashtags;
                    /** vector with urls **/
                    std::vector<std::string> m_urls;
                    /** user id **/
                    unsigned long long m_userid;
                    /** user name **/
                    std::string m_uname;
                    /** user language **/
                    language::code m_ulang;
            };

        
        
            /** inner class for the twitter optional search parameters **/
            class searchparameter 
            {
                
                public :
                
                    /** type of the resultset **/
                    enum resulttype {
                        mixed   = 0,
                        recent  = 1,
                        popular = 2
                    };
                
                    /** type of radius length **/
                    enum radiuslength {
                        kilometer = 0,
                        miles     = 1
                    };
                
                    /** struct of the geoposition **/
                    struct geoposition {
                        float latitude;
                        float longitude;
                        float radius;
                        radiuslength length;
                        
                        geoposition() : latitude(0), longitude(0), radius(0), length(kilometer) {}
                    };
                
                    searchparameter( void );
                    void setGeoPosition( const geoposition& );
                    void setUntilDate( const boost::gregorian::date& );
                    void setLanguage( const language::code& );
                    void setResultType( const resulttype& );
                    void setNumberResults( const std::size_t&, const std::size_t& = 1 );
                    void setSinceMessageID( const unsigned long long& );
                
                    friend std::ostream& operator<< ( std::ostream&, const searchparameter& );
        
                
                private :
                    
                    /** value witch data is used (binary coded) **/
                    std::size_t m_use;
                    
                    /** language code ISO 639-1 coded**/
                    language::code m_lang;
                    /** result type **/
                    std::string m_resulttype;
                    /** date value like YYYY-MM-DD for time-based searches **/
                    std::string m_until;
                    /** geoposition like latitude,longitude,radius_radiuslength **/
                    std::string m_geo;
                    /** number of results per page **/
                    std::size_t m_rpp;
                    /** number of result pages **/
                    std::size_t m_rpage;
                    /** since message id **/
                    unsigned long long m_msgid;
                
            };
        
        
            twitter( void );
            void setHTTPAgent( const std::string& );
            
            std::vector<searchtweet> search( const std::string&, const std::size_t& = 0 ); 
            std::vector<searchtweet> search( const std::string&, const searchparameter&, const std::size_t& = 0 );
            std::vector<searchtweet> refresh( const std::size_t& = 0 );
        
            std::vector<twitter::timelinetweet> getPublicTimeline( void );
        
            std::vector<std::string> getDailyTrends( void );
        
            ~twitter( void );
        
        
        private :
        
            /** io service objekt for resolving the server name of the search server **/
            boost::asio::io_service m_iosearch;
            /** socket objekt for send / receive the data of search calls **/
            bip::tcp::socket m_socketsearch; 
            /** resolver of search connect **/
            bip::tcp::endpoint m_resolvesearch;
        
            /** io service objekt for resolving the server name of the api server **/
            boost::asio::io_service m_ioapi;
            /** socket objekt for send / receive the data of api calls **/
            bip::tcp::socket m_socketapi; 
            /** resolver of api connect **/
            bip::tcp::endpoint m_resolveapi;
        
            /** name for the HTTP agent **/
            std::string m_httpagent;
            /** search parameter **/
            searchparameter m_searchparameter;
            /** refresh URL **/
            std::string m_refreshurl;
        
            std::string sendRequest( bip::tcp::socket&, const std::string&, const std::string& );
            void throwHTTPError( const unsigned int& ) const;   
            std::string urlencode( const std::string& ) const;
            std::vector<searchtweet> runSearchQuery( const std::string&, const std::size_t& );
            void extractSearchResult( const Json::Value&, const std::size_t&, std::vector<searchtweet>& ) const;
    };
    
    
    
    /** constructor **/
    inline twitter::twitter( void ) :
        m_iosearch(),
        m_socketsearch(m_iosearch),
        m_ioapi(),
        m_socketapi(m_ioapi),
        m_httpagent("Machine Learning Framework"),
        m_searchparameter(),
        m_refreshurl()
    {
        boost::system::error_code l_error = boost::asio::error::host_not_found;

        // determine IP of the twitter seach server
        bip::tcp::resolver l_resolversearch(m_iosearch);
        bip::tcp::resolver::query l_searchquery("search.twitter.com", "http");
        
        for(bip::tcp::resolver::iterator l_endpoint = l_resolversearch.resolve( l_searchquery ); (l_error && l_endpoint != bip::tcp::resolver::iterator()); l_endpoint++) {
            m_socketsearch.close();
            m_resolvesearch = *l_endpoint;
            m_socketsearch.connect(*l_endpoint, l_error);
        }
        
        m_socketsearch.close();
        if (l_error)
            throw exception::runtime(_("can not connect to twitter search server"), *this);
        
        
        // determine IP of the twitter api server
        l_error = boost::asio::error::host_not_found;
        
        // determine IP of the twitter seach server
        bip::tcp::resolver l_resolverapi(m_ioapi);
        bip::tcp::resolver::query l_apiquery("api.twitter.com", "http");
        
        for(bip::tcp::resolver::iterator l_endpoint = l_resolverapi.resolve( l_apiquery ); (l_error && l_endpoint != bip::tcp::resolver::iterator()); l_endpoint++) {
            m_socketapi.close();
            m_resolveapi = *l_endpoint;
            m_socketapi.connect(*l_endpoint, l_error);
        }
        
        m_socketapi.close();
        if (l_error)
            throw exception::runtime(_("can not connect to twitter api server"), *this);
    }
    
    
    /** destructor **/
    inline twitter::~twitter( void )
    {
        m_socketsearch.close();
        m_socketapi.close();
    }
    
    
    /** sets the name of the HTTP agent
     * @param p_agent name
     **/
    inline void twitter::setHTTPAgent( const std::string& p_agent) 
    {
        if (p_agent.empty())
            throw exception::runtime(_("HTTP agent name need not be empty"), *this);
        m_httpagent = p_agent;
    }
    
    
    /** run a search with the last set of searchparameters / default parameters
     * @param p_search search string
     * @param p_number returning number of tweets (0 = maximum)
     * @return vector with tweets
     **/
    inline std::vector<twitter::searchtweet> twitter::search( const std::string& p_search, const std::size_t& p_number )
    {
        return search( p_search, m_searchparameter, p_number );
    }
    
    
    /** run search with search parameters
     * @param p_search search string
     * @param p_params search parameter
     * @param p_number returning number of tweets (0 = maximum)
     * @return vector with tweets
     **/
    inline std::vector<twitter::searchtweet> twitter::search( const std::string& p_search, const searchparameter& p_params, const std::size_t& p_number )
    {
        if (p_search.empty())
            throw exception::runtime(_("search query need not be empty"), *this);
        
        m_searchparameter = p_params;
        
        // create GET query (with default values)
        std::ostringstream l_query;
        l_query << "?q=" << function::urlencode(p_search) << "&" << p_params;
        
        return runSearchQuery(l_query.str(), p_number);
    }
    
    
    /** refreshs the last query 
     * @param p_number returning number of tweets (0 = maximum)
     * @return vectors with tweet
     **/
    inline std::vector<twitter::searchtweet> twitter::refresh( const std::size_t& p_number )
    {
        if (m_refreshurl.empty())
            throw exception::runtime(_("refresh query need not be empty"), *this);
        
        return runSearchQuery(m_refreshurl, p_number);
    }
    
    
    /** runs a query and extract the Json data
     * @param p_query stringstream with query
     * @param p_number returning number of tweets (0 = maximum)
     * @return vector with tweet data
     * @todo check if twitter supports persistent connections, it seems there is no support (Connection: Keep-Alive)
     **/
    inline std::vector<twitter::searchtweet> twitter::runSearchQuery( const std::string& p_query, const std::size_t& p_number )
    {
        // run request
        boost::system::error_code l_error = boost::asio::error::host_not_found;
        
        // resulting vector for data and Json parser and clear the refresh url
        std::vector<twitter::searchtweet> l_result;
        Json::Reader l_jsonreader;
        m_refreshurl.clear();
        
        // we read all pages if needed
        for(std::string l_query(p_query); (!l_query.empty()) && ((p_number == 0) || (l_result.size() < p_number)); ) {

            // create connection
            m_socketsearch.connect(m_resolvesearch, l_error);
            if (l_error)
                throw exception::runtime(_("can not connect to twitter search server"), *this);
            
            const std::string l_json = sendRequest( m_socketsearch, "/search.json"+l_query, "search.twitter.com" );
            m_socketsearch.close();
        
            // if no data received we break
            if (l_json.empty())
                break;
            
            // do Json parsing
            Json::Value l_resultroot;
            if (!l_jsonreader.parse( l_json, l_resultroot ))
                throw exception::runtime(_("JSON data can not be parsed"), *this);
            
            // if we can find a refreh url we save it
            if (Json::stringValue == l_resultroot["refresh_url"].type())
                m_refreshurl = l_resultroot["refresh_url"].asString();
            
            // if property set to read all data, we check for a next page value
            // otherwise we set stop value
            l_query.clear();
            if (Json::stringValue == l_resultroot["next_page"].type())
                l_query = l_resultroot["next_page"].asString();
            
            // extract data if exists
            if (Json::arrayValue == l_resultroot["results"].type())
                extractSearchResult( l_resultroot["results"], p_number, l_result );
        }

        if (l_result.size() == 0)
            throw exception::runtime(_("no data received"), *this);
        
        return l_result;
    }
    
    /** extracts the Json result data
     * @param p_array Json array with result data
     * @param p_number returning number of tweets (0 = maximum)
     * @param p_result result vector with data
     **/
    inline void twitter::extractSearchResult( const Json::Value& p_array, const std::size_t& p_number, std::vector<twitter::searchtweet>& p_result ) const
    {
        for(std::size_t i=0; (i < p_array.size()) && ((p_number == 0) || (p_result.size() < p_number)); ++i) {
            Json::Value l_element = p_array[ static_cast<int>(i) ];
            
            if ( (Json::stringValue == l_element["from_user"].type()) &&
                 (Json::stringValue == l_element["iso_language_code"].type()) &&
                 (Json::stringValue == l_element["text"].type()) &&
                 (Json::stringValue == l_element["id_str"].type()) &&
                 (Json::stringValue == l_element["from_user_id_str"].type())
                )
            {
                
                // get geo position of the element
                ublas::vector<double> l_geo(0);
                if (Json::objectValue == l_element["geo"].type()) {
                    Json::Value l_geoelement = l_element["geo"];
                    
                    if (Json::arrayValue == l_geoelement["coordinates"].type()) {
                        Json::Value l_coordinates = l_geoelement["coordinates"];
                        l_geo = ublas::vector<double>(l_coordinates.size(), 0);
                        
                        for(std::size_t n=0; n < l_coordinates.size(); ++n)
                            l_geo(n) = l_coordinates[ static_cast<int>(n) ].asDouble();
                    }
                }
                
                // the language code can be different to the iso codes
                language::code l_lang = language::EN;
                try {
                    l_lang = language::fromString(l_element["iso_language_code"].asString());
                } catch (...) {}
            
                // the "to user id" must not be set, so we try it
                unsigned long long l_touser = 0;
                try {
                    l_touser = boost::lexical_cast<unsigned long long>(l_element["to_user_id_str"].asString());
                } catch (...) {}
                
                // convert the timestamp
                boost::local_time::local_date_time l_datetime(boost::date_time::not_a_date_time);
                if (Json::stringValue == l_element["created_at"].type()) {
                    std::istringstream l_datestream(l_element["created_at"].asString());
                    
                    // parsing data to a local time type (format pointer will be destroyed automatically)
                    l_datestream.imbue( std::locale( std::locale::classic(), new boost::local_time::local_time_input_facet("%a, %d %b %Y %H:%M:%S %q")) );
                    l_datestream >> l_datetime;
                }

                
                // create tweet object with data (we read the string representation of the ids
                // and convert them with boost::lexial_cast)
                twitter::searchtweet l_tweet
                (
                    boost::lexical_cast<unsigned long long>(l_element["id_str"].asString()),
                    l_datetime,
                    l_element["text"].asString(),
                    l_lang,
                    l_element["from_user"].asString(),
                    boost::lexical_cast<unsigned long long>(l_element["from_user_id_str"].asString()),
                    l_element["to_user"].asString(),
                    l_touser,
                    l_geo
                );
                
                p_result.push_back(l_tweet);
            }
        }
    }
        
    
    /** read the public timeline (only 20 tweets, refresh all 60 seconds)
     * @return 
     **/
    inline std::vector<twitter::timelinetweet> twitter::getPublicTimeline( void )
    {
        boost::system::error_code l_error = boost::asio::error::host_not_found;
        
        m_socketapi.connect(m_resolveapi, l_error);
        if (l_error)
            throw exception::runtime(_("can not connect to twitter search server"), *this);
        
        // create GET query (with default values)
        std::ostringstream l_query;
        l_query << "/1/statuses/public_timeline.json?include_entities=true";
        
        const std::string l_json = sendRequest( m_socketapi, l_query.str(), "api.twitter.com" );
        m_socketapi.close();
        
        // do Json parsing
        Json::Value l_resultroot;
        Json::Reader l_jsonreader;
        if (!l_jsonreader.parse( l_json, l_resultroot ))
            throw exception::runtime(_("JSON data can not be parsed"), *this);
        
        if (Json::arrayValue != l_resultroot.type())
            throw exception::runtime(_("no result data is found"), *this);
        
        std::vector<twitter::timelinetweet> l_result;
        for(std::size_t i=0; i < l_resultroot.size(); ++i) {
            Json::Value l_element = l_resultroot[ static_cast<int>(i) ];
            
            if ( (Json::stringValue == l_element["created_at"].type()) &&
                (Json::stringValue == l_element["text"].type()) &&
                (Json::stringValue == l_element["id_str"].type()) &&
                (Json::objectValue == l_element["user"].type())
               )
            {
                // get user object
                Json::Value l_userelement = l_element["user"];
                if ( (Json::stringValue != l_userelement["id_str"].type()) || (Json::stringValue != l_userelement["lang"].type()) || (Json::stringValue != l_userelement["screen_name"].type())  )
                    continue;
                
                std::string l_message = l_element["text"].asString();
                boost::trim( l_message );
                if ( (l_message.empty()) || (l_userelement["screen_name"].asString().empty()) )
                    continue;
                
                
                // convert the timestamp
                boost::local_time::local_date_time l_datetime(boost::date_time::not_a_date_time);
                std::istringstream l_datestream(l_element["created_at"].asString());

                // parsing data to a local time type (format pointer will be destroyed automatically)
                l_datestream.imbue( std::locale( std::locale::classic(), new boost::local_time::local_time_input_facet("%a %b %d %H:%M:%S %q %Y")) );
                l_datestream >> l_datetime;
                //std::cout << l_element["created_at"].asString() << " " << l_datetime << std::endl;
                
                // get geo position if exists
                ublas::vector<double> l_geo(0);
                if (Json::objectValue == l_element["geo"].type()) {
                    Json::Value l_geoelement = l_element["geo"];
                    
                    if (Json::arrayValue == l_geoelement["coordinates"].type()) {
                        Json::Value l_coordinates = l_geoelement["coordinates"];
                        l_geo = ublas::vector<double>(l_coordinates.size(), 0);
                        
                        for(std::size_t n=0; n < l_coordinates.size(); ++n)
                            l_geo(n) = l_coordinates[ static_cast<int>(n) ].asDouble();
                    }
                }
                
                // get hashtags & urltags if exists
                std::vector<std::string> l_hashtags;
                std::vector<std::string> l_urltags;
                
                if (Json::objectValue == l_element["entities"].type()) {
                    Json::Value l_entities = l_element["entities"];
                    
                    if (Json::arrayValue == l_entities["hashtags"].type()) {
                        Json::Value l_hash = l_entities["hashtags"];
                        
                        for(std::size_t n=0; n < l_hash.size(); ++n) {
                            Json::Value l_hashelement = l_hash[ static_cast<int>(n) ];
                            if ( (Json::stringValue == l_hashelement["text"].type()) && (!l_hashelement["text"].asString().empty()) )
                                l_hashtags.push_back( l_hashelement["text"].asString() );
                        }
                    }
                    
                    
                    if (Json::arrayValue == l_entities["urls"].type()) {
                        Json::Value l_url = l_entities["urls"];
                     
                        for(std::size_t n=0; n < l_url.size(); ++n) {
                            Json::Value l_urlelement = l_url[ static_cast<int>(n) ];
                            if ( (Json::stringValue == l_urlelement["text"].type()) && (!l_urlelement["text"].asString().empty()) )
                                l_urltags.push_back( l_urlelement["text"].asString() );
                        }
                    }
                }
                
                // the language code can be different to the iso codes
                language::code l_lang = language::EN;
                try {
                    l_lang = language::fromString(l_userelement["lang"].asString());
                } catch (...) {}
                
                
                // create tweet object with data (we read the string representation of the ids
                // and convert them with boost::lexial_cast)
                twitter::timelinetweet l_tweet
                (
                    boost::lexical_cast<unsigned long long>(l_element["id_str"].asString()),
                    l_datetime,
                    l_message,
                    l_geo,
                    l_hashtags,
                    l_urltags,
                    boost::lexical_cast<unsigned long long>(l_userelement["id_str"].asString()),
                    l_userelement["screen_name"].asString(),
                    l_lang
                );
                
                l_result.push_back( l_tweet );
            }
        }
        return l_result;
    }
    
    
    /** read the daily twitter trends
     * @return trends in a vector
     * @see https://dev.twitter.com/docs/api/1/get/trends/daily
     **/
    inline std::vector<std::string> twitter::getDailyTrends( void )
    {
        boost::system::error_code l_error = boost::asio::error::host_not_found;
        
        m_socketapi.connect(m_resolveapi, l_error);
        if (l_error)
            throw exception::runtime(_("can not connect to twitter search server"), *this);
        
        // create GET query (with default values)
        std::ostringstream l_query;
        l_query << "/1/trends/daily.json";
        
        
        const std::string l_json = sendRequest( m_socketapi, l_query.str(), "api.twitter.com" );
        m_socketapi.close();
        
        // do Json parsing
        Json::Value l_resultroot;
        Json::Reader l_jsonreader;
        if (!l_jsonreader.parse( l_json, l_resultroot ))
            throw exception::runtime(_("JSON data can not be parsed"), *this);
        
        if (Json::objectValue != l_resultroot.type())
            throw exception::runtime(_("no result data is found"), *this);
        
        // get content
        std::vector<std::string> l_searchtags;
        
        if (Json::objectValue == l_resultroot["trends"].type()) {
            Json::Value l_trends = l_resultroot["trends"];
            std::vector<std::string> l_hours  = l_trends.getMemberNames();
            
            for(std::size_t i=0; i < l_hours.size(); ++i)
                
                if (Json::arrayValue == l_trends[l_hours[i]].type())
                    for(std::size_t n=0; n < l_trends[l_hours[i]].size(); ++n) {
                        std::string l_tag = l_trends[l_hours[i]][ static_cast<int>(n) ]["name"].asString();
            
                        if (std::find( l_searchtags.begin(), l_searchtags.end(), l_tag ) == l_searchtags.end())
                            l_searchtags.push_back(l_tag);
                    }
        }
        
        return l_searchtags;
    }
    
    
    /** sends the HTTP request to the Twitter server and receives the header and returns the data
     * @param p_socket socket object
     * @param p_query query call
     * @param p_server server name
     * @return answer data without header
     **/
    inline std::string twitter::sendRequest( bip::tcp::socket& p_socket, const std::string& p_query, const std::string& p_server )
    {
        // create HTTP request and send them over the socket        
        boost::asio::streambuf l_request;
        std::ostream l_request_stream(&l_request);
        l_request_stream << "GET " << p_query << " HTTP/1.1\r\n";
        l_request_stream << "Host: " << p_server << "\r\n";
        l_request_stream << "Accept: */*\r\n";
        l_request_stream << "User-Agent: " << m_httpagent << "\r\n";
        l_request_stream << "Connection: close\r\n";
        l_request_stream << "\r\n";
        
        boost::asio::write( p_socket, l_request );
        
        // read first header line and extract HTTP status data
        boost::asio::streambuf l_response;
        boost::asio::read_until(p_socket, l_response, "\r\n");
        std::istream l_response_stream(&l_response);
        
        std::string l_http_version;
        unsigned int l_status;
        std::string l_status_message;
        
        l_response_stream >> l_http_version;
        l_response_stream >> l_status;
        
        std::getline(l_response_stream, l_status_message);
        if (!l_response_stream || l_http_version.substr(0, 5) != "HTTP/")
            throw exception::runtime(_("invalid response"), *this);
        
        // status code 403 == reached maximum number of tweets, so we stop with an empty result
        if (l_status == 403)
            return std::string();
        throwHTTPError( l_status );
        
        // read rest header until "double CR/LR"
        boost::asio::read_until(p_socket, l_response, "\r\n\r\n");
        // read each headerline, because on the socket can be more data than the header
        // so we read them until the header ends
        std::string l_header;
        while (std::getline(l_response_stream, l_header) && l_header != "\r")
            ;
        
        // read content data into a string stream
        std::ostringstream l_content( std::stringstream::binary );
        
        if (l_response.size() > 0)
            l_content << &l_response;
        
        boost::system::error_code l_error;
        while (boost::asio::read(p_socket, l_response, boost::asio::transfer_at_least(1), l_error))
            ;
        
        if (l_error != boost::asio::error::eof)
            throw exception::runtime(_("data can not be received"), *this);

        l_content << &l_response;

        return l_content.str();
    }
    
    
    /** create an exception on the status code
     * @param p_status status code
     **/
    inline void twitter::throwHTTPError( const unsigned int& p_status ) const
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
    

    //======= Searchparameter ===========================================================================================================================
    
    
    /** constructor **/
    inline twitter::searchparameter::searchparameter( void ) :
        m_use(0),
        m_lang( language::EN ),
        m_resulttype(),
        m_until(),
        m_geo(),
        m_rpp( 0 ),
        m_rpage( 0 ),
        m_msgid( 0 )
    {}
    
    /** set the search language
     * @param p_lang language option
     **/
    inline void twitter::searchparameter::setLanguage( const language::code& p_lang )
    {
        m_lang = p_lang;
        m_use  = m_use | 1;
    }
    
    /** set the result type
     * @param p_res result type
     **/
    inline void twitter::searchparameter::setResultType( const resulttype& p_res )
    {
        switch (p_res) {
            case mixed   :      m_resulttype = "mixed";   break;
            case popular :      m_resulttype = "popular"; break;
            case recent  :      m_resulttype = "recent";  break;
                
            default : throw exception::runtime(_("option value is unkown"), *this);
        }
        m_use    = m_use | 2;
    }
    
    /** set the date value
     * @param p_date time struct
     **/
    inline void twitter::searchparameter::setUntilDate( const boost::gregorian::date& p_date )
    {
        if (p_date.is_special())
            throw exception::runtime(_("not a valid date"), *this);
        
        std::stringstream l_stream;
        
        boost::gregorian::date_facet* l_format = new boost::gregorian::date_facet();
        l_format->format("%Y-%m-%d");
        l_stream.imbue(std::locale(std::locale::classic(), l_format)); 
                
        l_stream << p_date;

        m_until = l_stream.str();
        m_use   = m_use | 4;
    }
    
    /** set the geo position
     * @param p_geo geostruct
     **/
    inline void twitter::searchparameter::setGeoPosition( const geoposition& p_geo )
    {
        std::stringstream l_stream;
        
        l_stream << p_geo.latitude << ",";
        l_stream << p_geo.longitude << ",";
        l_stream << std::abs(p_geo.radius);
        switch (p_geo.length) {
            case kilometer :    l_stream << "km"; break;
            case miles     :    l_stream << "mi"; break;
        }
        
        m_geo = l_stream.str();
        m_use = m_use | 8;
    }

    
    /** sets the number of results
     * @param p_rpp results per page
     * @param p_rpage number of page (begins with 1)
     **/
    inline void twitter::searchparameter::setNumberResults( const std::size_t& p_rpp, const std::size_t& p_rpage )
    {
        if ((p_rpp == 0) || (p_rpp > 100))
            throw exception::runtime(_("results per page must be in the range [1,100]"), *this);
        if (p_rpage * p_rpp > 1500)
            throw exception::runtime(_("maximum results are reached"), *this);
        
        m_rpp   = p_rpp;
        m_rpage = p_rpage;
        m_use   = m_use | 16;
    }
    
    
    /** sets the message id for the option "since message id"
     * @param p_id message id
     **/
    inline void twitter::searchparameter::setSinceMessageID( const unsigned long long& p_id )
    {
        m_msgid = p_id;
        m_use   = m_use | 32;
    }
    
    
    /** overloaded << operator for creating the string representation of the object
     * @param p_stream output stream
     * @param p_obj object
     * @return result stream with added data
     **/
    inline std::ostream& operator<< ( std::ostream& p_stream, const twitter::searchparameter& p_obj )
    {
        if (p_obj.m_use & 1)
            p_stream << "lang=" << language::toString(p_obj.m_lang) << "&";
         
        if (p_obj.m_use & 2)
            p_stream << "result_type=" << p_obj.m_resulttype << "&";
         
        if (p_obj.m_use & 4)
            p_stream << "until=" << p_obj.m_until << "&";
         
        if (p_obj.m_use & 8)
            p_stream << "geocode=" << p_obj.m_geo << "&";
        
        if (p_obj.m_use & 16) 
            p_stream << "rpp=" << p_obj.m_rpp << "&page=" << p_obj.m_rpage << "&";
        
        if (p_obj.m_use & 32)
            p_stream << "since_id=" << p_obj.m_msgid << "&";

        return p_stream;
    }

    
    
    //======= Searchtweet ===============================================================================================================================
    
    inline twitter::searchtweet::searchtweet( const unsigned long long& p_msgid, const boost::local_time::local_date_time& p_createat, const std::string& p_text, const language::code& p_lang,
          const std::string& p_fromuser, const unsigned long long& p_fromuserid, const std::string& p_touser, const unsigned long long& p_touserid,
          const ublas::vector<double>& p_geo
          ) :
        m_msgid(p_msgid), 
        m_createat(p_createat), 
        m_text(p_text), 
        m_lang(p_lang), 
        m_fromuser(p_fromuser), 
        m_fromuserid(p_fromuserid), 
        m_touser(p_touser), 
        m_touserid(p_touserid), 
        m_geoposition(p_geo),
        m_hashtags()
    {
        // extract all #<word> as labels
        boost::smatch l_what;
        std::string::const_iterator it    = m_text.begin();
        std::string::const_iterator l_end = m_text.end();

        const boost::regex l_labelpattern( "#([[:alnum:]]+)", boost::regex_constants::perl );
        
        while (boost::regex_search(it, l_end, l_what, l_labelpattern)) {
            m_hashtags.push_back( l_what[1] );
            it = l_what[0].second;
        }
    }
    
    
    /** returns the message id
     * @return message id
     **/
    inline unsigned long long twitter::searchtweet::getMessageID( void ) const { return m_msgid; }
    
    
    /** returns the "create at" field
     * @return time
     **/
    inline boost::local_time::local_date_time twitter::searchtweet::getCreateAt( void ) const { return m_createat; }
    
    
    /** returns the tweet text
     * @return text
     **/
    inline std::string twitter::searchtweet::getText( void ) const { return m_text; }
    
    
    /** returns the language
     * @return language code
     **/
    inline language::code twitter::searchtweet::getLanguage( void ) const { return m_lang; }
    
    
    /** returns the "from user name"
     * @return name
     **/
    inline std::string twitter::searchtweet::getFromUserName( void ) const { return m_fromuser; }
    
    /** returns the "from user id"
     * @return name
     **/
    inline unsigned long long twitter::searchtweet::getFromUserID( void ) const { return m_fromuserid; }
    
    
    /** returns the "to user name"
     * @return name
     **/
    inline std::string twitter::searchtweet::getToUserName( void ) const { return m_touser; }
    
    
    /** returns the "to user id"
     * @return name
     **/
    inline unsigned long long twitter::searchtweet::getToUserID( void ) const { return m_touserid; }
    
    
    /** returns the geo position
     * @return ublas vector
     **/
    inline ublas::vector<double> twitter::searchtweet::getGeoPosition( void ) const { return m_geoposition; }
    
    
    /** returns the hashtags of the tweet
     * @return vector with tags
     **/
    inline std::vector<std::string> twitter::searchtweet::getHashtags( void ) const { return m_hashtags; }
    
    
    /** overloaded << operator for creating the string representation of the object
     * @param p_stream output stream
     * @param p_obj object
     * @return result stream with added data
     **/
    inline std::ostream& operator<< ( std::ostream& p_stream, const twitter::searchtweet& p_obj )
    {
        p_stream << p_obj.m_msgid;
        
        if ( !p_obj.m_createat.is_special() )
            p_stream << " " << _("at") << " " << p_obj.m_createat;
            
        p_stream << " [" << language::toString(p_obj.m_lang) << "] ";
        
        p_stream << p_obj.m_fromuser << " (" << p_obj.m_fromuserid << ")";
        if ((!p_obj.m_touser.empty()) && (p_obj.m_touserid > 0))
            p_stream << " " << _("to") << " " << p_obj.m_touser << " (" << p_obj.m_touserid << ")";
        
        p_stream << ": " << p_obj.m_text;
        
        if (p_obj.m_geoposition.size() > 0)
            p_stream << " [" << _("geoposition") << ": " << p_obj.m_geoposition << "]";
        
        if (p_obj.m_hashtags.size() > 0) {
            p_stream << " [" << _("hashtags") << ":";
            for (std::size_t i=0; i < p_obj.m_hashtags.size(); ++i)
                p_stream << " " << p_obj.m_hashtags[i];
            p_stream << "]";
        }
        
        return p_stream;
    }
    
    

    //======= Timelinetweet =============================================================================================================================
    
    inline twitter::timelinetweet::timelinetweet( const unsigned long long& p_msgid, const boost::local_time::local_date_time& p_createat, const std::string& p_text, const ublas::vector<double>& p_geo,
                                                 const std::vector<std::string>& p_hashtags, const std::vector<std::string>& p_urls, const unsigned long long& p_uid, const std::string& p_uname,
                                                 const language::code& p_ulang ) :
        m_msgid( p_msgid ),
        m_createat( p_createat ),
        m_text( p_text ),
        m_geoposition( p_geo ),
        m_hashtags( p_hashtags ),
        m_urls( p_urls ),
        m_userid( p_uid ),
        m_uname( p_uname ),
        m_ulang( p_ulang )
    {}
    
    
    /** returns the message id
     * @return message id
     **/
    inline unsigned long long twitter::timelinetweet::getMessageID( void ) const { return m_msgid; }
    
    
    /** returns the "create at" field
     * @return time
     **/
    inline boost::local_time::local_date_time twitter::timelinetweet::getCreateAt( void ) const { return m_createat; }
    
    
    /** returns the tweet text
     * @return text
     **/
    inline std::string twitter::timelinetweet::getText( void ) const { return m_text; }

    
    /** returns the "user id"
     * @return name
     **/
    inline unsigned long long twitter::timelinetweet::getUserID( void ) const { return m_userid; }
    
    
    /** returns the "user name"
     * @return name
     **/
    inline std::string twitter::timelinetweet::getUserName( void ) const { return m_uname; }
    

    /** returns the geo position
     * @return ublas vector
     **/
    inline ublas::vector<double> twitter::timelinetweet::getGeoPosition( void ) const { return m_geoposition; }
    
    
    /** returns the hashtags of the tweet
     * @return vector with tags
     **/
    inline std::vector<std::string> twitter::timelinetweet::getHashtags( void ) const { return m_hashtags; }
    
    
    /** returns the urltags of the tweet
     * @return vector with urls
     **/
    inline std::vector<std::string> twitter::timelinetweet::getURLs( void ) const { return m_urls; }
    
    
    /** user language
     * @return language
     **/
    inline language::code twitter::timelinetweet::getUserLanguage( void ) const { return m_ulang; }
    

    /** overloaded << operator for creating the string representation of the object
     * @param p_stream output stream
     * @param p_obj object
     * @return result stream with added data
     **/
    inline std::ostream& operator<< ( std::ostream& p_stream, const twitter::timelinetweet& p_obj )
    {
        p_stream << p_obj.m_msgid;
        
        if ( !p_obj.m_createat.is_special() )
            p_stream << " " << _("at") << " " << p_obj.m_createat;
        
        p_stream << " " << p_obj.m_uname << " (" << p_obj.m_userid << ")";
        p_stream << " [" << language::toString(p_obj.m_ulang) << "] ";
        p_stream << ": " << p_obj.m_text;
        
        if (p_obj.m_geoposition.size() > 0)
            p_stream << " [" << _("geoposition") << ": " << p_obj.m_geoposition << "]";
        
        if (p_obj.m_hashtags.size() > 0) {
            p_stream << " [" << _("hashtags") << ":";
            for (std::size_t i=0; i < p_obj.m_hashtags.size(); ++i)
                p_stream << " " << p_obj.m_hashtags[i];
            p_stream << "]";
        }
        
        if (p_obj.m_urls.size() > 0) {
            p_stream << " [" << _("urltags") << ":";
            for (std::size_t i=0; i < p_obj.m_urls.size(); ++i)
                p_stream << " " << p_obj.m_urls[i];
            p_stream << "]";
        }
        
        return p_stream;
    }
    
    
}}}
#endif
#endif
