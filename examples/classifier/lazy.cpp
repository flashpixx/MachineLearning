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