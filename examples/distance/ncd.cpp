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

#include <cstdlib>
#include <machinelearning.h>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/options_description.hpp>


namespace po = boost::program_options;
using namespace boost::numeric;
using namespace machinelearning;


/** main program
 * @param p_argc number of arguments
 * @param p_argv arguments
 **/
int main(int p_argc, char* p_argv[])
{
    #ifdef MACHINELEARNING_MULTILANGUAGE
    tools::language::bindings::bind();
    #endif
    
    // default values
    std::string l_compress;
    std::string l_algorithm;
    std::string l_matrix;

    // create CML options with description
    po::options_description l_description("allowed options");
    l_description.add_options()
        ("help", "produce help message")
        ("outfile", po::value<std::string>(), "output HDF5 file")
        ("sources", po::value< std::vector<std::string> >()->multitoken(), "list of text files or directories with text files (all files in the directory will be read and subdirectories will be ignored)")
        ("compress", po::value<std::string>(&l_compress)->default_value("default"), "compression level (allowed values are: default [default], bestspeed or bestcompression)")
        ("algorithm", po::value<std::string>(&l_algorithm)->default_value("gzip"), "compression algorithm (allowed values are: gzip [default], bzip)")
        ("matrix", po::value<std::string>(&l_matrix)->default_value("symmetric"), "structure of the matrix (allowed values are: symmetric [default] or unsymmetric")
    ;

    po::variables_map l_map;
    po::positional_options_description l_input;
    po::store(po::command_line_parser(p_argc, p_argv).options(l_description).positional(l_input).run(), l_map);
    po::notify(l_map);

    if (l_map.count("help")) {
        std::cout << l_description << std::endl;
        return EXIT_SUCCESS;
    }

    if (!l_map.count("sources"))  {
        std::cerr << "[--sources] must be set" << std::endl;
        return EXIT_FAILURE;
    }



    // create ncd object
    distances::ncd<double> l_ncd( (l_algorithm == "gzip") ? distances::ncd<double>::gzip : distances::ncd<double>::bzip2 );
    if (l_compress == "bestspeed")
        l_ncd.setCompressionLevel( distances::ncd<double>::bestspeed );
    if (l_compress == "bestcompression")
        l_ncd.setCompressionLevel( distances::ncd<double>::bestcompression );


    // create the distance matrix and use the each element of the vector as a filename
    ublas::matrix<double> l_distancematrix;
    if (l_matrix == "unsymmetric")
        l_distancematrix = l_ncd.unsymmetric( l_map["sources"].as< std::vector<std::string> >(), true);
    else
        l_distancematrix = l_ncd.symmetric( l_map["sources"].as< std::vector<std::string> >(), true);


    if (!l_map.count("outfile"))
        std::cout << l_distancematrix << std::endl;
    else {
        // create hdf file and write data
        tools::files::hdf file(l_map["outfile"].as<std::string>(), true);
        file.writeBlasMatrix<double>( "/ncd",  l_distancematrix, tools::files::hdf::NATIVE_DOUBLE );
        std::cout << "structure of the output file" << std::endl;
        std::cout << "/ncd" << "\t\t" << "distance matrix" << std::endl;
    }

    return EXIT_SUCCESS;
}
