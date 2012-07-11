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

#ifdef MACHINELEARNING_LOGGER
#ifndef __MACHINELEARNING_TOOLS_LOGGER_HPP
#define __MACHINELEARNING_TOOLS_LOGGER_HPP

#define MACHINELEARNING_LOGGER_PATHSUFFIX "machinelearning_%%%%%"
#define MACHINELEARNING_LOGGER_FILENAME   "log.txt"

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#include <boost/thread.hpp>
#include <boost/filesystem.hpp>

#ifdef MACHINELEARNING_MPI

#define MACHINELEARNING_LOGGER_MPI_TAG  999
#define MACHINELEARNING_LOGGER_MPI_EOT  "$EOT$"
#include <boost/mpi.hpp>
#include <boost/bind.hpp>

#endif

#include "../exception/exception.h"
#include "language/language.h"


namespace machinelearning { namespace tools { 
    
    namespace fsys  = boost::filesystem;
    #ifdef MACHINELEARNING_MPI
    namespace mpi   = boost::mpi;
    #endif
    

    /** logger class for writing log information 
     * $LastChangedDate$
     * @note there are two defines for setting the logger directory (MACHINELEARNING_LOGGER_PATHSUFFIX) and logger file (MACHINELEARNING_LOGGER_FILENAME). Within the path suffix the % characters will be
     * substitute to a random hexadecimal value and the directory is created under the temporary path. The value of the filename is append to this directory for writing the log items
     * @note for MPI using every process must call startListener and shutdownListener for synchronize the CPUs and the tag for logtargets is set with a preprocessor flag (MACHINELEARNING_LOGGER_MPI_TAG).
     * The singletone object works only "between" the calls start- and shutdownListener because the MPI object must exists. The connection is closed in the shutdown call and use a message
     * that is defined with the preprocessor flog MACHINELEARNING_LOGGER_MPI_EOT.
     * @note If the listener function is used, you don't use the Boost::MPI::Environmental initialisation. Use the default MPI calls of the mpi.h: 
     * @code
        #include <mpi.h>
        MPI::Init_thread( argc, argv, MPI_THREAD_SERIALIZED )
        MPI::Finalize())
     * @endcode
     * The MPI libraries must be compiled with multithread support
     * @todo adding stream operator for writing data to logger
     * @todo removing Boost Thread and change it to OpenMP support
     **/
    class logger
    {
        
        public :
        
            enum logstate {
                none  = 0,
                error = 1,
                warn  = 2,
                info  = 3
            };
                
            static void createInstance( void );
            static logger* getInstance( void );
            void setLevel( const logstate& );
            logstate getLevel( void ) const;
            std::string getFilename( void ) const;
            template<typename T> void write( const logstate&, const T& );
                            
            #ifdef MACHINELEARNING_MPI
            void startListener( const mpi::communicator& );
            void shutdownListener( const mpi::communicator& );
            template<typename T> void write( const mpi::communicator&, const logstate&, const T& );
            #endif
        
        
        
        private : 
        
            /** local instance **/
            static logger* m_instance;
            /** filename for logging output **/
            std::string m_filename;
            /** logstate for writing data **/
            logstate m_logstate;
            /** file handle **/
            std::ofstream m_file;
            /** mutex for locking **/
            boost::mutex m_muxwriter;
        
        
            logger( void );  
            ~logger( void ); 
            logger( const logger& ) {};
            logger& operator=( const logger& );
        
            template<typename T> void logformat( const logstate&, const T&, std::ostringstream& ) const;
            void write2file( const std::ostringstream& );
        
        
            #ifdef MACHINELEARNING_MPI
        
            /** mutex for creating the listener **/
            boost::mutex m_muxlistener;
            /** mutex for finalizing **/
            boost::mutex m_muxfinalize;
            /** bool for running the listener **/
            bool m_listenerrunning;
        
            void listener( const mpi::communicator& );
        
            #endif
        
    };

    

