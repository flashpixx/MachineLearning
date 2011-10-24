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

#include <cstdlib>
#include <machinelearning.h>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/options_description.hpp>

#if defined(_WIN32) || defined(__CYGWIN__)
#include <windows.h>
#endif


namespace po        = boost::program_options;
namespace ublas     = boost::numeric::ublas;
namespace cluster   = machinelearning::clustering::nonsupervised;
namespace tools     = machinelearning::tools;


/** main program
 * @param argc number of arguments
 * @param argv arguments
 **/
int main(int argc, char* argv[])
{
    #ifdef MACHINELEARNING_MULTILANGUAGE
    tools::language::bindings::bind("machinelearning", "language");
    #endif
    
    // default values
    bool l_log;
    std::size_t l_iteration;

    // create CML options with description
    po::options_description l_description("allowed options");
    l_description.add_options()
        ("help", "produce help message")
        ("outfile", po::value<std::string>(), "output HDF5 file")
        ("inputfile", po::value<std::string>(), "input HDF5 file")
        ("inputpath", po::value<std::string>(), "path to dataset")
        ("prototype", po::value<std::size_t>(), "number of prototypes")
        ("iteration", po::value<std::size_t>(&l_iteration)->default_value(15), "number of iteration [default: 15]")
        ("log", po::value<bool>(&l_log)->default_value(false), "'true' for enable logging [default: false]")
    ;

    po::variables_map l_map;
    po::positional_options_description l_input;
    po::store(po::command_line_parser(argc, argv).options(l_description).positional(l_input).run(), l_map);
    po::notify(l_map);

    if (l_map.count("help")) {
        std::cout << l_description << std::endl;
        return EXIT_SUCCESS;
    }

    if ( (!l_map.count("outfile")) || (!l_map.count("inputfile")) || (!l_map.count("inputpath")) || (!l_map.count("prototype")) )
    {
        std::cerr << "[--outfile], [--inputfile], [--inputpath] and [--prototype] option must be set" << std::endl;
        return EXIT_FAILURE;
    }







    // read source hdf file and data
    tools::files::hdf source( l_map["inputfile"].as<std::string>() );
    ublas::matrix<double> data = source.readBlasMatrix<double>(l_map["inputpath"].as<std::string>(), H5::PredType::NATIVE_DOUBLE);

    // create spectral clustering object, set number of prototypes, data size and logging
    cluster::spectralclustering<double> spectral(l_map["prototype"].as<std::size_t>(), data.size2());
    spectral.setLogging(l_log);

    // do clustering
    spectral.train(data, l_iteration);


    // create file and write data to hdf
    tools::files::hdf target(l_map["outfile"].as<std::string>(), true);

    target.writeBlasMatrix<double>( "/protos",  spectral.getPrototypes(), H5::PredType::NATIVE_DOUBLE );
    target.writeValue<std::size_t>( "/numprotos",  l_map["prototype"].as<std::size_t>(), H5::PredType::NATIVE_ULONG );
    target.writeValue<std::size_t>( "/iteration",  l_iteration, H5::PredType::NATIVE_ULONG );

    // if logging exists write data to file
    if (spectral.getLogging()) {
        target.writeBlasVector<double>( "/error",  tools::vector::copy(spectral.getLoggedQuantizationError()), H5::PredType::NATIVE_DOUBLE );
        std::vector< ublas::matrix<double> > p = spectral.getLoggedPrototypes();
        for(std::size_t i=0; i < p.size(); ++i)
            target.writeBlasMatrix<double>("/log" + boost::lexical_cast<std::string>( i )+"/protos", p[i], H5::PredType::NATIVE_DOUBLE );
    }


    std::cout << "structure of the output file" << std::endl;
    std::cout << "/numprotos \t\t number of prototypes" << std::endl;
    std::cout << "/protos \t\t prototype matrix (row orientated)" << std::endl;
    std::cout << "/iteration \t\t number of iterations" << std::endl;

    if (spectral.getLogging()) {
        std::cout << "/error \t\t quantization error on each iteration" << std::endl;
        std::cout << "/log<0 to number of iteration-1>/protosos \t\t prototypes on each iteration" << std::endl;
    }



    return EXIT_SUCCESS;
}
