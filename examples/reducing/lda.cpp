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
namespace dim   = machinelearning::dimensionreduce::supervised;
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
        ("labelpath", po::value<std::string>(), "label path of the datapoints within the input file")
        ("labeltype", po::value<std::string>(), "datatype of labels (values: string, uint, int)")
        ("outfile", po::value<std::string>(), "output HDF5 file")
        ("outpath", po::value<std::string>(&l_outpath)->default_value("/lda"), "output path within the HDF5 file [default: /lda]")
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

    if ( (!l_map.count("infile")) || (!l_map.count("inpath")) || (!l_map.count("outfile")) || (!l_map.count("labelpath")) || (!l_map.count("labeltype")) ) {
        std::cerr << "[--infile], [--inpath], [--outfile], [--labelpath] and [--labeltype] option must be set" << std::endl;
        return EXIT_FAILURE;
    }


    // read source hdf file and data / create target file
    tools::files::hdf l_target( l_map["outfile"].as<std::string>(), true);
    tools::files::hdf l_source( l_map["infile"].as<std::string>() );
    const ublas::matrix<double> l_data = l_source.readBlasMatrix<double>(l_map["inpath"].as<std::string>(), tools::files::hdf::NATIVE_DOUBLE);


    // lda with uint labels
    if (l_map["labeltype"].as<std::string>() == "uint") {
        dim::lda<double, std::size_t> l_lda( l_dimension );

        const std::vector<std::size_t> l_labels = tools::vector::copy( l_source.readBlasVector<std::size_t>(l_map["labelpath"].as<std::string>(), tools::files::hdf::NATIVE_ULONG) );
        l_target.writeBlasMatrix<double>( l_outpath,  l_lda.map(l_data, l_labels), tools::files::hdf::NATIVE_DOUBLE );
    }

    // lda with int labels
    if (l_map["labeltype"].as<std::string>() == "int") {
        dim::lda<double, std::ptrdiff_t> l_lda( l_dimension );

        const std::vector<std::ptrdiff_t> l_labels = tools::vector::copy( l_source.readBlasVector<std::ptrdiff_t>(l_map["labelpath"].as<std::string>(), tools::files::hdf::NATIVE_LONG) );
        l_target.writeBlasMatrix<double>( l_outpath,  l_lda.map(l_data, l_labels), tools::files::hdf::NATIVE_DOUBLE );
    }

    // lda with string labels
    if (l_map["labeltype"].as<std::string>() == "string") {
        dim::lda<double, std::string> l_lda( l_dimension );

        const std::vector<std::string> l_labels = l_source.readStringVector(l_map["labelpath"].as<std::string>());
        l_target.writeBlasMatrix<double>( l_outpath,  l_lda.map(l_data, l_labels), tools::files::hdf::NATIVE_DOUBLE );
    }


    return EXIT_SUCCESS;
}
