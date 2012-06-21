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


namespace po         = boost::program_options;
namespace ublas      = boost::numeric::ublas;
namespace classifier = machinelearning::classifier;
namespace distance   = machinelearning::distances;
namespace neighbor   = machinelearning::neighborhood;
namespace tools      = machinelearning::tools;


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
    std::size_t l_knn;
    std::string l_distance;

    // create CML options with description
    po::options_description l_description("allowed options");
    l_description.add_options()
        ("help", "produce help message")
        ("outfile", po::value<std::string>(), "output HDF5 file")
        ("inputfile", po::value<std::string>(), "input HDF5 file")
        ("datapath", po::value<std::string>(), "path to dataset")
        ("datalabel", po::value<std::string>(), "path to the data labels")
        ("unkowndata", po::value<std::string>(), "path to unlabeld data")
        ("labeltype", po::value<std::string>(), "datatype of labels (values: string, uint, int)")
        ("neighbour", po::value<std::size_t>(&l_knn)->default_value(3), "number of neighbours [default: 3]")
        ("distance", po::value<std::string>(&l_distance)->default_value("invert"), "distance (values: invert [default], distance)")
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

    if ( (!l_map.count("outfile")) || (!l_map.count("inputfile")) || (!l_map.count("datapath")) || (!l_map.count("datalabel")) || (!l_map.count("unkowndata")) || (!l_map.count("labeltype")) )
    {
        std::cerr << "[--outfile], [--inputfile], [--datapath], [--datalabel], [--unkwondata] and [--labeltype] option must be set" << std::endl;
        return EXIT_FAILURE;
    }




    // read source hdf file and data
    tools::files::hdf source( l_map["inputfile"].as<std::string>() );
    const ublas::matrix<double> data        = source.readBlasMatrix<double>( l_map["datapath"].as<std::string>(), H5::PredType::NATIVE_DOUBLE);
    const ublas::matrix<double> unknowndata = source.readBlasMatrix<double>( l_map["unkowndata"].as<std::string>(), H5::PredType::NATIVE_DOUBLE);

    // create target file
    tools::files::hdf target(l_map["outfile"].as<std::string>(), true);



    // distance / neighbourhood
    distance::euclid<double> d;
    neighbor::knn<double> n(d, l_knn);

    if (l_map["labeltype"].as<std::string>() == "int") {
        classifier::lazylearner<double, std::ptrdiff_t>::weighttype dist = classifier::lazylearner<double, std::ptrdiff_t>::inversedistance;
        if (l_distance == "distance")
            dist = classifier::lazylearner<double, std::ptrdiff_t>::distance;

        // create lazy-learner
        classifier::lazylearner<double, std::ptrdiff_t> lazy(n, dist);

        // set data and read new labels for unkown data
        lazy.setLogging(l_log);
        lazy.setDatabase( data, tools::vector::copy( source.readBlasVector<std::ptrdiff_t>(l_map["datalabel"].as<std::string>(), H5::PredType::NATIVE_INT) ) );
        target.writeBlasVector<std::ptrdiff_t>( "/unkwonlabel", tools::vector::copy(lazy.use(unknowndata)), H5::PredType::NATIVE_INT );

        // if logging exists write data to file
        if (lazy.getLogging())
            target.writeBlasVector<double>( "/error",  tools::vector::copy(lazy.getLoggedQuantizationError()), H5::PredType::NATIVE_DOUBLE );
    }


    if (l_map["labeltype"].as<std::string>() == "uint") {
        classifier::lazylearner<double, std::size_t>::weighttype dist = classifier::lazylearner<double, std::size_t>::inversedistance;
        if (l_distance == "distance")
            dist = classifier::lazylearner<double, std::size_t>::distance;

        // create lazy-learner
        classifier::lazylearner<double, std::size_t> lazy(n, dist);

        // set data and read new labels for unkown data
        lazy.setLogging(l_log);
        lazy.setDatabase( data, tools::vector::copy( source.readBlasVector<std::size_t>(l_map["datalabel"].as<std::string>(), H5::PredType::NATIVE_UINT) ) );
        target.writeBlasVector<std::size_t>( "/unkwonlabel", tools::vector::copy(lazy.use(unknowndata)), H5::PredType::NATIVE_UINT );

        // if logging exists write data to file
        if (lazy.getLogging())
            target.writeBlasVector<double>( "/error",  tools::vector::copy(lazy.getLoggedQuantizationError()), H5::PredType::NATIVE_DOUBLE );
    }


    if (l_map["labeltype"].as<std::string>() == "string") {
        classifier::lazylearner<double, std::string>::weighttype dist = classifier::lazylearner<double, std::string>::inversedistance;
        if (l_distance == "distance")
            dist = classifier::lazylearner<double, std::string>::distance;

        // create lazy-learner
        classifier::lazylearner<double, std::string> lazy(n, dist);

        // set data and read new labels for unkown data
        lazy.setLogging(l_log);
        lazy.setDatabase( data, source.readStringVector(l_map["datalabel"].as<std::string>()) );
        target.writeStringVector( "/unkwonlabel", lazy.use(unknowndata) );

        // if logging exists write data to file
        if (lazy.getLogging())
            target.writeBlasVector<double>( "/error",  tools::vector::copy(lazy.getLoggedQuantizationError()), H5::PredType::NATIVE_DOUBLE );
    }


    std::cout << "create HDF file with dataset \"/unkwonlabel\" for unkwon data, and if logging is enabled \"/error\" with quantization error" << std::endl;
    return EXIT_SUCCESS;
}
