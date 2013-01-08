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


/** create the group list with the arguments
 * @param p_nntp nntp object
 * @param p_groups group parameters
 * @return list with group names
**/
std::vector<std::string> createGroupList( tools::sources::nntp& p_nntp, const std::vector<std::string>& p_groups )
{
    // check if first element is a number or "rand <number>"
    if (p_groups.size() == 1) {
        std::size_t l_num = 0;
        try {
            l_num = boost::lexical_cast<std::size_t>(p_groups[0]);
        } catch (...) {}

        if (l_num > 0) {
            std::vector<std::string> l_groups;
            std::map<std::string, std::size_t> l_ingroup = p_nntp.getGroupList();

            // create vector with n elements (frequency itemset)
            std::vector< std::pair<std::string, std::size_t> > l_max;
            for(std::size_t i=0; i < l_num; ++i)
                l_max.push_back( std::pair<std::string, std::size_t>("", 0) );

            // we read the group list and get the the groups with the most entries (frequent itemsets)
            for(std::map<std::string, std::size_t>::iterator it = l_ingroup.begin(); it != l_ingroup.end(); it++)
                if ((*it).second > l_max[0].second) {
                    l_max.push_back( std::pair<std::string, std::size_t>( (*it).first, (*it).second) );
                    l_max.erase( l_max.begin(), l_max.begin()+1 );
                }

            for(std::size_t i=0; i < l_max.size(); ++i)
                l_groups.push_back( l_max[i].first );

            return l_groups;
        }
    }

    // check if the first element "rand" and the second a number
    if ((p_groups.size() == 2) && (p_groups[0] == "rand")) {
        std::size_t l_num = 0;
        try {
            l_num = boost::lexical_cast<std::size_t>(p_groups[1]);
        } catch (...) {}

        if (l_num > 0) {
            tools::random l_rand;
            std::vector<std::string> l_groups;
            std::map<std::string, std::size_t> l_ingroup = p_nntp.getGroupList();

            for(std::size_t i=0; i < l_num; ++i) {
                const unsigned int l_id = static_cast<std::size_t>(l_rand.get<double>(tools::random::uniform, 0, l_ingroup.size()-1));
                std::map<std::string, std::size_t>::iterator it = l_ingroup.begin();

                std::advance(it, l_id);
                l_groups.push_back( (*it).first );
            }
            return l_groups;
        }
    }

    // otherwise return list
    return p_groups;
}


/** read articles
 * @param p_nntp nntp object
 * @param p_numarticles number of articles
 * @param p_groups list with group names
 * @param p_article vector with returned articles
 * @param p_articlegroup returned vector with group names for each article
**/
void getArticles( tools::sources::nntp& p_nntp, const std::size_t& p_numarticles, const std::vector<std::string>& p_groups, std::vector<std::string>& p_article, std::vector<std::string>& p_articlegroup )
{
    // read group size
    std::map<std::string, std::size_t> l_numarticles;
    std::size_t l_allcount = 0;

    for(std::size_t i=0; i < p_groups.size(); ++i) {
        std::map<std::string, std::size_t> l_groupcount = p_nntp.getGroupList( p_groups[i] );
        l_numarticles[p_groups[i]] =  l_groupcount[p_groups[i]];
        l_allcount += l_groupcount[p_groups[i]];
    }

    if (l_allcount < 2*p_numarticles)
        throw std::runtime_error("not enough articles within the groups");

    // determine percent articles for each group in conclusion with their size
    std::size_t l_count = 0;
    for(std::map<std::string, std::size_t>::iterator it = l_numarticles.begin(); it != l_numarticles.end(); it++) {
        (*it).second = static_cast<std::size_t>(static_cast<double>((*it).second) / l_allcount * p_numarticles);
        l_count += (*it).second;
    }

    if (l_count != p_numarticles)
        (*l_numarticles.begin()).second += p_numarticles - l_count;

    // read articles
    p_article.clear();
    p_articlegroup.clear();

    tools::random l_rand;
    while (p_article.size() < p_numarticles)
        for(std::map<std::string, std::size_t>::iterator it = l_numarticles.begin(); it != l_numarticles.end(); it++) {
            if ((*it).second == 0)
                continue;

            std::size_t n=0;
            p_nntp.setGroup( (*it).first );
            while ((n < (*it).second) && p_nntp.nextArticle()) {
                const std::string l_text = p_nntp.getArticle();

                if ( (!p_nntp.isArticleCanceled()) && (l_rand.get<double>(tools::random::uniform) < 0.5) && (!l_text.empty()) ) {
                    p_article.push_back( l_text );
                    p_articlegroup.push_back( (*it).first );
                    n++;

                    if (p_article.size() >= p_numarticles)
                        break;
                }
            }

            if (p_article.size() >= p_numarticles)
                break;
	}
}



