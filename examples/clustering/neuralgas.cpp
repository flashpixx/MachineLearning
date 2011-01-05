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
 # along with this program. If not, see <http://www.gnu.org/licenses/>.     #
 ############################################################################
 @endcond
 **/

#include <machinelearning.h>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/numeric/ublas/matrix.hpp>

namespace ublas     = boost::numeric::ublas;
namespace cluster   = machinelearning::clustering::nonsupervised;
namespace distance  = machinelearning::distances;
namespace tools     = machinelearning::tools;
#ifdef MACHINELEARNING_MPI
namespace mpi       = boost::mpi;
#endif


int main(int argc, char* argv[]) {
    
    #ifdef MACHINELEARNING_MPI
    mpi::environment loMPIenv(argc, argv);
    mpi::communicator loMPICom;
    #endif
        
    
    if (argc < 4)
        throw std::runtime_error("you need at least three parameter as input. first HDF file, second path to dataset, third number of prototypes, optional forth number of iterations or/and (fifth) \"log\" for logging");
    
    // convert string parameter to numerical data
    std::size_t numprotos = 0;
    try {
        numprotos = boost::lexical_cast<std::size_t>(argv[3]);
    } catch (...) {
        throw std::runtime_error("target dimension can not be read");
    }
    
    // convert iteration or logging if exists
    std::size_t iteration = 15;
    bool log = false;
    try {
        std::string forth(  (argc > 4) ? argv[4] : ""  );
        std::string fifth(  (argc > 5) ? argv[5] : ""  );
        
        if (!forth.empty())
            boost::to_lower( forth );
        if (!fifth.empty())
            boost::to_lower( fifth );
        
        if (forth == "log")
            log = true;
        else
            if (!forth.empty())
                iteration = boost::lexical_cast<std::size_t>(forth);
        
        if (fifth == "log")
            log = true;
    } catch (...) {
        throw std::runtime_error("numerical data can not be read");
    }
    
    
    // read source hdf file and data 
    tools::files::hdf source( argv[1] );
    ublas::matrix<double> data = source.readMatrix<double>(argv[2], H5::PredType::NATIVE_DOUBLE);
    
        
    // create distance object, neural gas object and enable logging
    distance::euclid<double> d;
    cluster::neuralgas<double> ng(d, numprotos, data.size2());
    ng.setLogging(log);
    
    
    #ifdef MACHINELEARNING_MPI
    mpi::timer t;
    ng.train(loMPICom, data, iteration);
    std::cout << "number of process: " << loMPICom.size() << " Time: " << t.elapsed() << std::endl;
    
    
    // collect all data (of each process)
    ublas::matrix<double> protos = ng.getPrototypes(loMPICom);
    ublas::vector<double> qerror;
    std::vector< ublas::matrix<double> > logproto;
    
    if (ng.getLogging()) {
        qerror      = tools::vector::copy(ng.getLoggedQuantizationError(loMPICom));
        logproto    = ng.getLoggedPrototypes(loMPICom);
    }
    
    
    // only process 0 writes hdf
    if (loMPICom.rank() == 0) {
        tools::files::hdf target("neuralgas.hdf5", true);

        target.write<double>( "/protos",  protos, H5::PredType::NATIVE_DOUBLE );
        target.write<double>( "/numprotos",  numprotos, H5::PredType::NATIVE_DOUBLE );
        target.write<std::size_t>( "/iteration",  iteration, H5::PredType::NATIVE_ULONG );
        
        if (ng.getLogging()) {
            target.write<double>( "/error", qerror, H5::PredType::NATIVE_DOUBLE );
            for(std::size_t i=0; i < logproto.size(); ++i)
                target.write<double>("/log" + boost::lexical_cast<std::string>( i ), logproto[i], H5::PredType::NATIVE_DOUBLE );
        }
    }
    
    #else    
    
    // train prototypes
    ng.train(data, iteration);
    
    // create target file
    tools::files::hdf target("neuralgas.hdf5", true);
    target.write<double>( "/numprotos",  numprotos, H5::PredType::NATIVE_DOUBLE );
    target.write<double>( "/protos",  ng.getPrototypes(), H5::PredType::NATIVE_DOUBLE );    
    target.write<std::size_t>( "/iteration",  iteration, H5::PredType::NATIVE_ULONG );
    
    if (ng.getLogging()) {
        target.write<double>( "/error",  tools::vector::copy(ng.getLoggedQuantizationError()), H5::PredType::NATIVE_DOUBLE );
        std::vector< ublas::matrix<double> > logproto =  ng.getLoggedPrototypes();
        for(std::size_t i=0; i < logproto.size(); ++i)
            target.write<double>("/log" + boost::lexical_cast<std::string>( i ), logproto[i], H5::PredType::NATIVE_DOUBLE );
    }
    
    #endif
    
    
    #ifdef MACHINELEARNING_MPI
    if (loMPICom.rank() == 0)
    #endif
    std::cout << "create HDF file \"neuralgas.hdf5\" with dataset \"/protos \", \"/iteration\" number of iteration, \"numprotos\" number of prototypes and if logging is enabled \"/error\" with quantization error and \"/log<0 to iterations-1>\" logged prototypes" << std::endl;
    return EXIT_SUCCESS;
}