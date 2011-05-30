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

namespace ublas = boost::numeric::ublas;
namespace dim   = machinelearning::dimensionreduce::supervised;
namespace tools = machinelearning::tools;

/** main program
 * @param argc number of arguments
 * @param argv arguments
 **/
int main(int argc, char* argv[]) {
    
    if (argc < 6)
        throw std::runtime_error("you need at least five parameter as input. first HDF file, second number of projected dimensions, third type of labels (string, int, uint, double), forth path to dataset, fifth path to labels");

    // convert string parameter to numerical data
    std::size_t targetdim = 0;
    try {
        targetdim = boost::lexical_cast<std::size_t>(argv[2]);
    } catch (...) {
        throw std::runtime_error("target dimension can not be read");
    }
    
    
    // read source hdf file and data
    tools::files::hdf source( argv[1] );
    ublas::matrix<double> data = source.readBlasMatrix<double>( argv[4], H5::PredType::NATIVE_DOUBLE);
    ublas::matrix<double> project;

    // read label type
    std::string labeltype( argv[3] );
    boost::to_lower(labeltype);
    
    // create projection
    if (labeltype == "int") {
        dim::lda<double, int> lda(targetdim);
        std::vector<int> labels = tools::vector::copy( source.readBlasVector<int>(argv[5], H5::PredType::NATIVE_INT) );
        
        project = lda.map(data, labels);
    }
        
    if (labeltype == "uint") {
        dim::lda<double, unsigned int> lda(targetdim);
        std::vector<unsigned int> labels = tools::vector::copy( source.readBlasVector<unsigned int>(argv[5], H5::PredType::NATIVE_UINT) );
        
        project = lda.map(data, labels);
    }
        
    if (labeltype == "double") {
        dim::lda<double, double> lda(targetdim);
        std::vector<double> labels = tools::vector::copy( source.readBlasVector<double>(argv[5], H5::PredType::NATIVE_DOUBLE) );
        
        project = lda.map(data, labels);
    }

    /*
    if (labeltype == "string") {
    }
    */
     
    if ((project.size1() == 0) || (project.size2() == 0))
        throw std::runtime_error("label type is unkown");
    
    
    
    // create file and write data to hdf
    tools::files::hdf target("lda.hdf5", true);
    target.writeBlasMatrix<double>( "/data",  project, H5::PredType::NATIVE_DOUBLE );
    
    std::cout << "create HDF file \"lda.hdf5\" with dataset \"/data\"" << std::endl;
    return EXIT_SUCCESS;
}