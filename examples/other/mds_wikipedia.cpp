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

#ifdef MACHINELEARNING_MPI
#include <boost/mpi.hpp>
#endif

#if defined(_WIN32) || defined(__CYGWIN__)
#include <windows.h>
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
 * @param argc number of arguments
 * @param argv arguments
**/
int main(int argc, char* argv[])
{

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
    po::store(po::command_line_parser(argc, argv).options(l_description).positional(l_input).run(), l_map);
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
    if (l_artnum.size() != static_cast<std::size_t>(loMPICom.size()))
        throw std::runtime_error("number of articles and used CPUs are not equal");
    if (l_artnum[loMPICom.rank()] == 0)
    #else
    const std::size_t l_artnum = l_map["articles"].as<std::size_t>();
    if (l_artnum < 2)
    #endif
        throw std::runtime_error("number of articles must be greater or equal than one");



    // get n random wikipedia articles
    #ifdef MACHINELEARNING_MPI
    loMPICom.barrier();
    std::cout << "CPU " << loMPICom.rank() << ": ";
    #endif
    std::cout << "read wikipedia articles..." << std::endl;

    std::vector<std::string> l_wikidata;
    std::vector<std::string> l_wikilabel;

    tools::language::code l_lang = tools::language::EN;
    if (l_map.count("lang"))
        l_lang = tools::language::fromString(l_map["lang"].as<std::string>());
    tools::sources::wikipedia wiki( l_lang );

    #ifdef MACHINELEARNING_MPI
    for(std::size_t i=0; i < l_artnum[loMPICom.rank()]; )
    #else
    for(std::size_t i=0; i < l_artnum; )
    #endif
        try {
            wiki.getRandomArticle();
            if (wiki.isArticle()) {
                #ifdef MACHINELEARNING_MPI
                std::cout << "CPU " << loMPICom.rank() << ": ";
                #endif
                std::cout << (i+1) << "\t" << wiki.getArticleTitle() << std::endl;

                l_wikidata.push_back( wiki.getArticleContent() );
                l_wikilabel.push_back( wiki.getArticleTitle() );
                i++;
            }
        } catch (...) {}


    #ifdef MACHINELEARNING_MPI
    // each process must get the article data
    std::vector< std::vector<std::string> > l_processarticles;
    std::vector< std::vector<std::string> > l_processlabel;
    mpi::all_gather(loMPICom, l_wikidata, l_processarticles);
    mpi::all_gather(loMPICom, l_wikilabel, l_processlabel);

    std::vector<std::string> l_allarticles;
    std::vector<std::string> l_alllabel;
    std::size_t startcol = 0;
    for(std::size_t i=0; i < l_processarticles.size(); ++i) {
        if (i < static_cast<std::size_t>(loMPICom.rank()))
            startcol += l_processarticles[i].size();

        for(std::size_t n=0; n < l_processarticles[i].size(); ++n) {
            l_allarticles.push_back( l_processarticles[i][n] );
            l_alllabel.push_back( l_processlabel[i][n] );
        }
    }
    #endif


    // do stopword reduction
    if (l_map.count("stopword")) {
        #ifdef MACHINELEARNING_MPI
        loMPICom.barrier();
        std::cout << "CPU " << loMPICom.rank() << ": ";
        #endif
        std::cout << "stopword reduction..." << std::endl;

        const std::vector<double> l_val = l_map["stopword"].as< std::vector<double> >();
        if (l_val.size() >= 2) {
            text::termfrequency tfc;

            #ifdef MACHINELEARNING_MPI
            tfc.add(l_allarticles);
            text::stopwordreduction stopword( tfc.getTerms(l_val[0], l_val[1] ), tfc.iscaseinsensitivity() );
            for(std::size_t i=0; i < l_allarticles.size(); ++i)
                l_allarticles[i] = stopword.remove( l_allarticles[i] );

            #else
            tfc.add(l_wikidata);
            text::stopwordreduction stopword( tfc.getTerms(l_val[0], l_val[1] ), tfc.iscaseinsensitivity() );
            #endif
            for(std::size_t i=0; i < l_wikidata.size(); ++i)
                l_wikidata[i] = stopword.remove( l_wikidata[i] );
        }
    }



    // create ncd object and calculate the distances
    #ifdef MACHINELEARNING_MPI
    loMPICom.barrier();
    std::cout << "CPU " << loMPICom.rank() << ": ";
    #endif
    std::cout << "calculate normalized compression distance..." << std::endl;

    distances::ncd<double> ncd( (l_algorithm == "gzip") ? distances::ncd<double>::gzip : distances::ncd<double>::bzip2 );

    if (l_compress == "bestspeed")
        ncd.setCompressionLevel( distances::ncd<double>::bestspeed );
    if (l_compress == "bestcompression")
        ncd.setCompressionLevel( distances::ncd<double>::bestcompression );


    #ifdef MACHINELEARNING_MPI
    ublas::matrix<double> distancematrix = ncd.unsquare( l_allarticles, l_wikidata );

    for(std::size_t j=0; j < distancematrix.size2(); ++j)
        distancematrix(j+startcol, j) = 0;

    l_allarticles.clear();
    #else
    ublas::matrix<double> distancematrix = ncd.unsymmetric( l_wikidata );
    #endif
    l_wikidata.clear();




    // run hit mds over the distance matrix
    #ifdef MACHINELEARNING_MPI
    loMPICom.barrier();
    std::cout << "CPU " << loMPICom.rank() << ": ";
    #endif
    std::cout << "run mds projection..." << std::endl;

    dim::mds<double>::project l_project = dim::mds<double>::hit;
    if (l_mapping == "metric")
        l_project = dim::mds<double>::metric;
    if (l_mapping == "sammon")
        l_project = dim::mds<double>::sammon;

    dim::mds<double> mds( l_dimension, l_project );
    mds.setIteration( l_iteration );
    mds.setRate( l_rate );


    #ifdef MACHINELEARNING_MPI
    ublas::matrix<double> project = mds.map( loMPICom, distancematrix );

    std::vector< ublas::matrix<double> > l_allproject;
    mpi::all_gather(loMPICom, project, l_allproject);

    project = l_allproject[0];
    for(std::size_t i=1; i < l_allproject.size(); ++i) {
        project.resize( project.size1()+l_allproject[i].size1(), project.size2() );
        ublas::matrix_range< ublas::matrix<double> > l_range( project, ublas::range(project.size1()-l_allproject[i].size1(), project.size1()), ublas::range(0, project.size2()) );
        l_range.assign( l_allproject[i] );
    }
    #else
    ublas::matrix<double> project = mds.map( distancematrix );
    #endif




    // create file and write data to hdf
    #ifdef MACHINELEARNING_MPI
    if (loMPICom.rank() == 0) {
    #endif
    tools::files::hdf target( l_map["outfile"].as<std::string>(), true);
    target.writeBlasMatrix<double>( "/project",  project, H5::PredType::NATIVE_DOUBLE );
    #ifndef MACHINELEARNING_MPI
    target.writeStringVector( "/label",  l_wikilabel );
    #else
    target.writeStringVector( "/label",  l_alllabel );
    #endif


    std::cout << "within the target file there are three datasets: /project = projected data, /label = datapoint label" << std::endl;
    #ifdef MACHINELEARNING_MPI
    }
    #endif


    return EXIT_SUCCESS;
}