    /** constructor **/
    inline logger::logger( void ) :
        m_filename(),
        m_logstate(none),
        m_muxwriter()
        #ifdef MACHINELEARNING_MPI
        , m_muxlistener(),
        m_muxfinalize(),
        m_listenerrunning(false)
        #endif
    {
        // create temporary path for logging
        fsys::path temppath(fsys::temp_directory_path());
        temppath /= MACHINELEARNING_LOGGER_PATHSUFFIX;
        temppath = fsys::unique_path(temppath);
        
        // create filename with path
        temppath /= MACHINELEARNING_LOGGER_FILENAME;
        m_filename = temppath.string();
    };

    
    /** destructor **/
    inline logger::~logger( void )
    {
        #ifdef MACHINELEARNING_MPI
        m_listenerrunning = false;
        #endif
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
            throw exception::runtime(_("no logger instance exists, run createInstance first"), *this);

        return m_instance;
    }
    
    
    /** creates a logger instance, run on main first **/
    inline void logger::createInstance( void )
    {
        if (m_instance)
            throw exception::runtime(_("loger instance exists"), *this);
            
        m_instance = new logger();
    }
    
    
    /** returns the temporary filename for logging
     * @return string with path and filename
     **/
    inline std::string logger::getFilename( void ) const
    {
        return m_filename;
    }
    
    
    /** set the log state
     * @param p_state state for logging
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
  
        std::ostringstream l_stream;
        l_stream << "local - ";
        logformat(p_state, p_val, l_stream);
        write2file( l_stream );
    }
    
    
    /** create the log format 
     * @param p_state log level
     * @param p_val value for writing
     * @param p_stream reference for input the data
     **/
    template<typename T> inline void logger::logformat( const logstate& p_state, const T& p_val, std::ostringstream& p_stream ) const
    {
        switch (p_state) {
            case info   : p_stream << "[info]       " << p_val;   break;
            case warn   : p_stream << "[warn]       " << p_val;   break;
            case error  : p_stream << "[error]      " << p_val;   break;
                
            default     : throw exception::runtime(_("log state is unkown"), *this);
        }
    }
    
    
    /** writes the output stream to the file with thread locking
     * @param p_data output stream
     **/
    inline void logger::write2file( const std::ostringstream& p_data )
    {
        if (p_data.str().empty())
            return;
        
        // lock will remove with the destructor call
        boost::lock_guard<boost::mutex> l_lock(m_muxwriter);         
        
        if (!m_file.is_open()) {
            fsys::path logpath( m_filename );
            logpath = logpath.remove_filename();
            
            if (!fsys::is_directory(logpath))
                fsys::create_directories( logpath );
                
            m_file.open( m_filename.c_str(), std::ios_base::app );
        }
            
        m_file << p_data.str() << "\n";
        m_file.flush();
    }

    
    
    //======= MPI ==================================================================================================================================
    #ifdef MACHINELEARNING_MPI
    
    /** creates the local listener on CPU 0
      * @note this method must be called on each instance
      * @param p_mpi MPI object
     **/
    inline void logger::startListener( const mpi::communicator& p_mpi )
    {
        if ((p_mpi.size() == 1) || (m_listenerrunning))
            return;
        
        // synchonize all CPUs thread-safe
        boost::lock_guard<boost::mutex> l_lock(m_muxlistener); 
        m_listenerrunning = true;
        
        if (p_mpi.rank() == 0)
            boost::thread l_thread( boost::bind( &logger::listener, this, boost::cref(p_mpi)) );
       
        p_mpi.barrier();
    }
    
    
    /** shutdown the listener thread and synchronize the CPUs
     * @note this method must be called on each instance
     * @param p_mpi MPI object
     **/
    inline void logger::shutdownListener( const mpi::communicator& p_mpi ) {
        if (!m_listenerrunning)
            return;
        
        m_listenerrunning = false;
        boost::lock_guard<boost::mutex> l_lock(m_muxfinalize);
        
        // we create a end-of-transmission message which sends every process except the CPU 0. CPU 0 receives messages until
        // the EOT is transmitted. This is needed because MPI has no timeslot in which the messages must be received
        if (p_mpi.rank() == 0) {
            std::size_t l_eot = p_mpi.size() - 1;
            while (l_eot > 0) {
                while (boost::optional<mpi::status> l_status = p_mpi.iprobe(mpi::any_source, MACHINELEARNING_LOGGER_MPI_TAG)) {
                    std::string l_str;               
                    p_mpi.recv(  l_status->source(), l_status->tag(), l_str );
                
                    if (l_str != MACHINELEARNING_LOGGER_MPI_EOT) {
                        std::ostringstream l_stream;
                        l_stream << l_str;
                        write2file( l_stream );
                    } else
                        l_eot--;
                }
                boost::this_thread::yield();
            }
        } else 
            p_mpi.isend(0, MACHINELEARNING_LOGGER_MPI_TAG, std::string(MACHINELEARNING_LOGGER_MPI_EOT));
        
        p_mpi.barrier();
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
        
        std::ostringstream l_stream;
        l_stream << "CPU " << p_mpi.rank() << " - ";
        logformat(p_state, p_val, l_stream);
        
        if (p_mpi.rank() == 0)
            write2file( l_stream );
        else
            p_mpi.isend(0, MACHINELEARNING_LOGGER_MPI_TAG, l_stream.str());
    }
    
    
    /** thread method that receive the asynchrone messages of the MPI interface.
     * @param p_mpi MPI object
     **/
    inline void logger::listener( const mpi::communicator& p_mpi )
    {
        boost::lock_guard<boost::mutex> l_lock(m_muxfinalize);
        
        while (m_listenerrunning) {
            while (boost::optional<mpi::status> l_status = p_mpi.iprobe(mpi::any_source, MACHINELEARNING_LOGGER_MPI_TAG)) {
                std::string l_str;
                std::ostringstream l_stream;

                p_mpi.recv(  l_status->source(), l_status->tag(), l_str );
                l_stream << l_str;
                write2file( l_stream );
            }
            boost::this_thread::yield();
        }
    }
    
    #endif
    
    
}}
#endif
#endif