/** main program, that reads a subset of newsgroup articles, calculate the distance between articles
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
        ("server", po::value<std::string>(), "IP / address of the NNTP server")
        ("groups", po::value< std::vector<std::string> >()->multitoken(), "list of group names or \"rand n\" random n groups or number of groups (using of the the i-th biggest groups)")
        #ifdef MACHINELEARNING_MPI
        ("articles", po::value< std::vector<std::size_t> >()->multitoken(), "number of articles")
        #else
        ("articles", po::value<std::size_t>(), "number of articles")
        #endif
        ("outfile", po::value<std::string>(), "output HDF5 file")
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

    if ( (!l_map.count("outfile")) || (!l_map.count("articles")) || (!l_map.count("groups")) || (!l_map.count("server")) )  {
        std::cerr << "[--server], [--outfile], [--groups] and [--articles] option must be set" << std::endl;
        return EXIT_FAILURE;
    }



    // connect to server
    tools::sources::nntp l_nntp( l_map["server"].as<std::string>() );

    // read group list
    #ifdef MACHINELEARNING_MPI
    l_mpicom.barrier();
    std::cout << "CPU " << l_mpicom.rank() << ": ";
    #endif
    std::cout << "getting group data..." << std::endl;
    std::vector<std::string> l_groups = createGroupList(l_nntp, l_map["groups"].as< std::vector<std::string> >());

    // read article data
    #ifdef MACHINELEARNING_MPI
    l_mpicom.barrier();
    std::cout << "CPU " << l_mpicom.rank() << ": ";
    #endif
    std::cout << "read article data..." << std::endl;
    std::vector<std::string> l_article;
    std::vector<std::string> l_articlegroup;

    #ifdef MACHINELEARNING_MPI
    std::vector<std::size_t> l_artnum = l_map["articles"].as< std::vector<std::size_t> >();
    if (l_artnum.size() != static_cast<std::size_t>(l_mpicom.size()))
        throw std::runtime_error("number of articles and used CPUs are not equal");

    getArticles( l_nntp, l_artnum[l_mpicom.rank()], l_groups, l_article, l_articlegroup );
    #else
    getArticles( l_nntp, l_map["articles"].as<std::size_t>(), l_groups, l_article, l_articlegroup );
    #endif

    if (l_article.size() == 0)
        throw std::runtime_error("no articles are readed");



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
            l_tfc.add(l_article);
            text::stopwordreduction l_stopword( l_tfc.getTerms(l_val[0], l_val[1] ), l_tfc.iscaseinsensitivity() );
            for(std::size_t i=0; i < l_article.size(); ++i)
                l_article[i] = l_stopword.remove( l_article[i] );
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
    ublas::matrix<double> l_distancematrix = l_ncd.unsquare( l_mpicom, l_article );
    #else
    ublas::matrix<double> l_distancematrix = l_ncd.unsymmetric( l_article );
    #endif
    l_article.clear();




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
    l_target.writeStringVector( "/group",  l_articlegroup );
    l_target.writeStringVector( "/uniquegroup",  tools::vector::unique(l_articlegroup) );
    
    
    #ifdef MACHINELEARNING_MPI
    if (l_mpicom.rank() == 0)
    #endif
    std::cout << "within the target file there are three datasets: /project = projected data, /group = newsgroup label of each dataset, /uniquegroup = list of unique newsgroups" << std::endl;


    return EXIT_SUCCESS;
}
