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


#ifndef MACHINELEARNING_TOOLS_LOGGER_HPP
#define MACHINELEARNING_TOOLS_LOGGER_HPP

#include <string>
#include <fstream>

#ifdef CLUSTER
#include <boost/mpi.hpp>
#endif

//#include "../exception/exception.h"
#include "language/language.h"


namespace machinelearning { namespace tools { 
    
    #ifdef CLUSTER
    namespace mpi   = boost::mpi;
    #endif
    

    /** logger class for writing log information 
     * @todo writer method
     * @todo thread safe
     * @todo MPI using with non-blocking operation (every message should send to process 0 and write down - optiomal a thread checks some time if there are new messages)
     **/
    class logger {
        
        public :
        
            enum logstate {
                none  = 0,
                info  = 1,
                warn  = 2,
                error = 3
            };
        
        
            
            static logger* getInstance( void );
            void setLevel( const logstate& );
            logstate getLevel( void ) const;
            std::string getFilename( void ) const;
        
            template<typename T> void write( const logstate&, const T& );
                            
         
        
        private : 
            /** local instance **/
            static logger* m_instance;
        
            /** filename for logging output **/
            static std::string m_filename;
        
            /** logstate for writing data **/
            logstate m_logstate;
        
            /** file handle **/
            std::ofstream m_file;

        
            logger( void );  
            ~logger( void ); 
            logger( const logger& );
            logger& operator=( const logger& );
        
    };

    
    template<typename T> inline void logger::write( const logstate& p_state, const T& p_val ) {
        if ( (m_logstate == none) || (p_state == none) || (p_state > m_logstate) )
            return;
        
        if (!m_file.is_open())
            m_file.open( m_filename.c_str(), std::ios_base::app );
        
        
        switch (p_state) {
            case info   : m_file << "[info]\t";
            case warn   : m_file << "[warn]\t";
            case error  : m_file << "[error]\t";
                
            default     : m_file << "[unkown]\t";
        }
        m_file << p_val << "\n";
        m_file.flush();
    }
    
    
    /** constructor **/
    inline logger::logger( void ) :
        m_logstate(none)
        #ifdef CLUSTER
        , m_mpi(NULL)
        #endif
    {};
    
    
    /** copy constructor **/
    inline logger::logger( const logger& ) {}

    
    /** destructor **/
    inline logger::~logger( void )
    {
        m_file.close();
    }
    

    /** equal operator **/
    inline logger& logger::operator=( const logger& )
    {
        return *this;
    }

    
    /** returns the instance
     * @return logger instance
     **/
    inline logger* logger::getInstance()
    {
        if (!m_instance)
            m_instance = new logger();
        
        return m_instance;
    }
    
    
    /** returns the temporary filename for logging
     * @return string with path and filename
     **/
    inline std::string logger::getFilename( void ) const
    {
        return m_filename;
    }
    
    
    /** set the log state
     * @paramp p_state state for logging
     **/
    inline void logger::setLevel( const logstate& p_state )
    {
        m_logstate = p_state;
    }
    
    
    /** returns the status of logging
     * @return boolean status
     **/
    inline logger::logstate logger::getLevel( void ) const
    {
        return m_logstate;
    }
    
    
    
    //======= MPI ==================================================================================================================================
    #ifdef CLUSTER
    
    

    
    #endif
    
    
};};



#endif