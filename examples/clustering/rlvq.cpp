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
namespace cluster   = machinelearning::clustering::supervised;
namespace distance  = machinelearning::distances;
namespace tools     = machinelearning::tools;

/** main program
 * @param argc number of arguments
 * @param argv arguments
 **/
int main(int argc, char* argv[]) {
    
    if (argc < 5)
        throw std::runtime_error("you need at least five parameter as input. first HDF file, second type of labels (string, int, uint, double), third path to dataset, forth path to labels, optional fifth number of iterations or/and (sixth) \"log\" for logging");
    
    // convert iteration or logging if exists
    std::size_t iteration = 15;
    bool log = false;
    try {
        std::string fifth(  (argc > 5) ? argv[5] : ""  );
        std::string sixth(  (argc > 6) ? argv[6] : ""  );
        
        if (!sixth.empty())
            boost::to_lower( sixth );
        if (!fifth.empty())
            boost::to_lower( fifth );
        
        if (fifth == "log")
            log = true;
        else
            if (!fifth.empty())
                iteration = boost::lexical_cast<std::size_t>(fifth);
        
        if (sixth == "log")
            log = true;
    } catch (...) {
        throw std::runtime_error("numerical data can not be read");
    }
    
    
    // read source hdf file and data
    tools::files::hdf source( argv[1] );
    ublas::matrix<double> data = source.readBlasMatrix<double>( argv[3], H5::PredType::NATIVE_DOUBLE);
    ublas::matrix<double> prototypes;
    
    // read label type
    std::string labeltype( argv[2] );
    boost::to_lower(labeltype);
    
    // create target file
    tools::files::hdf target("rlvq.hdf5", true);
    
    
    // create data
    distance::euclid<double> d;
    
    if (labeltype == "int") {
        std::vector<int> labels = tools::vector::copy( source.readBlasVector<int>(argv[4], H5::PredType::NATIVE_INT) );
        std::vector<int> unique = tools::vector::unique(labels);
        
        // create rlvq object with double for data and int for labels
        cluster::rlvq<double, int> rlvq(d, unique, data.size2());
        rlvq.setLogging(log);
        
        // train data
        rlvq.train( data, labels, iteration );
        prototypes = rlvq.getPrototypes();
        
        // write data to hdf
        target.writeValue<double>( "/numprotos",  unique.size(), H5::PredType::NATIVE_DOUBLE );
        target.writeBlasVector<int>( "/protolabel", tools::vector::copy(unique), H5::PredType::NATIVE_INT );
        
        // if logging exists write data to file
        if (rlvq.getLogging()) {
            target.writeBlasVector<double>( "/error",  tools::vector::copy(rlvq.getLoggedQuantizationError()), H5::PredType::NATIVE_DOUBLE );
            std::vector< ublas::matrix<double> > p = rlvq.getLoggedPrototypes();
            for(std::size_t i=0; i < p.size(); ++i)
                target.writeBlasMatrix<double>("/log" + boost::lexical_cast<std::string>( i ), p[i], H5::PredType::NATIVE_DOUBLE );
        }
    }
    
    if (labeltype == "uint") {
        std::vector<unsigned int> labels = tools::vector::copy( source.readBlasVector<unsigned int>(argv[4], H5::PredType::NATIVE_UINT) );
        std::vector<unsigned int> unique = tools::vector::unique(labels);
        
        // create rlvq object with double for data and uint for labels
        cluster::rlvq<double, unsigned int> rlvq(d, unique, data.size2());
        rlvq.setLogging(log);
        
        // train data
        rlvq.train( data, labels, iteration );
        prototypes = rlvq.getPrototypes();
        
        // write data to hdf
        target.writeValue<double>( "/numprotos",  unique.size(), H5::PredType::NATIVE_DOUBLE );
        target.writeBlasVector<unsigned int>( "/protolabel", tools::vector::copy(unique), H5::PredType::NATIVE_UINT );
        
        // if logging exists write data to file
        if (rlvq.getLogging()) {
            target.writeBlasVector<double>( "/error",  tools::vector::copy(rlvq.getLoggedQuantizationError()), H5::PredType::NATIVE_DOUBLE );
            std::vector< ublas::matrix<double> > p = rlvq.getLoggedPrototypes();
            for(std::size_t i=0; i < p.size(); ++i)
                target.writeBlasMatrix<double>("/log" + boost::lexical_cast<std::string>( i ), p[i], H5::PredType::NATIVE_DOUBLE );
        }
    }
    
    if (labeltype == "double") {
        std::vector<double> labels = tools::vector::copy( source.readBlasVector<double>(argv[4], H5::PredType::NATIVE_DOUBLE) );
        std::vector<double> unique = tools::vector::unique(labels);
        
        // create rlvq object with double for data and double for labels
        cluster::rlvq<double, double> rlvq(d, unique, data.size2());
        rlvq.setLogging(log);
        
        // train data
        rlvq.train( data, labels, iteration );
        prototypes = rlvq.getPrototypes();
        
        // write data to hdf
        target.writeValue<double>( "/numprotos",  unique.size(), H5::PredType::NATIVE_DOUBLE );
        target.writeBlasVector<double>( "/protolabel", tools::vector::copy(unique), H5::PredType::NATIVE_DOUBLE );
        
        // if logging exists write data to file
        if (rlvq.getLogging()) {
            target.writeBlasVector<double>( "/error",  tools::vector::copy(rlvq.getLoggedQuantizationError()), H5::PredType::NATIVE_DOUBLE );
            std::vector< ublas::matrix<double> > p = rlvq.getLoggedPrototypes();
            for(std::size_t i=0; i < p.size(); ++i)
                target.writeBlasMatrix<double>("/log" + boost::lexical_cast<std::string>( i ), p[i], H5::PredType::NATIVE_DOUBLE );
        }
    }

    /*
    if (labeltype == "string") {
    }
    */
     
    if ((prototypes.size1() == 0) || (prototypes.size2() == 0))
        throw std::runtime_error("label type is unkown");    
    
    target.writeValue<std::size_t>( "/iteration",  iteration, H5::PredType::NATIVE_ULONG );
    target.writeBlasMatrix<double>( "/protos",  prototypes, H5::PredType::NATIVE_DOUBLE );

    
    std::cout << "create HDF file \"rlvq.hdf5\" with dataset \"/protos \", \"/protolabel\" label of prototypes, \"/iteration\" number of iteration, \"numprotos\" number of prototypes and if logging is enabled \"/error\" with quantization error and \"/log<0 to iterations-1>\" logged prototypes" << std::endl;
    
    return EXIT_SUCCESS;
}