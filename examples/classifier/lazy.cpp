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
namespace tools      = machinelearning::tools;


int main(std::size_t argc, char* argv[]) {

    if (argc < 6)
        throw std::runtime_error("you need at least five parameter as input. first HDF file, second type of labels (string, int, uint, double), third path to dataset, forth path to labels, fifth type of distance (invert, distance), optional sixth number of iterations or/and (seventh) \"log\" for logging");
    
    // convert iteration or logging if exists
    std::size_t iteration = 15;
    bool log = false;
    try {
        std::string sixth(  (argc > 6) ? argv[6] : ""  );
        std::string seventh(  (argc > 7) ? argv[7] : ""  );
               
        if (!sixth.empty())
            boost::to_lower( sixth );
        if (!seventh.empty())
            boost::to_lower( seventh );
        
        if (sixth == "log")
            log = true;
        else
            if (!sixth.empty())
                iteration = boost::lexical_cast<std::size_t>(sixth);
        
        if (seventh == "log")
            log = true;
    } catch (...) {
        throw std::runtime_error("numerical data can not be read");
    }
    
    // distance value
    std::string fifth( argv[5] )
    if (!fifth.empty())
        boost::to_lower( fifth );
    
    std::size_t distancetyp = 0;
    if (fifth == "distance")
        distancetyp = 1;
    else
        if (fifth == "invert")
            distancetyp = 2;
        else
            throw std::runtime_error("distance unkwon");
    
    
    // read source hdf file and data
    tools::files::hdf source( argv[1] );
    ublas::matrix<double> data = source.readMatrix<double>( argv[3], H5::PredType::NATIVE_DOUBLE);
    ublas::matrix<double> prototypes;
    
    // read label type
    std::string labeltype( argv[2] );
    boost::to_lower(labeltype);
    
    // create target file
    tools::files::hdf target("lazy.hdf5", true);
    
    
    // create projection
    distance::euclid<double> d;
    
    if (labeltype == "int") {
        std::vector<int> labels = tools::vector::copy( source.readVector<int>(argv[4], H5::PredType::NATIVE_INT) );
        std::vector<int> unique = tools::vector::unique(labels);
        
        // create lazy learner object with double for data and int for labels
        cluster::rlvq<double, int> rlvq(d, unique, data.size2());
        rlvq.setLogging(log);
        
        // train data
        rlvq.train( data, labels, iteration );
        prototypes = rlvq.getPrototypes();
        
        // write data to hdf
        target.write<double>( "/numprotos",  unique.size(), H5::PredType::NATIVE_DOUBLE );
        target.write<int>( "/protolabel", tools::vector::copy(unique), H5::PredType::NATIVE_INT );
        
        // if logging exists write data to file
        if (rlvq.getLogging()) {
            target.write<double>( "/error",  tools::vector::copy(rlvq.getLoggedQuantizationError()), H5::PredType::NATIVE_DOUBLE );
            std::vector< ublas::matrix<double> > p = rlvq.getLoggedPrototypes();
            for(std::size_t i=0; i < p.size(); ++i)
                target.write<double>("/log" + boost::lexical_cast<std::string>( i ), p[i], H5::PredType::NATIVE_DOUBLE );
        }
    }
    
    
    
    /*
     ublas::matrix<double> data       = o.readMatrix<double>("/iris/data", H5::PredType::NATIVE_DOUBLE); 
     ublas::vector<std::size_t> label = o.readVector<std::size_t>("/iris/label", H5::PredType::NATIVE_ULONG); 
     
     
     ublas::indirect_array<> x(10);
     tl::random l_rand;
     for(std::size_t i=0; i < x.size(); ++i)
     x(i) = static_cast<std::size_t>(l_rand.get<double>( tl::random::uniform, 0, data.size1()));
     
     ublas::indirect_array<> y(data.size2());
     for(std::size_t i=0; i < y.size(); ++i)
     y(i) = i;
     
     
     ublas::matrix<double>      blub1 = ublas::project(data, x, y);
     ublas::vector<std::size_t> blub2 = ublas::project(label, x);
     
     dist::euclid<double> d;
     nd::knn<double> k(d, 4);
     cl::lazylearner<double, std::size_t> ll(k, cl::lazylearner<double, std::size_t>::inversedistance);
     
     ll.setDatabase( blub1,  tl::vector::copy(blub2) );
     
     
     tl::files::hdf f("lazy.hdf5", true);
     f.write<double>( "/data",  data, H5::PredType::NATIVE_DOUBLE );
     f.write<double>( "/label",  tl::vector::copy( ll.use(data) ), H5::PredType::NATIVE_DOUBLE ); 
     f.write<double>( "/basedata",  blub1, H5::PredType::NATIVE_DOUBLE );
     f.write<double>( "/baselabel",  blub2, H5::PredType::NATIVE_DOUBLE );
     */
}