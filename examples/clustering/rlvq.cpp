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

#if defined(_WIN32) || defined(__CYGWIN__)
#include <windows.h>
#endif


namespace po        = boost::program_options;
namespace ublas     = boost::numeric::ublas;
namespace cluster   = machinelearning::clustering::supervised;
namespace distance  = machinelearning::distances;
namespace tools     = machinelearning::tools;



/** main program
 * @param argc number of arguments
 * @param argv arguments
 **/
int main(int argc, char* argv[])
{
    #ifdef MACHINELEARNING_MULTILANGUAGE
    tools::language::bindings::bind();
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
        ("labelpath", po::value<std::string>(), "path to the data labels")
        ("labeltype", po::value<std::string>(), "datatype of labels (values: string, uint, int)")
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

    if ( (!l_map.count("outfile")) || (!l_map.count("inputfile")) || (!l_map.count("inputpath")) || (!l_map.count("labeltype")) ||  (!l_map.count("labelpath")))
    {
        std::cerr << "[--outfile], [--inputfile], [--inputpath], [--labelpath] and [--labeltype] option must be set" << std::endl;
        return EXIT_FAILURE;
    }







    // read source hdf file and data
    tools::files::hdf source( l_map["inputfile"].as<std::string>() );
    ublas::matrix<double> data = source.readBlasMatrix<double>( l_map["inputpath"].as<std::string>(), H5::PredType::NATIVE_DOUBLE);

    // create target file
    tools::files::hdf target( l_map["outfile"].as<std::string>(), true);
    target.writeValue<std::size_t>( "/iteration",  l_iteration, H5::PredType::NATIVE_ULONG );

    // create data
    distance::euclid<double> dist;



    // rlvq with uint labels
    if (l_map["labeltype"].as<std::string>() == "uint") {
        std::vector<std::size_t> labels = tools::vector::copy( source.readBlasVector<std::size_t>(l_map["labelpath"].as<std::string>(), H5::PredType::NATIVE_ULONG) );
        std::vector<std::size_t> unique = tools::vector::unique(labels);

        // create rlvq object
        cluster::rlvq<double, std::size_t> rlvq(dist, unique, data.size2());
        rlvq.setLogging( l_log );

        // train data
        rlvq.train( data, labels, l_iteration );


        // write target data
        target.writeValue<std::size_t>( "/numprotos",  unique.size(), H5::PredType::NATIVE_ULONG );
        target.writeBlasVector<std::size_t>( "/protolabel", tools::vector::copy(unique), H5::PredType::NATIVE_ULONG );
        target.writeBlasMatrix<double>( "/protos",  rlvq.getPrototypes(), H5::PredType::NATIVE_DOUBLE );

        // if logging exists write data to file
        if (rlvq.getLogging()) {
            target.writeBlasVector<double>( "/error",  tools::vector::copy(rlvq.getLoggedQuantizationError()), H5::PredType::NATIVE_DOUBLE );
            std::vector< ublas::matrix<double> > p = rlvq.getLoggedPrototypes();
            for(std::size_t i=0; i < p.size(); ++i)
                target.writeBlasMatrix<double>("/log" + boost::lexical_cast<std::string>( i ), p[i], H5::PredType::NATIVE_DOUBLE );
        }
    }


    // rlvq with int labels
    if (l_map["labeltype"].as<std::string>() == "int") {
        std::vector<std::ptrdiff_t> labels = tools::vector::copy( source.readBlasVector<std::ptrdiff_t>(l_map["labelpath"].as<std::string>(), H5::PredType::NATIVE_LONG) );
        std::vector<std::ptrdiff_t> unique = tools::vector::unique(labels);

        // create rlvq object
        cluster::rlvq<double, std::ptrdiff_t> rlvq(dist, unique, data.size2());
        rlvq.setLogging( l_log );

        // train data
        rlvq.train( data, labels, l_iteration );


        // write target data
        target.writeValue<std::size_t>( "/numprotos",  unique.size(), H5::PredType::NATIVE_ULONG );
        target.writeBlasVector<std::ptrdiff_t>( "/protolabel", tools::vector::copy(unique), H5::PredType::NATIVE_LONG );
        target.writeBlasMatrix<double>( "/protos",  rlvq.getPrototypes(), H5::PredType::NATIVE_DOUBLE );

        // if logging exists write data to file
        if (rlvq.getLogging()) {
            target.writeBlasVector<double>( "/error",  tools::vector::copy(rlvq.getLoggedQuantizationError()), H5::PredType::NATIVE_DOUBLE );
            std::vector< ublas::matrix<double> > p = rlvq.getLoggedPrototypes();
            for(std::size_t i=0; i < p.size(); ++i)
                target.writeBlasMatrix<double>("/log" + boost::lexical_cast<std::string>( i ), p[i], H5::PredType::NATIVE_DOUBLE );
        }
    }


    // rlvq with string labels
    if (l_map["labeltype"].as<std::string>() == "string") {
        std::vector<std::string> labels = source.readStringVector(l_map["labelpath"].as<std::string>());
        std::vector<std::string> unique = tools::vector::unique(labels);

        // create rlvq object
        cluster::rlvq<double, std::string> rlvq(dist, unique, data.size2());
        rlvq.setLogging( l_log );

        // train data
        rlvq.train( data, labels, l_iteration );


        // write target data
        target.writeValue<std::size_t>( "/numprotos",  unique.size(), H5::PredType::NATIVE_ULONG );
        target.writeStringVector( "/protolabel", unique );
        target.writeBlasMatrix<double>( "/protos",  rlvq.getPrototypes(), H5::PredType::NATIVE_DOUBLE );

        // if logging exists write data to file
        if (rlvq.getLogging()) {
            target.writeBlasVector<double>( "/error",  tools::vector::copy(rlvq.getLoggedQuantizationError()), H5::PredType::NATIVE_DOUBLE );
            std::vector< ublas::matrix<double> > p = rlvq.getLoggedPrototypes();
            for(std::size_t i=0; i < p.size(); ++i)
                target.writeBlasMatrix<double>("/log" + boost::lexical_cast<std::string>( i ), p[i], H5::PredType::NATIVE_DOUBLE );
        }
    }



    std::cout << "structure of the output file" << std::endl;
    std::cout << "/numprotos \t\t number of prototypes" << std::endl;
    std::cout << "/protolabel \t\t label of prototypes" << std::endl;
    std::cout << "/protos \t\t prototype matrix (row orientated)" << std::endl;
    std::cout << "/iteration \t\t number of iterations" << std::endl;

    if (l_log) {
        std::cout << "/error \t\t quantization error on each iteration" << std::endl;
        std::cout << "/log<0 to number of iteration-1>/protosos \t\t prototypes on each iteration" << std::endl;
    }

    return EXIT_SUCCESS;
}
