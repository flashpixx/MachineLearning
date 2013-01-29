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

/** implementation for initialization of static member 
 * or different code structures that must run only once
 **/

#include <string>
#include <cstddef>
#include <boost/random.hpp>

#ifdef MACHINELEARNING_MPI
#include <boost/mpi.hpp>
#endif

#ifdef MACHINELEARNING_RANDOMDEVICE
#include <boost/nondet_random.hpp>
#endif


#include "machinelearning.h"


namespace machinelearning {

    /** initialization of the random device **/
    #ifdef MACHINELEARNING_RANDOMDEVICE
    boost::random_device tools::random::m_random;
    #else

        #ifdef MACHINELEARNING_MPI
        boost::mpi::environment l_mpienv;
        boost::mpi::communicator l_mpi;
        boost::mt19937 tools::random::m_random( time(NULL) * (l_mpi.rank()+1) );
        #else
        boost::mt19937 tools::random::m_random( time(NULL) );
        #endif

    #endif


    /** initialization of the logger instance **/
    #ifdef MACHINELEARNING_LOGGER
    tools::logger* tools::logger::m_instance = NULL;

        #ifdef MACHINELEARNING_MPI
        std::size_t tools::logger::m_mpitag = 999;
        std::string tools::logger::m_mpieot = "$EOT$";
        #endif

    #endif
    
}




