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

#include <string>
#include <fstream>

#include <boost/thread.hpp>

#ifdef MACHINELEARNING_MPI
#include <boost/mpi.hpp>
#endif


namespace machinelearning { namespace tools { 
    
    #ifdef MACHINELEARNING_MPI
    namespace mpi   = boost::mpi;
    #endif
    

    /** logger class for writing log information 
     * @note there are two defines for setting the logger directory (MACHINELEARNING_LOGGER_PATHSUFFIX) and logger file (MACHINELEARNING_LOGGER_FILENAME). Within the path suffix the % characters will be
     * substitute to a random hexadecimal value and the directory is created under the temporary path. The value of the filename is append to this directory for writing the log items
     * @note for MPI using every process must call startListener and shutdownListener for synchronize the CPUs and the tag for logtargets is set with a preprocessor flag (MACHINELEARNING_LOGGER_MPI_TAG).
     * The singleton object works only "between" the calls start- and shutdownListener because the MPI object must exists. The connection is closed in the shutdown call and use a message
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
                none,
                #ifndef MACHINELEARNING_NDEBUG
                assert,
                #endif
                exception,
                error,
                warn,
                info
            };
                
            static bool exists( void );
            static void createInstance( const std::string& = "", const std::string& = "" );
            static void releaseInstance( void );
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
        
        
            logger( const std::string& = "", const std::string& = "" );  
            ~logger( void ); 
            logger( const logger& ) {};
            logger& operator=( const logger& );
        
            template<typename T> void logformat( const logstate&, const T&, std::ostringstream& ) const;
            void write2file( const std::ostringstream& );
        
        
            #ifdef MACHINELEARNING_MPI
        
            /** ID for the MPI logger message **/
            static std::size_t m_mpitag;
            /** end-of-transmission string for closing the MPI logger listener **/
            static std::string m_mpieot;
            /** mutex for creating the listener **/
            boost::mutex m_muxlistener;
            /** mutex for finalizing **/
            boost::mutex m_muxfinalize;
            /** bool for running the listener **/
            bool m_listenerrunning;
        
            void listener( const mpi::communicator& );
        
            #endif
        
    };
    
    
}}

#include "logger.implementation.hpp"

#endif
#endif
