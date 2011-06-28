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


#include <map>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include <machinelearning.h>
#include <boost/any.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/numeric/ublas/matrix.hpp>


namespace ublas     = boost::numeric::ublas;
namespace dim       = machinelearning::dimensionreduce::nonsupervised;
namespace tools     = machinelearning::tools;
namespace distances = machinelearning::distances;
namespace text      = machinelearning::textprocess;


/** read all input arguments 
 * @param argc number of arguments of "main"
 * @param argv arguments of "main"
 * @param p_args map with argument values (default values)
 * @return bool if all is correct
 **/
bool cliArguments( int argc, char* argv[], std::map<std::string, boost::any>& p_args ) {
   
    if (argc < 2) {
        std::cout << "--outfile \t\t output HDF5 file" << std::endl;
        std::cout << "--dimension \t\t number of project dimensions (default 3)" << std::endl;
        std::cout << "--rate \t\t\t iteration rate for sammon / hit (default 1)" << std::endl;
        std::cout << "--sources \t\t list of text files or directories with text files (all files in the directory will be read and subdirectories will be ignored)" << std::endl;
        std::cout << "--compress \t\t compression level (allowed values are: default, bestspeed or bestcompression)" << std::endl;
        std::cout << "--algorithm \t\t compression algorithm (allowed values are: gzip, bzip)" << std::endl;
        std::cout << "--mapping \t\t mapping type (values: metric, sammon, hit [default])" << std::endl;
        std::cout << "--iteration \t\t number of iterations (detected automatically)" << std::endl;
        std::cout << "--stopword \t\t ranges of stopword reduction (defaults: 0.33 & 0.66 [word out of the range will be removed] or 'off' for disable)" << std::endl;
        return false;
    }
    
    
    // set default arguments
    std::map<std::string, std::vector<std::string> > l_argmap;
    l_argmap["outfile"]   = std::vector<std::string>();
    l_argmap["dimension"] = std::vector<std::string>();
    l_argmap["iteration"] = std::vector<std::string>();
    l_argmap["rate"]      = std::vector<std::string>();
    l_argmap["compress"]  = std::vector<std::string>();
    l_argmap["algorithm"] = std::vector<std::string>();
    l_argmap["sources"]   = std::vector<std::string>(); 
    l_argmap["mapping"]   = std::vector<std::string>(); 
    l_argmap["stopword"]  = std::vector<std::string>();
    
    
    // read all arguments
    std::size_t n=1;
    for(int i=1; i < argc; i+=n) {
        n = 1;
        std::string lc(argv[i]);
        if (lc.length() < 2)
            continue;
        
        lc = lc.substr(2);
        boost::to_lower( lc );
        
        if (l_argmap.find(lc) != l_argmap.end()) {
            std::vector<std::string> lv;
            
            for(int l=i+1; l < argc; ++l) {
                std::string lc2(argv[l]);
                if ( (lc2.length() >= 2) && (lc2.substr(0,2) == "--") )
                    break;
                
                lv.push_back(lc2);
                n++;
            }
            
            l_argmap[lc] = lv;
        }
        
    }
    
    
    //check map values and convert them
    if ((l_argmap["outfile"].size() != 1) ||
        (l_argmap["sources"].size() == 0)
        )
        throw std::runtime_error("number of arguments are incorrect");
    
    
    p_args["outfile"]       = l_argmap["outfile"][0];
    p_args["rate"]          = double(1);
    p_args["dimension"]     = std::size_t(3);
    p_args["sources"]       = l_argmap["sources"];
    
    std::vector<float> l_stopwords;
    l_stopwords.push_back(0.33);
    l_stopwords.push_back(0.66);
    
    // check input arguments and convert them
    p_args["algorithm"] = distances::ncd<double>::bzip2;
    if (l_argmap["algorithm"].size() > 0) {
        boost::to_lower(l_argmap["algorithm"][0]);
        if (l_argmap["algorithm"][0] == "gzip")
            p_args["algorithm"]   = distances::ncd<double>::gzip;
        if ((l_argmap["algorithm"].size() > 0) && (l_argmap["algorithm"][0] == "bzip"))
            p_args["algorithm"]   = distances::ncd<double>::bzip2;
    }
    
    p_args["compress"] = distances::ncd<double>::defaultcompression;
    if (l_argmap["compress"].size() > 0) {
        boost::to_lower(l_argmap["compress"][0]);
        if (l_argmap["compress"][0] == "default")
            p_args["compress"]    = distances::ncd<double>::defaultcompression;
        if (l_argmap["compress"][0] == "bestspeed")
            p_args["compress"]    = distances::ncd<double>::bestspeed;
        if (l_argmap["compress"][0] == "bestcompression")
            p_args["compress"]    = distances::ncd<double>::bestcompression;
    }
    
    // check mapping argument
    p_args["mapping"] = dim::mds<double>::hit;
    if (l_argmap["mapping"].size() == 1) {
        boost::to_lower(l_argmap["mapping"][0]);
        if (l_argmap["mapping"][0] == "sammon")
            p_args["mapping"]   = dim::mds<double>::sammon;
        if (l_argmap["mapping"][0] == "metric")
            p_args["mapping"]   = dim::mds<double>::metric;
    }
    
    
    try {
        if (l_argmap["dimension"].size() > 0)
            p_args["dimension"]  = boost::lexical_cast<std::size_t>( l_argmap["dimension"][0] );
        
        if (l_argmap["rate"].size() > 0)
            p_args["rate"] = boost::lexical_cast<double>(l_argmap["rate"][0]);
                   
        if (l_argmap["stopword"].size() >= 2) {
            l_stopwords[0] = boost::lexical_cast<float>(l_argmap["stopword"][0]);
            l_stopwords[1] = boost::lexical_cast<float>(l_argmap["stopword"][1]);
        }
        
    } catch (...) {
        throw std::runtime_error("numerical data can not extracted");
    }  
    
    p_args["stopword"]      = l_stopwords;
    p_args["stopwordsoff"]  = bool(false);
    if (l_argmap["stopword"].size() >= 1) {
        std::string l_onoff = l_argmap["stopword"][0];
        boost::to_lower(l_onoff);
        p_args["stopwordsoff"] = l_onoff == "off";
    }
    
    // set iteration
    p_args["iteration"] = l_argmap["files"].size();
    if (l_argmap["iteration"].size() != 0)
        try {
            p_args["iteration"] = boost::lexical_cast<std::size_t>(l_argmap["iteration"][0]);
        } catch (...) {}
    
    
    return true;
}
    
    
    
