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



/** main program, that reads a subset of wikipedia articles, calculate the distance between articles
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
        #ifdef MACHINELEARNING_MPI
        ("articles", po::value< std::vector<std::size_t> >()->multitoken(), "number of articles")
        #else
        ("articles", po::value<std::size_t>(), "number of articles")
        #endif
        ("outfile", po::value<std::string>(), "output HDF5 file")
        ("lang", po::value<std::string>(), "language code (iso 639-1 or -3)")
        ("dimension", po::value<std::size_t>(&l_dimension)->default_value(3), "number of project dimensions (default 3)")
        ("rate", po::value<double>(&l_rate)->default_value(1), "iteration rate for sammon / hit (default 1)")
        ("compress", po::value<std::string>(&l_compress)->default_value("default"), "compression level (allowed values are: default [default], bestspeed or bestcompression)")
        ("algorithm", po::value<std::string>(&l_algorithm)->default_value("gzip"), "compression algorithm (allowed values are: gzip [default], bzip)")
        ("iteration", po::value<std::size_t>(&l_iteration)->default_value(0), "number of iterations (detected automatically)")
        ("mapping", po::value<std::string>(&l_mapping)->default_value("hit"), "mapping type (values: metric, sammon, hit [default])")
        ("stopword", po::value< std::vector<double> >()->multitoken(), "minimal and maximal value of the stopword reduction (value within the range [0,1])")
    ;

    po::variables_map l_map;
    po::positional_options_description l_input;
    po::store(po::command_line_parser(p_argc, p_argv).options(l_description).positional(l_input).run(), l_map);
    po::notify(l_map);

    if (l_map.count("help")) {
        std::cout << l_description << std::endl;
        return EXIT_SUCCESS;
    }

    if ( (!l_map.count("outfile")) || (!l_map.count("articles")) )  {
        std::cerr << "[--outfile] and [--articles] option must be set" << std::endl;
        return EXIT_FAILURE;
    }


    #ifdef MACHINELEARNING_MPI
    const std::vector<std::size_t> l_artnum = l_map["articles"].as< std::vector<std::size_t> >();
    if (l_artnum.size() != static_cast<std::size_t>(l_mpicom.size()))
        throw std::runtime_error("number of articles and used CPUs are not equal");
    if (l_artnum[l_mpicom.rank()] == 0)
    #else
    const std::size_t l_artnum = l_map["articles"].as<std::size_t>();
    if (l_artnum < 2)
    #endif
        throw std::runtime_error("number of articles must be greater or equal than one");



    // get n random wikipedia articles
    #ifdef MACHINELEARNING_MPI
    l_mpicom.barrier();
    std::cout << "CPU " << l_mpicom.rank() << ": ";
    #endif
    std::cout << "read wikipedia articles..." << std::endl;

    std::vector<std::string> l_wikidata;
    std::vector<std::string> l_wikilabel;

    tools::language::code l_lang = tools::language::EN;
    if (l_map.count("lang"))
        l_lang = tools::language::fromString(l_map["lang"].as<std::string>());
    tools::sources::wikipedia wiki( l_lang );

    #ifdef MACHINELEARNING_MPI
    for(std::size_t i=0; i < l_artnum[l_mpicom.rank()]; )
    #else
    for(std::size_t i=0; i < l_artnum; )
    #endif
        try {
            wiki.getRandomArticle();
            if (wiki.isArticle()) {
                #ifdef MACHINELEARNING_MPI
                std::cout << "CPU " << l_mpicom.rank() << ": ";
                #endif
                std::cout << (i+1) << "\t" << wiki.getArticleTitle() << std::endl;

                l_wikidata.push_back( wiki.getArticleContent() );
                l_wikilabel.push_back( wiki.getArticleTitle() );
                i++;
            }
        } catch (...) {}



    // do stopword reduction
    if (l_map.count("stopword")) {
        #ifdef MACHINELEARNING_MPI
        l_mpicom.barrier();
        std::cout << "CPU " << l_mpicom.rank() << ": ";
        #endif
        std::cout << "stopword reduction..." << std::endl;

        const std::vector<double> l_val = l_map["stopword"].as< std::vector<double> >();
        if (l_val.size() >= 2) {
            text::termfrequency l_tfc;
            l_tfc.add(l_wikidata);
            text::stopwordreduction l_stopword( l_tfc.getTerms(l_val[0], l_val[1] ), l_tfc.iscaseinsensitivity() );
            for(std::size_t i=0; i < l_wikidata.size(); ++i)
                l_wikidata[i] = l_stopword.remove( l_wikidata[i] );
        }
    }



    // create ncd object and calculate the distances
    #ifdef MACHINELEARNING_MPI
    l_mpicom.barrier();
    std::cout << "CPU " << l_mpicom.rank() << ": ";
    #endif
    std::cout << "calculate normalized compression distance..." << std::endl;

    distances::ncd<double> l_ncd( (l_algorithm == "gzip") ? distances::ncd<double>::gzip : distances::ncd<double>::bzip2 );

    if (l_compress == "bestspeed")
        l_ncd.setCompressionLevel( distances::ncd<double>::bestspeed );
    if (l_compress == "bestcompression")
        l_ncd.setCompressionLevel( distances::ncd<double>::bestcompression );


    #ifdef MACHINELEARNING_MPI
    ublas::matrix<double> l_distancematrix = l_ncd.unsquare( l_mpicom, l_wikidata );
    #else
    ublas::matrix<double> l_distancematrix = l_ncd.unsymmetric( l_wikidata );
    #endif
    l_wikidata.clear();



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
    tools::files::hdf l_target( "node"+tools::function::toString(l_mpicom.rank())+"_"+l_map["outfile"].as<std::string>(), true);
    #else
    tools::files::hdf l_target( l_map["outfile"].as<std::string>(), true);
    #endif
        
    l_target.writeBlasMatrix<double>( "/project",  l_projectdata, tools::files::hdf::NATIVE_DOUBLE );
    l_target.writeStringVector( "/label",  l_wikilabel );

    
    #ifdef MACHINELEARNING_MPI
    if (l_mpicom.rank() == 0)
    #endif
    std::cout << "within the target file there are three datasets: /project = projected data, /label = datapoint label" << std::endl;

    return EXIT_SUCCESS;
}
