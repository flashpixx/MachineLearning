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


namespace po    = boost::program_options;
namespace ublas = boost::numeric::ublas;
namespace dim   = machinelearning::dimensionreduce::nonsupervised;
namespace tools = machinelearning::tools;


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
    std::size_t l_dimension;
    std::string l_outpath;

    // create CML options with description
    po::options_description l_description("allowed options");
    l_description.add_options()
        ("help", "produce help message")
        ("infile", po::value<std::string>(), "input file")
        ("inpath", po::value<std::string>(), "input path of the datapoint within the input file")
        ("outfile", po::value<std::string>(), "output HDF5 file")
        ("outpath", po::value<std::string>(&l_outpath)->default_value("/pca"), "output path within the HDF5 file [default: /pca]")
        ("dimension", po::value<std::size_t>(&l_dimension)->default_value(3), "target dimension [default: 3]")
    ;

    po::variables_map l_map;
    po::positional_options_description l_input;
    po::store(po::command_line_parser(p_argc, p_argv).options(l_description).positional(l_input).run(), l_map);
    po::notify(l_map);

    if (l_map.count("help")) {
        std::cout << l_description << std::endl;
        return EXIT_SUCCESS;
    }

    if ( (!l_map.count("infile")) || (!l_map.count("inpath")) || (!l_map.count("outfile")) ) {
        std::cerr << "[--infile], [--inpath] and [--outfile] option must be set" << std::endl;
        return EXIT_FAILURE;
    }


    // read source hdf file
    tools::files::hdf source( l_map["infile"].as<std::string>() );

    // create pca object and map the data
    dim::pca<double> l_pca( l_dimension );
    const ublas::matrix<double> l_project = l_pca.map( source.readBlasMatrix<double>( l_map["inpath"].as<std::string>(), tools::files::hdf::NATIVE_DOUBLE) );


    // create file and write data to hdf
    tools::files::hdf target( l_map["outfile"].as<std::string>(), true);
    target.writeBlasMatrix<double>( l_outpath,  l_project, tools::files::hdf::NATIVE_DOUBLE );

    return EXIT_SUCCESS;
}
