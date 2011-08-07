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
#include <algorithm>

#include <machinelearning.h>
#include <boost/any.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/numeric/ublas/matrix.hpp>

#ifdef MACHINELEARNING_MPI
#include <boost/mpi.hpp>
#endif

namespace ublas     = boost::numeric::ublas;
namespace dim       = machinelearning::dimensionreduce::nonsupervised;
namespace tools     = machinelearning::tools;
namespace distances = machinelearning::distances;
namespace text      = machinelearning::textprocess;
#ifdef MACHINELEARNING_MPI
namespace mpi       = boost::mpi;
#endif


/** read all input arguments 
 * @param argc number of arguments of "main"
 * @param argv arguments of "main"
 * @param p_args map with argument values (default values)
 * @return bool if all is correct
 **/
#ifdef MACHINELEARNING_MPI
bool cliArguments( int argc, char* argv[], std::map<std::string, boost::any>& p_args, mpi::communicator& p_mpi ) {
#else
bool cliArguments( int argc, char* argv[], std::map<std::string, boost::any>& p_args ) {
#endif
        
        if (argc < 2) {
            #ifdef MACHINELEARNING_MPI
            if (p_mpi.rank() == 0) {
            #endif
                std::cout << "--tweet \t\t keyword(s) for search" << std::endl;
                //std::cout << "--lang \t\t language" << std::endl;
                //std::cout << "--geo \t\t geographic position (latitude, longitude, radius, radiuslength [km = kilometer, mi = miles])" << std::endl;
                std::cout << "--outfile \t\t output HDF5 file" << std::endl;
                std::cout << "--dimension \t\t number of project dimensions (default 3)" << std::endl;
                std::cout << "--rate \t\t\t iteration rate for sammon / hit (default 1)" << std::endl;
                std::cout << "--max \t\t\t number of tweets of each keyword (0 = maximum)" << std::endl;
                std::cout << "--compress \t\t compression level (allowed values are: default, bestspeed or bestcompression)" << std::endl;
                std::cout << "--algorithm \t\t compression algorithm (allowed values are: gzip, bzip)" << std::endl;
                std::cout << "--mapping \t\t mapping type (values: metric, sammon, hit [default])" << std::endl;
                std::cout << "--iteration \t\t number of iterations (detected automatically)" << std::endl;
            #ifdef MACHINELEARNING_MPI
            }
            #endif
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
        l_argmap["tweet"]     = std::vector<std::string>();
        l_argmap["max"]       = std::vector<std::string>();
        l_argmap["mapping"]   = std::vector<std::string>(); 
        //l_argmap["lang"]      = std::vector<std::string>();
        //l_argmap["geo"]       = std::vector<std::string>();
        
        
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
            (l_argmap["tweet"].size() == 0)
            )
            throw std::runtime_error("number of arguments are incorrect");
        
        p_args["tweet"]         = l_argmap["tweet"];
        p_args["outfile"]       = l_argmap["outfile"][0];
        p_args["rate"]          = double(1);
        p_args["dimension"]     = std::size_t(3);
        
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
            
        } catch (...) {
            throw std::runtime_error("numerical data can not extracted");
        }  
    
    
        // set number of tweets
        std::vector<std::size_t> l_max;
        try {
            
            for(std::size_t i=0; i < l_argmap["max"].size(); ++i)
                l_max.push_back( boost::lexical_cast<std::size_t>(l_argmap["max"][i]) ); 
        } catch (...) {}
    
        for(std::size_t i=l_argmap["max"].size(); i < l_argmap["tweet"].size(); ++i)
            l_max.push_back(0);
        p_args["max"] = l_max;
        
        // set iteration
        if (l_argmap["iteration"].size() == 0) {
            std::size_t l_it = std::accumulate( l_max.begin(), l_max.end(), 0 );
            if (l_it == 0)
                l_it = l_argmap["tweet"].size();
            p_args["iteration"] = l_it;
        } else
            p_args["iteration"] = boost::lexical_cast<std::size_t>(l_argmap["iteration"][0]);
        
        return true;
    }
    
    
    
    /** main program, that reads a subset of wikipedia articles, calculate the distance between articles 
     * and create the plot via MDS.
     * @param argc number of arguments
     * @param argv arguments
     **/
    int main(int argc, char* argv[]) {
        
        std::map<std::string, boost::any> l_args;
        
        #ifdef MACHINELEARNING_MPI
        mpi::environment loMPIenv(argc, argv);
        mpi::communicator loMPICom;
        if (!cliArguments(argc, argv, l_args, loMPICom))    
        #else
        if (!cliArguments(argc, argv, l_args))
        #endif
            return EXIT_FAILURE;
        
        
        const std::vector<std::string> l_tweet    = boost::any_cast< std::vector<std::string> >(l_args["tweet"]);
        const std::vector<std::size_t> l_tweetmax = boost::any_cast< std::vector<std::size_t> >(l_args["max"]);
        #ifdef MACHINELEARNING_MPI
        if (l_tweet.size() != static_cast<std::size_t>(loMPICom.size()))
            throw std::runtime_error("number of tweet searches and used CPUs are not equal");
        #endif
       
        
        
        
        // get tweets
        #ifdef MACHINELEARNING_MPI 
        std::cout << "CPU " << loMPICom.rank() << ": ";
        #endif
        std::cout << "read tweets..." << std::endl;
        
        
        tools::sources::twitter l_twitter;
        #ifdef MACHINELEARNING_MPI 
        std::vector<tools::sources::twitter::searchtweet> l_data = l_twitter.search( l_tweet[static_cast<std::size_t>(loMPICom.size())], l_tweetmax[static_cast<std::size_t>(loMPICom.size())] );
        
        std::vector<std::string> l_tweetdata;
        std::vector<std::string> l_tweetlabel;
        for(std::size_t i=0; i < l_data.size(); ++i) {
            l_tweetdata.push_back( l_data.getText() );
            l_tweetlabel.push_back( l_tweet[static_cast<std::size_t>(loMPICom.size())] );
        }
        
        #else
        
        std::vector<std::string> l_tweetdata;
        std::vector<std::string> l_tweetlabel;
        
        for(std::size_t n=0; n < l_tweet.size(); ++n) {
            std::vector<tools::sources::twitter::searchtweet> l_data = l_twitter.search( l_tweet[n], l_tweetmax[n] );
    
            for(std::size_t i=0; i < l_data.size(); ++i) {
                l_tweetdata.push_back( l_data[i].getText() );
                l_tweetlabel.push_back( l_tweet[n] );
            }
        }
        
        #endif
        
        
        
        
        
        #ifdef MACHINELEARNING_MPI 
        geht nicht
        #endif
        
        
        
        // create ncd object and calculate the distances
        #ifdef MACHINELEARNING_MPI 
        loMPICom.barrier();
        std::cout << "CPU " << loMPICom.rank() << ": ";
        #endif
        std::cout << "calculate normalized compression distance..." << std::endl;
        
        distances::ncd<double> ncd( boost::any_cast< distances::ncd<double>::compresstype >(l_args["algorithm"]) );
        ncd.setCompressionLevel( boost::any_cast< distances::ncd<double>::compresslevel >(l_args["compress"]) );
        
        #ifdef MACHINELEARNING_MPI 
        geht nicht
        #else
        ublas::matrix<double> distancematrix = ncd.unsymmetric( l_tweetdata );
        #endif
        l_tweetdata.clear();
        
        
        
        
        // run hit mds over the distance matrix
        #ifdef MACHINELEARNING_MPI 
        loMPICom.barrier();
        std::cout << "CPU " << loMPICom.rank() << ": ";
        #endif
        std::cout << "run mds projection..." << std::endl;
        
        dim::mds<double> mds( boost::any_cast<std::size_t>(l_args["dimension"]), boost::any_cast<dim::mds<double>::project>(l_args["mapping"]) );
        mds.setIteration( boost::any_cast<std::size_t>(l_args["iteration"]) );
        mds.setRate( boost::any_cast<double>(l_args["rate"]) );
        
        #ifdef MACHINELEARNING_MPI 
        ?????
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
        tools::files::hdf target(boost::any_cast<std::string>(l_args["outfile"]), true);
        target.writeBlasMatrix<double>( "/project",  project, H5::PredType::NATIVE_DOUBLE );
        #ifndef MACHINELEARNING_MPI
        target.writeStringVector( "/label",  l_tweetlabel );
        target.writeStringVector( "/uniquegroup",  l_tweet );
        #else
        ????
        target.writeStringVector( "/label",  l_alllabel );
        target.writeStringVector( "/uniquegroup",  l_tweet );
        #endif
            
        std::cout << "within the target file there are three datasets: /project = projected data, /label = datapoint label, /uniquegroup = list of unique newsgroups" << std::endl;
        #ifdef MACHINELEARNING_MPI
        }
        #endif
        
        
        return EXIT_SUCCESS;
    }
