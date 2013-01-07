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
#error never include this file directly
#elif !defined __MACHINELEARNING_TOOLS_LOGGER_IMPLEMENTATION_HPP
#define __MACHINELEARNING_TOOLS_LOGGER_IMPLEMENTATION_HPP


#include <cassert>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#include <boost/thread.hpp>
#include <boost/filesystem.hpp>

#ifdef MACHINELEARNING_MPI
#include <boost/mpi.hpp>
#include <boost/bind.hpp>
#endif

#include "../errorhandling/exception.hpp"
#include "language/language.h"


namespace machinelearning { namespace tools { 


    namespace fsys  = boost::filesystem;
    #ifdef MACHINELEARNING_MPI
    namespace mpi   = boost::mpi;
    #endif

     

    /** constructor
     * @param p_pathsuffix path suffix
     * @param p_filename log filename
     **/
    inline logger::logger( const std::string& p_pathsuffix, const std::string& p_filename ) :
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
        if (!p_pathsuffix.empty())
            temppath /= p_pathsuffix;
        else
            temppath /= "machinelearning_%%%%%%";
        temppath = fsys::unique_path(temppath);
        
        // create filename with path
        if (!p_filename.empty())
            temppath /= p_filename;
        else 
            temppath /= "log.txt";

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
        assert(m_instance);
        return m_instance;
    }


    /** checks if a logger instance exists
     * @return bool
     **/
    inline bool logger::exists( void )
    {
        return m_instance;
    }


    /** creates a logger instance, run on main first
     * @param p_pathsuffix path suffix
     * @param p_filename log filename
     **/
    inline void logger::createInstance( const std::string& p_pathsuffix, const std::string& p_filename )
    {
        assert(!m_instance);
        m_instance = new logger( p_pathsuffix, p_filename );
    }


    /** release logger instance, run before return in main **/
    inline void logger::releaseInstance( void )
    {
        assert(m_instance);
        delete m_instance;
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
    inline void logger::setLevel( const logger::logstate& p_state )
    {
        m_logstate = p_state;
    }


    /** returns the status of logging
     * @return logger state
     **/
    inline logger::logstate logger::getLevel( void ) const
    {
        return m_logstate;
    }


    /** writes the data in the local log file 
     * @param p_state log level
     * @param p_val value
     **/
    template<typename T> inline void logger::write( const logger::logstate& p_state, const T& p_val )
    {
        if ( !(
               #ifndef MACHINELEARNING_NDEBUG
               (p_state == assert) || 
               #endif
               (p_state == exception)) && ( (m_logstate == none) || (p_state == none) || (p_state > m_logstate) )
            )
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
    template<typename T> inline void logger::logformat( const logger::logstate& p_state, const T& p_val, std::ostringstream& p_stream ) const
    {
        switch (p_state) {
            case info       : p_stream << "[info]       " << p_val;   break;
            case warn       : p_stream << "[warn]       " << p_val;   break;
            #ifndef MACHINELEARNING_NDEBUG
            case assert     : p_stream << "[assert]     " << p_val;   break;
            #endif
            case error      : p_stream << "[error]      " << p_val;   break;
            case exception  : p_stream << "[exception]  " << p_val;   break;
                
            default :
                ;
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
            boost::thread l_thread( boost::bind( &machinelearning::tools::logger::listener, this, boost::cref(p_mpi)) );
        
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
                while (boost::optional<mpi::status> l_status = p_mpi.iprobe(mpi::any_source, m_mpitag)) {
                    std::string l_str;               
                    p_mpi.recv(  l_status->source(), l_status->tag(), l_str );
                    
                    if (l_str != m_mpieot) {
                        std::ostringstream l_stream;
                        l_stream << l_str;
                        write2file( l_stream );
                    } else
                        l_eot--;
                }
                boost::this_thread::yield();
            }
        } else 
            p_mpi.isend(0, m_mpitag, m_mpieot);
        
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
            p_mpi.isend(0, m_mpitag, l_stream.str());
    }


    /** thread method that receive the asynchrone messages of the MPI interface.
     * @param p_mpi MPI object
     **/
    inline void logger::listener( const mpi::communicator& p_mpi )
    {
        boost::lock_guard<boost::mutex> l_lock(m_muxfinalize);
        
        while (m_listenerrunning) {
            while (boost::optional<mpi::status> l_status = p_mpi.iprobe(mpi::any_source, m_mpitag)) {
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


} }

#endif
#endif
