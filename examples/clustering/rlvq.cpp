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

#include <machinelearning.h>
#include <boost/any.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/numeric/ublas/matrix.hpp>

namespace ublas     = boost::numeric::ublas;
namespace cluster   = machinelearning::clustering::supervised;
namespace distance  = machinelearning::distances;
namespace tools     = machinelearning::tools;


/** read all input arguments 
 * @param argc number of arguments of "main"
 * @param argv arguments of "main"
 * @param p_args map with argument values (default values)
 * @return bool if all is correct
 **/
bool cliArguments( int argc, char* argv[], std::map<std::string, boost::any>& p_args ) {
    
    if (argc < 2) {
        std::cout << "--outfile \t\t output HDF5 file" << std::endl;
        std::cout << "--iteration \t\t number of iteration" << std::endl;
        std::cout << "--inputfile \t\t input HDF5 file" << std::endl;
        std::cout << "--inputpath \t\t path to dataset" << std::endl;
        std::cout << "--labelpath \t\t path to the data labels" << std::endl;
        std::cout << "--labeltype \t\t datatype of labels (values: string, uint, int)" << std::endl;
        std::cout << "--log \t\t\t 'on' for enable logging" << std::endl;
        
        return false;
    }
    
    
    // set default arguments
    std::map<std::string, std::vector<std::string> > l_argmap;
    l_argmap["inputfile"] = std::vector<std::string>();
    l_argmap["inputpath"] = std::vector<std::string>();
    l_argmap["outfile"]   = std::vector<std::string>();
    l_argmap["iteration"] = std::vector<std::string>();
    l_argmap["labelpath"] = std::vector<std::string>();
    l_argmap["labeltype"] = std::vector<std::string>();
    l_argmap["log"]       = std::vector<std::string>();
    
    
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
    if ( (l_argmap["iteration"].size() != 1) || 
        (l_argmap["prototype"].size() != 1) || 
        (l_argmap["outfile"].size() != 1) || 
        (l_argmap["inputfile"].size() != 1) || 
        (l_argmap["inputpath"].size() != 1) ||
        (l_argmap["labelpath"].size() != 1) ||
        (l_argmap["labeltype"].size() != 1)
        )
        throw std::runtime_error("number of arguments are incorrect");
    
    p_args["log"]           = l_argmap["log"].size() > 0;
    p_args["inputfile"]     = l_argmap["inputfile"][0];
    p_args["inputpath"]     = l_argmap["inputpath"][0];
    p_args["outfile"]       = l_argmap["outfile"][0];
    p_args["labelpath"]     = l_argmap["labelpath"][0];
    
    p_args["labeltype"]     = std::string("");
    if (l_argmap["labeltype"].size()) {
        std::string lc      = l_argmap["labeltype"][0];
        boost::to_lower(lc);
    
        if ((lc == "string") || (lc == "int") || (lc == "uint"))
            p_args["labeltype"] = lc;
    }
    if ((boost::any_cast<std::string>(p_args["labelpath"])).empty())
        throw std::runtime_error("label data unkown");
    
    
    try {
        p_args["iteration"] = boost::lexical_cast<std::size_t>( l_argmap["iteration"][0] );
    } catch (...) {
        throw std::runtime_error("numerical data can not extracted");
    }  
    
    return true;
}



/** main program
 * @param argc number of arguments
 * @param argv arguments
 **/
int main(int argc, char* argv[]) {
    
    std::map<std::string, boost::any> l_args;
    if (!cliArguments(argc, argv, l_args))
        return EXIT_FAILURE;
    
    
    // read source hdf file and data
    tools::files::hdf source( boost::any_cast<std::string>(l_args["inputfile"]) );
    ublas::matrix<double> data = source.readBlasMatrix<double>(boost::any_cast<std::string>(l_args["inputpath"]), H5::PredType::NATIVE_DOUBLE);
    
    // create target file
    tools::files::hdf target(boost::any_cast<std::string>(l_args["outfile"]), true);
    target.writeValue<std::size_t>( "/iteration",  boost::any_cast<std::size_t>(l_args["iteration"]), H5::PredType::NATIVE_ULONG );
    
    // create data
    distance::euclid<double> dist;
    
    
    
    // rlvq with uint labels
    if (boost::any_cast<std::string>(l_args["labeltype"]) == "uint") {
        std::vector<std::size_t> labels = tools::vector::copy( source.readBlasVector<std::size_t>(boost::any_cast<std::string>(l_args["labelpath"]), H5::PredType::NATIVE_ULONG) );
        std::vector<std::size_t> unique = tools::vector::unique(labels);
        
        // create rlvq object
        cluster::rlvq<double, std::size_t> rlvq(dist, unique, data.size2());
        rlvq.setLogging( boost::any_cast<bool>(l_args["log"]) );
        
        // train data
        rlvq.train( data, labels, boost::any_cast<std::size_t>(l_args["iteration"]) );
        
        
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
    if (boost::any_cast<std::string>(l_args["labeltype"]) == "int") {
        std::vector<std::ptrdiff_t> labels = tools::vector::copy( source.readBlasVector<std::ptrdiff_t>(boost::any_cast<std::string>(l_args["labelpath"]), H5::PredType::NATIVE_LONG) );
        std::vector<std::ptrdiff_t> unique = tools::vector::unique(labels);
        
        // create rlvq object
        cluster::rlvq<double, std::ptrdiff_t> rlvq(dist, unique, data.size2());
        rlvq.setLogging( boost::any_cast<bool>(l_args["log"]) );
        
        // train data
        rlvq.train( data, labels, boost::any_cast<std::size_t>(l_args["iteration"]) );
        
        
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
    if (boost::any_cast<std::string>(l_args["labeltype"]) == "string") {
        std::vector<std::string> labels = source.readStringVector(boost::any_cast<std::string>(l_args["labelpath"]));
        std::vector<std::string> unique = tools::vector::unique(labels);
        
        // create rlvq object
        cluster::rlvq<double, std::string> rlvq(dist, unique, data.size2());
        rlvq.setLogging( boost::any_cast<bool>(l_args["log"]) );
        
        // train data
        rlvq.train( data, labels, boost::any_cast<std::size_t>(l_args["iteration"]) );
        
        
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
    std::cout << "/numprotos" << "\t\t" << "number of prototypes" << std::endl;
    std::cout << "/protolabel" << "\t\t" << "label of prototypes" << std::endl;
    std::cout << "/protos" << "\t\t" << "prototype matrix (row orientated)" << std::endl;
    std::cout << "/iteration" << "\t\t" << "number of iterations" << std::endl;
    
    if (boost::any_cast<bool>(l_args["log"])) {
        std::cout << "/error" << "\t\t" << "quantization error on each iteration" << std::endl;
        std::cout << "/log<0 to number of iteration-1>/protosos" << "\t\t" << "prototypes on each iteration" << std::endl;
    }
    
    return EXIT_SUCCESS;
}