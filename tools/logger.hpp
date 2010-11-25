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
#include <ctime>

#include <boost/thread.hpp>

#ifdef CLUSTER
#include <boost/mpi.hpp>
#endif

#include "../exception/exception.h"
#include "language/language.h"


namespace machinelearning { namespace tools { 
    
    #ifdef CLUSTER
    namespace mpi   = boost::mpi;
    #endif
    

    /** logger class for writing log information 
     * @todo MPI using with non-blocking operation (every message should send to process 0 and write down - optiomal a thread checks some time if there are new messages)
     **/
    class logger {
        
        public :
        
            enum logstate {
                none  = 0,
                error = 1,
                warn  = 2,
                info  = 3
            };
                
            
            static logger* getInstance( void );
            void setLevel( const logstate& );
            logstate getLevel( void ) const;
            std::string getFilename( void ) const;
            template<typename T> void write( const logstate&, const T& );
                            
            #ifdef CLUSTER
            void createListener( mpi::communicator& );
            template<typename T> void write( const mpi::communicator&, const logstate&, const T& );
            #endif
        
        
        
        private : 
            /** local instance **/
            static logger* m_instance;
        
            /** filename for logging output **/
            static std::string m_filename;
        
            /** logstate for writing data **/
            logstate m_logstate;
        
            /** file handle **/
            std::ofstream m_file;

            /** mutex for locking **/
            boost::mutex m_mutex;
        
        
            logger( void );  
            ~logger( void ); 
            logger( const logger& );
            logger& operator=( const logger& );
        
        
            #ifdef CLUSTER
            
            mpi::communicator* m_mpi;
        
            //class listener {
            //};
        
            #endif
        
    };
    
    
    /** constructor **/
    inline logger::logger( void ) :
        m_logstate(none),
        m_mutex()
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
    
    
    /** writes the data in the local log file 
     * @param p_state log level
     * @param p_val value
     **/
    template<typename T> inline void logger::write( const logstate& p_state, const T& p_val ) {
        if ( (m_logstate == none) || (p_state == none) || (p_state > m_logstate) )
            return;
        
        // lock will remove with the destructor call
        boost::lock_guard<boost::mutex> l_lock(m_mutex);         
        
        if (!m_file.is_open())
            m_file.open( m_filename.c_str(), std::ios_base::app );
        

        switch (p_state) {
            case info   : m_file << "[local info]       ";   break;
            case warn   : m_file << "[local warn]       ";   break;
            case error  : m_file << "[local error]      ";   break;
                
            default     : throw exception::runtime(_("log state is unkown"));
        }
        
        m_file << p_val << "\n";
        m_file.flush();
    }

    
    
    //======= MPI ==================================================================================================================================
    #ifdef CLUSTER
    
    /** creates the local listener on CPU 0
     * @param p_mpi MPI object (no const reference)
     **/
    inline void logger::createListener( mpi::communicator& p_mpi )
    {
       if (p_mpi.rank() != 0)
            throw exception::runtime(_("the listener can be produced only on CPU 0"));
        
        m_mpi = &p_mpi;
        
        // create worker thread 
    }
    
    
    /** write log entry. If the CPU rank == 0 the log will write to the file, on other CPU rank the message
     * is send to the CPU 0 and write down there. The local log state is relevant for writing
     * @param p_mpi MPI object
     * @param p_state log level
     * @param p_val value
     **/     
    template<typename T> inline void logger::write( const mpi::communicator& p_mpi, const logstate& p_state, const T& p_val )
    {
        if ( (m_logstate == none) || (p_state == none) || (p_state > m_logstate) )
            return;
        
        
        if (p_mpi.rank() != 0) {
            // create message and send it with non-blocking to CPU 0
        } else {
            
            // lock will remove with the destructor call
            boost::lock_guard<boost::mutex> l_lock(m_mutex);         
            
            if (!m_file.is_open())
                m_file.open( m_filename.c_str(), std::ios_base::app );
            
            switch (p_state) {
                case info   : m_file << "[CPU 0 info]       ";   break;
                case warn   : m_file << "[CPU 0 warn]       ";   break;
                case error  : m_file << "[CPU 0 error]      ";   break;
                    
                default     : throw exception::runtime(_("log state is unkown"));
            }
            
            m_file << p_val << "\n";
            m_file.flush();
        }
        
    }

    
    #endif
    
    
};};



#endif