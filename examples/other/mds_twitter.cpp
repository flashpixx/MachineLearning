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

#ifdef MACHINELEARNING_MPI
#include <boost/mpi.hpp>
#endif



namespace po        = boost::program_options;
namespace ublas     = boost::numeric::ublas;
namespace dim       = machinelearning::dimensionreduce::nonsupervised;
namespace tools     = machinelearning::tools;
namespace distances = machinelearning::distances;
namespace text      = machinelearning::textprocess;
#ifdef MACHINELEARNING_MPI
namespace mpi       = boost::mpi;
#endif



/** main program, that reads twitter tweets, calculate the distance between tweet
 * and create the plot via MDS.
 * @param p_argc number of arguments
 * @param p_argv arguments
 **/
int main(int p_argc, char* p_argv[])
{
	#ifdef MACHINELEARNING_MPI
    mpi::environment l_mpienv(p_argc, p_argv);
    mpi::communicator l_mpicom;
    #endif
	
    #ifdef MACHINELEARNING_MULTILANGUAGE
    tools::language::bindings::bind();
    #endif

    // default values
    std::size_t l_dimension;
    std::size_t l_iteration;
    double l_rate;
    std::string l_compress;
    std::string l_algorithm;
    std::string l_mapping;

    // create CML options with description
    po::options_description l_description("allowed options");
    l_description.add_options()
        ("help", "produce help message")
        ("search", po::value< std::vector<std::string> >()->multitoken(), "search keyword / keyword list")
        ("outfile", po::value<std::string>(), "output HDF5 file")
        ("lang", po::value<std::string>(), "language code (iso 639-1 or -3)")
        ("geo", po::value< std::vector<std::string> >()->multitoken(), "geographic position (format: latitude longitude radius radiuslength [km = kilometer, mi = miles])")
        ("until", po::value< std::vector<std::string> >()->multitoken(), "date value (format: YYYY MM DD)")
        ("dimension", po::value<std::size_t>(&l_dimension)->default_value(3), "number of project dimensions (default 3)")
        ("max", po::value< std::vector<std::size_t> >()->multitoken(), "number of tweets of each keyword (0 = maximum)")
        ("rate", po::value<double>(&l_rate)->default_value(1), "iteration rate for sammon / hit (default 1)")
        ("compress", po::value<std::string>(&l_compress)->default_value("default"), "compression level (allowed values are: default [default], bestspeed or bestcompression)")
        ("algorithm", po::value<std::string>(&l_algorithm)->default_value("gzip"), "compression algorithm (allowed values are: gzip [default], bzip)")
        ("iteration", po::value<std::size_t>(&l_iteration)->default_value(0), "number of iterations (detected automatically)")
        ("mapping", po::value<std::string>(&l_mapping)->default_value("hit"), "mapping type (values: metric, sammon, hit [default])")
    ;

    po::variables_map l_map;
    po::positional_options_description l_input;
    po::store(po::command_line_parser(p_argc, p_argv).options(l_description).positional(l_input).run(), l_map);
    po::notify(l_map);

    if (l_map.count("help")) {
        std::cout << l_description << std::endl;
        return EXIT_SUCCESS;
    }

    if ( (!l_map.count("outfile")) || (!l_map.count("search")) )  {
        std::cerr << "[--outfile] and [--search] option must be set" << std::endl;
        return EXIT_FAILURE;
    }




    // create basis data and twitter objects
    const std::vector<std::string> l_tweet = l_map["search"].as< std::vector<std::string> >();
    #ifdef MACHINELEARNING_MPI
    if (l_tweet.size() != static_cast<std::size_t>(l_mpicom.size()))
        throw std::runtime_error("number of tweet searches and used CPUs are not equal");
    #endif
    
    std::vector<std::size_t> l_tweetmax;
    if (l_map.count("max"))
        l_tweetmax    = l_map["max"].as< std::vector<std::size_t> >();
    for(std::size_t i=l_tweetmax.size(); i < l_tweet.size(); ++i)
        l_tweetmax.push_back(0);




    tools::sources::twitter l_twitter;
    tools::sources::twitter::searchparameter l_params;

    if (l_map.count("lang"))
        try {
            l_params.setLanguage( tools::language::fromString(l_map["lang"].as<std::string>()) );
        } catch (...) {}

    if (l_map.count("geo")) {
        const std::vector<std::string> l_geoparam = l_map["geo"].as< std::vector<std::string> >();

        if (l_geoparam.size() >= 4)
            try {
                tools::sources::twitter::searchparameter::geoposition l_geo;

                l_geo.latitude      = boost::lexical_cast<double>(l_geoparam[0]);
                l_geo.longitude     = boost::lexical_cast<double>(l_geoparam[1]);
                l_geo.radius        = boost::lexical_cast<double>(l_geoparam[2]);
                if (l_geoparam[3] == "mi")
                    l_geo.length  = tools::sources::twitter::searchparameter::miles;

                l_params.setGeoPosition( l_geo );
            } catch (...) {}
    }

    if (l_map.count("until")) {
        const std::vector<std::string> l_dateparam = l_map["until"].as< std::vector<std::string> >();

        if (l_dateparam.size() >= 3)
            try {
                boost::gregorian::date l_until(
                                               boost::lexical_cast<std::size_t>(l_dateparam[0]),
                                               boost::lexical_cast<std::size_t>(l_dateparam[1]),
                                               boost::lexical_cast<std::size_t>(l_dateparam[2])
                                               );
                l_params.setUntilDate( l_until );
            } catch (...) {}
    }




    // get tweets
    #ifdef MACHINELEARNING_MPI
    l_mpicom.barrier();
    std::cout << "CPU " << l_mpicom.rank() << ": ";
    #endif
    std::cout << "read tweets..." << std::endl;
    std::vector<std::string> l_tweetdata;
    std::vector<std::string> l_tweetlabel;

    #ifdef MACHINELEARNING_MPI
    std::vector<tools::sources::twitter::searchtweet> l_data = l_twitter.search( l_tweet[static_cast<std::size_t>(l_mpicom.rank())], l_params, l_tweetmax[static_cast<std::size_t>(l_mpicom.rank())] );

    for(std::size_t i=0; i < l_data.size(); ++i) {
        l_tweetdata.push_back( l_data[i].getText() );
        l_tweetlabel.push_back( l_tweet[static_cast<std::size_t>(l_mpicom.rank())] );
    }

    #else

    for(std::size_t n=0; n < l_tweet.size(); ++n) {
        std::vector<tools::sources::twitter::searchtweet> l_data = l_twitter.search( l_tweet[n], l_params, l_tweetmax[n] );

        for(std::size_t i=0; i < l_data.size(); ++i) {
            l_tweetdata.push_back( l_data[i].getText() );
            l_tweetlabel.push_back( l_tweet[n] );
        }
    }

    #endif




    // create ncd object and calculate the distances
    #ifdef MACHINELEARNING_MPI
    l_mpicom.barrier();
    std::cout << "CPU " << l_mpicom.rank() << ": ";
    #endif
    std::cout << "calculate normalized compression distance..." << std::endl;

    distances::ncd<double> l_ncd( (l_algorithm == "gzip") ? distances::ncd<double>::gzip : distances::ncd<double>::bzip2 );

    l_ncd.setCompressionLevel( distances::ncd<double>::defaultcompression );
    if (l_compress == "bestspeed")
        l_ncd.setCompressionLevel( distances::ncd<double>::bestspeed );
    if (l_compress == "bestcompression")
        l_ncd.setCompressionLevel( distances::ncd<double>::bestcompression );

    #ifdef MACHINELEARNING_MPI
    ublas::matrix<double> l_distancematrix = l_ncd.unsquare( l_mpicom, l_tweetdata );
    #else
    ublas::matrix<double> l_distancematrix = l_ncd.unsymmetric( l_tweetdata );
    #endif
    l_tweetdata.clear();




    // run hit mds over the distance matrix
    #ifdef MACHINELEARNING_MPI
    l_mpicom.barrier();
    std::cout << "CPU " << l_mpicom.rank() << ": ";
    #endif
    std::cout << "run mds projection..." << std::endl;

    dim::mds<double>::project l_project = dim::mds<double>::hit;
    if (l_mapping == "metric")
        l_project = dim::mds<double>::metric;
    if (l_mapping == "sammon")
        l_project = dim::mds<double>::sammon;

    dim::mds<double> l_mds( l_dimension, l_project );
    if (l_iteration == 0)
        l_mds.setIteration( l_distancematrix.size2() );
    else
        l_mds.setIteration( l_iteration );
    l_mds.setRate( l_rate );

    #ifdef MACHINELEARNING_MPI
    ublas::matrix<double> l_projectdata = l_mds.map( l_mpicom, l_distancematrix );
    #else
    ublas::matrix<double> l_projectdata = l_mds.map( l_distancematrix );
    #endif



    
    
    // create file and write data to hdf
    #ifdef MACHINELEARNING_MPI
    l_mpicom.barrier();
    tools::files::hdf target( "node"+tools::function::toString(l_mpicom.rank())+"_"+l_map["outfile"].as<std::string>(), true);
    #else
    tools::files::hdf target( l_map["outfile"].as<std::string>(), true);
    #endif
    
    target.writeBlasMatrix<double>( "/project",  l_projectdata, tools::files::hdf::NATIVE_DOUBLE );
    target.writeStringVector( "/label",  l_tweetlabel );
    target.writeStringVector( "/uniquegroup",  l_tweet );
    
    
    #ifdef MACHINELEARNING_MPI
    if (l_mpicom.rank() == 0)
    #endif
    std::cout << "within the target file there are three datasets: /project = projected data, /label = datapoint label, /uniquegroup = list of unique labels" << std::endl;


    return EXIT_SUCCESS;
}