/** main program, that reads the text files, calculate the distance between articles 
 * and create the plot via MDS.
 * @param argc number of arguments
 * @param argv arguments
 **/
int main(int argc, char* argv[]) {
    
    std::map<std::string, boost::any> l_args;
    if (!cliArguments(argc, argv, l_args))
        return EXIT_FAILURE;
        
    // read all file content into a vector
    std::cout << "read files..." << std::endl;
    std::vector<std::string> l_content;
    std::vector<std::string> l_files;
    const std::vector<std::string> l_sources = boost::any_cast< std::vector<std::string> >(l_args["sources"]);
    for(std::size_t i=0; i < l_sources.size(); ++i) {
        
        boost::filesystem::path data(l_sources[i]);
        if (!boost::filesystem::exists(data))
            throw std::runtime_error( "data [" + l_sources[i] + "] does not exist");
        
        // if data is a file
        if (boost::filesystem::is_regular_file(data)) {
            std::ifstream l_file(l_sources[i].c_str(), std::ifstream::in);
            if (l_file.bad())
                throw std::runtime_error( "file [" + l_files[i] + "] can not be read");
       
            std::stringbuf l_str;
            l_file >> &l_str;

            if (!l_str.str().empty()) {
                l_content.push_back( l_str.str() );
                l_files.push_back(l_sources[i]);
            }
        }
        
        // if data is a directory
        if (boost::filesystem::is_directory(data)) {
            std::vector<boost::filesystem::path> l_subdata;
            std::copy(boost::filesystem::directory_iterator(data), boost::filesystem::directory_iterator(), back_inserter(l_subdata));
            for(std::size_t j=0; j < l_subdata.size(); ++j) {
                if (!boost::filesystem::is_regular_file(l_subdata[j]))
                    continue;
                
                std::ifstream l_file(l_subdata[j].generic_string().c_str(), std::ifstream::in);
                if (l_file.bad())
                    throw std::runtime_error( "file [" + l_subdata[j].generic_string() + "] can not be read");
                
                std::stringbuf l_str;
                l_file >> &l_str;
                
                if (!l_str.str().empty()) {
                    l_content.push_back( l_str.str() );
                    l_files.push_back(l_subdata[j].generic_string());
                }
            }
        }
        
    }
    
    if (l_content.size() < 2)
        throw std::runtime_error( "at least two files are needed");

    // create file and write data to hdf
    tools::files::hdf target(boost::any_cast<std::string>(l_args["outfile"]), true);
    
    
    
    // run stop word reduction
    if (!boost::any_cast<bool>(l_args["stopwordsoff"])) {
        std::cout << "stopword reduction..." << std::endl;
        text::termfrequency tfc;
        tfc.add(l_content);
    
        const std::vector<std::string> l_stopwords = tfc.getTerms( boost::any_cast< std::vector<float> >(l_args["stopword"])[0], boost::any_cast< std::vector<float> >(l_args["stopword"])[1] );
        text::stopwordreduction stopword( l_stopwords, tfc.iscaseinsensitivity() );
        for(std::size_t i=0; i < l_content.size(); ++i)
            l_content[i] = stopword.remove( l_content[i] );
        
        target.writeStringVector("/stopwords", l_stopwords);
    }
    
            
    // run NCD
    std::cout << "calculate normalized compression distance..." << std::endl;
    distances::ncd<double> ncd( boost::any_cast< distances::ncd<double>::compresstype >(l_args["algorithm"]) );
    ncd.setCompressionLevel( boost::any_cast< distances::ncd<double>::compresslevel >(l_args["compress"]) );
        
    ublas::matrix<double> distancematrix = ncd.unsymmetric( l_content );
      
        
        
    // run hit mds over the distance matrix
    std::cout << "run mds projection..." << std::endl;
        
    dim::mds<double> mds( boost::any_cast<std::size_t>(l_args["dimension"]), boost::any_cast<dim::mds<double>::project>(l_args["mapping"]) );
    mds.setIteration( boost::any_cast<std::size_t>(l_args["iteration"]) );
    mds.setRate( boost::any_cast<double>(l_args["rate"]) );
        
    ublas::matrix<double> project = mds.map( distancematrix );
        
    
    // write data
    target.writeBlasMatrix<double>( "/project",  project, H5::PredType::NATIVE_DOUBLE );
    target.writeBlasMatrix<double>( "/distances",  distancematrix, H5::PredType::NATIVE_DOUBLE );
    target.writeStringVector("/files", l_files);
           
    std::cout << "within the target file there are four datasets: /project = projected data (first row = first input file ...), /files = filename list, /stopwords = list with stopwords (if enable), /distances = distance matrix" << std::endl;
    return EXIT_SUCCESS;
}
