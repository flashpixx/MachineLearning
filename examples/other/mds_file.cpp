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
#include <fstream>
#include <machinelearning.h>
#include <boost/filesystem.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/options_description.hpp>



namespace po        = boost::program_options;
namespace ublas     = boost::numeric::ublas;
namespace dim       = machinelearning::dimensionreduce::nonsupervised;
namespace tools     = machinelearning::tools;
namespace distances = machinelearning::distances;
namespace text      = machinelearning::textprocess;



/** main program, that reads the text files, calculate the distance between articles
 * and create the plot via MDS.
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
    std::size_t l_iteration;
    double l_rate;
    std::string l_compress;
    std::string l_algorithm;
    std::string l_mapping;

    // create CML options with description
    po::options_description l_description("allowed options");
    l_description.add_options()
        ("help", "produce help message")
        ("outfile", po::value<std::string>(), "output HDF5 file")
        ("sources", po::value< std::vector<std::string> >()->multitoken(), "list of text files or directories with text files (all files in the directory will be read and subdirectories will be ignored)")
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

    if ( (!l_map.count("outfile")) || (!l_map.count("sources")) )  {
        std::cerr << "[--outfile] and [--sources] option must be set" << std::endl;
        return EXIT_FAILURE;
    }







    // read all file content into a vector
    std::cout << "read files..." << std::endl;

    // first read all files
    std::vector<std::string> l_files;
    const std::vector<std::string> l_sources = l_map["sources"].as< std::vector<std::string> >();

    for(std::size_t i=0; i < l_sources.size(); ++i) {

        boost::filesystem::path l_data(l_sources[i]);
        if (!boost::filesystem::exists(l_data))
            throw std::runtime_error( "data [" + l_sources[i] + "] does not exist");

        // if data is a file
        if (boost::filesystem::is_regular_file(l_data))
            l_files.push_back(l_sources[i]);

        // if data is a directory
        if (boost::filesystem::is_directory(l_data)) {
            std::vector<boost::filesystem::path> l_subdata;
            std::copy(boost::filesystem::directory_iterator(l_data), boost::filesystem::directory_iterator(), back_inserter(l_subdata));
            for(std::size_t j=0; j < l_subdata.size(); ++j)
                if (boost::filesystem::is_regular_file(l_subdata[j]))
                    l_files.push_back(l_subdata[j].generic_string());
        }
    }

    if (l_files.size() < 2)
        throw std::runtime_error("at least two files are needed");

    // create file and write data to hdf
    tools::files::hdf target(l_map["outfile"].as<std::string>(), true);



    // if stopword reduction enabled, we read the file content
    std::vector<std::string> l_content;
    if (l_map.count("stopword")) {
        const std::vector<double> l_val = l_map["stopword"].as< std::vector<double> >();
        if (l_val.size() >= 2) {

            for(std::size_t i=0; i < l_files.size(); ++i) {
                std::ifstream l_file(l_files[i].c_str(), std::ifstream::in);
                if (l_file.bad())
                    throw std::runtime_error( "file [" + l_files[i] + "] can not be read");

                std::stringbuf l_str;
                l_file >> &l_str;

                l_content.push_back( l_str.str() );
            }

            std::cout << "stopword reduction..." << std::endl;
            text::termfrequency l_tfc;
            l_tfc.add(l_content);

            const std::vector<std::string> l_stopwords = l_tfc.getTerms( l_val[0], l_val[1] );
            text::stopwordreduction l_stopword( l_stopwords, l_tfc.iscaseinsensitivity() );
            for(std::size_t i=0; i < l_content.size(); ++i)
                l_content[i] = l_stopword.remove( l_content[i] );

            target.writeStringVector("/stopwords", l_stopwords);
        }
    }


    // run NCD
    
    std::cout << "calculate normalized compression distance..." << std::endl;
    distances::ncd<double> l_ncd( (l_algorithm == "gzip") ? distances::ncd<double>::gzip : distances::ncd<double>::bzip2 );

    if (l_compress == "bestspeed")
        l_ncd.setCompressionLevel( distances::ncd<double>::bestspeed );
    if (l_compress == "bestcompression")
        l_ncd.setCompressionLevel( distances::ncd<double>::bestcompression );


    ublas::matrix<double> l_distancematrix;
    if ( (l_map.count("stopwordmin")) && (l_map.count("stopwordmax")) )
        l_distancematrix = l_ncd.unsymmetric( l_content );
    else
        l_distancematrix = l_ncd.unsymmetric( l_files, true );
    


    // run hit mds over the distance matrix
    std::cout << "run mds projection..." << std::endl;


    dim::mds<double>::project l_project = dim::mds<double>::hit;
    if (l_mapping == "metric")
        l_project = dim::mds<double>::metric;
    if (l_mapping == "sammon")
        l_project = dim::mds<double>::sammon;
    
    dim::mds<double> l_mds( l_dimension, l_project );
    if (l_iteration == 0)
        l_mds.setIteration( l_distancematrix.size1() );
    else
        l_mds.setIteration( l_iteration );
    l_mds.setRate( l_rate );

    ublas::matrix<double> l_projectdata = l_mds.map( l_distancematrix );
    

    // write data
    target.writeBlasMatrix<double>( "/project",  l_projectdata, tools::files::hdf::NATIVE_DOUBLE );
    target.writeStringVector("/files", l_files);

    std::cout << "within the target file there are four datasets: /project = projected data (first row = first input file ...), /files = filename list, /stopwords = list with stopwords (if enable)" << std::endl;
    
    return EXIT_SUCCESS;
}
