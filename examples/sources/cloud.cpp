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
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/options_description.hpp>


using namespace machinelearning;
namespace ublas = boost::numeric::ublas;
namespace po    = boost::program_options;


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
    std::size_t l_sampling;
    std::size_t l_pointsmin;
    std::size_t l_pointsmax;
    double l_rangemin;
    double l_rangemax;
    double l_variancemin;
    double l_variancemax;
    double l_probability;
    bool l_shuffle;
    std::string l_create;


    // create CML options with description
    po::options_description l_description("allowed options");
    l_description.add_options()
        ("help", "produce help message")
        ("outfile", po::value<std::string>(), "output HDF5 file")
        ("outpath", po::value<std::string>(), "output path within the HDF5 file")
        ("dimension", po::value<std::size_t>(&l_dimension)->default_value(2), "number of dimension (default: 2)")
        ("sampling", po::value<std::size_t>(&l_sampling)->default_value(10), "sampling of the intervall (default: 10)")
        ("pointsmin", po::value<std::size_t>(&l_pointsmin)->default_value(50), "minimal number of points (default: 50)")
        ("pointsmax", po::value<std::size_t>(&l_pointsmax)->default_value(500), "maximal number of points (default: 500)")
        ("rangemin", po::value<double>(&l_rangemin)->default_value(0), "minimal range value (default: 0)")
        ("rangemax", po::value<double>(&l_rangemax)->default_value(100), "maximal range value (default: 100)")
        ("variancemin", po::value<double>(&l_variancemin)->default_value(0.1), "minimal variance value (default: 0.1)")
        ("variancemax", po::value<double>(&l_variancemax)->default_value(0.8), "maximal variance value (default: 0.8)")
        ("probability", po::value<double>(&l_probability)->default_value(0.5), "probability for the random create option (default: 0.5)")
        ("shuffel", po::value<bool>(&l_shuffle)->default_value(false), "shuffle the vectors of the clouds (default: false / 0)")
        ("create", po::value<std::string>(&l_create)->default_value("all"), "type for creating clouds (values are: all [default], alternate, random)")
    ;

    po::variables_map l_map;
    po::store(po::parse_command_line(p_argc, p_argv, l_description), l_map);
    po::notify(l_map);

    if (l_map.count("help")) {
        std::cout << l_description << std::endl;
        return EXIT_SUCCESS;
    }

    if ( (!l_map.count("outfile")) || (!l_map.count("outpath"))) {
        std::cerr << "[--outfile] and [--outpath] option must be set" << std::endl;
        return EXIT_FAILURE;
    }




    // create cloud object and set the data
    tools::sources::cloud<double> cloud( l_dimension );

    // set range of the points
    cloud.setPoints( l_pointsmin, l_pointsmax );

    // set variance, range and samples
    cloud.setVariance( l_variancemin, l_variancemax );
    for(std::size_t i=0; i < l_dimension; ++i)
        cloud.setRange(i, l_rangemin, l_rangemax, l_sampling);

    // create file and write data to hdf
    tools::sources::cloud<double>::cloudcreate l_typecreate = tools::sources::cloud<double>::all;
    if (l_create == "alternate")
        l_typecreate = tools::sources::cloud<double>::alternate;
    if (l_create == "random")
        l_typecreate = tools::sources::cloud<double>::random;


    tools::files::hdf target( l_map["outfile"].as<std::string>(), true);
    target.writeBlasMatrix<double>(
                l_map["outpath"].as<std::string>(),
                cloud.generate( l_typecreate, l_probability, l_shuffle ),
                tools::files::hdf::NATIVE_DOUBLE );

    return EXIT_SUCCESS;
}
