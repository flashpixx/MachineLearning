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

namespace ublas      = boost::numeric::ublas;
namespace classifier = machinelearning::classifier;
namespace distance   = machinelearning::distances;
namespace neighbor   = machinelearning::neighborhood;
namespace tools      = machinelearning::tools;


int main(int argc, char* argv[]) {

    if (argc < 7)
        throw std::runtime_error("you need at least five parameter as input. first HDF file, second type of labels (string, int, uint, double), third path to dataset, forth path to labels, fifth path to unkwon data, sixth type of distance (invert, distance), optional seventh for number of neighbours, eighth \"log\" for logging");
    
    // convert logging if exists
    std::size_t knn = 3;
    bool log = false;
    try {
        std::string seventh(  (argc > 7) ? argv[7] : ""  );
        std::string eighth(  (argc > 8) ? argv[8] : ""  );
        
        if (!seventh.empty())
            boost::to_lower( seventh );
        if (!eighth.empty())
            boost::to_lower( eighth );
        
        if (seventh == "log")
            log = true;
        else
            if (!seventh.empty())
                knn = boost::lexical_cast<std::size_t>(seventh);
        
        if (eighth == "log")
            log = true;
    } catch (...) {
        throw std::runtime_error("numerical data can not be read");
    }
    
    // distance value
    std::string sixth( argv[6] );
    if (!sixth.empty())
        boost::to_lower( sixth );
    
    if ((sixth != "distance") || (sixth != "invert"))
        throw std::runtime_error("distance unkwon");
    
    
    // read source hdf file and data
    tools::files::hdf source( argv[1] );
    ublas::matrix<double> data        = source.readMatrix<double>( argv[3], H5::PredType::NATIVE_DOUBLE);
    ublas::matrix<double> unknowndata = source.readMatrix<double>( argv[5], H5::PredType::NATIVE_DOUBLE);

    
    // read label type
    std::string labeltype( argv[2] );
    boost::to_lower(labeltype);
    bool labelcheck = true;
    
    
    // create target file
    tools::files::hdf target("lazy.hdf5", true);
    
    
    // create data
    distance::euclid<double> d;
    neighbor::knn<double> n(d, knn);
    
    if (labeltype == "int") {
        labelcheck = false;
        
        // create lazy learner object with double for data and labels
        classifier::lazylearner<double, int>::weighttype dist;
        if (sixth == "distance")
            dist = classifier::lazylearner<double, int>::distance;
        else
            dist = classifier::lazylearner<double, int>::inversedistance;
        
        // create lazy-learner
        classifier::lazylearner<double, int> lazy(n, dist);
        
        // set data and read new labels for unkown data
        lazy.setLogging(log);
        lazy.setDatabase( data, tools::vector::copy( source.readVector<int>(argv[4], H5::PredType::NATIVE_INT) ) );
        std::vector<int> labels = lazy.use(unknowndata);
        
        
        // write data to hdf
        target.write<int>( "/unkwonlabel", tools::vector::copy(labels), H5::PredType::NATIVE_INT );
        
        // if logging exists write data to file
        if (lazy.getLogging())
            target.write<double>( "/error",  tools::vector::copy(lazy.getLoggedQuantizationError()), H5::PredType::NATIVE_DOUBLE );
    }

    if (labeltype == "uint") {
        labelcheck = false;
        
        // create lazy learner object with double for data and labels
        classifier::lazylearner<double, unsigned int>::weighttype dist;
        if (sixth == "distance")
            dist = classifier::lazylearner<double, unsigned int>::distance;
        else
            dist = classifier::lazylearner<double, unsigned int>::inversedistance;
        
        // create lazy-learner
        classifier::lazylearner<double, unsigned int> lazy(n, dist);
        
        // set data and read new labels for unkown data
        lazy.setLogging(log);
        lazy.setDatabase( data, tools::vector::copy( source.readVector<unsigned int>(argv[4], H5::PredType::NATIVE_UINT) ) );
        std::vector<unsigned int> labels = lazy.use(unknowndata);
        
        
        // write data to hdf
        target.write<unsigned int>( "/unkwonlabel", tools::vector::copy(labels), H5::PredType::NATIVE_UINT );
        
        // if logging exists write data to file
        if (lazy.getLogging())
            target.write<double>( "/error",  tools::vector::copy(lazy.getLoggedQuantizationError()), H5::PredType::NATIVE_DOUBLE );
    }
    
    if (labeltype == "double") {
        labelcheck = false;
        
        // create lazy learner object with double for data and labels
        classifier::lazylearner<double, double>::weighttype dist;
        if (sixth == "distance")
            dist = classifier::lazylearner<double, double>::distance;
        else
            dist = classifier::lazylearner<double, double>::inversedistance;
        
        // create lazy-learner
        classifier::lazylearner<double, double> lazy(n, dist);
        
        // set data and read new labels for unkown data
        lazy.setLogging(log);
        lazy.setDatabase( data, tools::vector::copy( source.readVector<double>(argv[4], H5::PredType::NATIVE_DOUBLE) ) );
        std::vector<double> labels = lazy.use(unknowndata);
        
        
        // write data to hdf
        target.write<double>( "/unkwonlabel", tools::vector::copy(labels), H5::PredType::NATIVE_DOUBLE );
        
        // if logging exists write data to file
        if (lazy.getLogging())
            target.write<double>( "/error",  tools::vector::copy(lazy.getLoggedQuantizationError()), H5::PredType::NATIVE_DOUBLE );
    }
    
    /*
    if (labeltype == "string") {
    }
    */
    
    if (labelcheck)
        throw std::runtime_error("label type is unkown");

    

    std::cout << "create HDF file \"lazy.hdf5\" with dataset \"/unkwonlabel\" for unkwon data, and if logging is enabled \"/error\" with quantization error" << std::endl;
    return EXIT_SUCCESS;
}