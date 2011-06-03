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

#include <machinelearning.h>
#include <boost/any.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/numeric/ublas/matrix.hpp>

namespace ublas     = boost::numeric::ublas;
namespace cluster   = machinelearning::clustering::nonsupervised;
namespace tools     = machinelearning::tools;
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
        std::cout << "--inputfile \t\t input HDF5 file" << std::endl;
        std::cout << "--inputpath \t\t path to dataset" << std::endl;
        std::cout << "--outfile \t\t output HDF5 file" << std::endl;
        std::cout << "--iteration \t\t number of iteration" << std::endl;
        std::cout << "--prototype \t\t number of prototypes" << std::endl;
        std::cout << "--log \t\t\t 'on' for enable logging" << std::endl;
        #ifdef MACHINELEARNING_MPI
        }
        #endif
        return false;
    }
    
    
    // set default arguments
    std::map<std::string, std::vector<std::string> > l_argmap;
    l_argmap["inputfile"] = std::vector<std::string>();
    l_argmap["inputpath"] = std::vector<std::string>();
    l_argmap["outfile"]   = std::vector<std::string>();
    l_argmap["iteration"] = std::vector<std::string>();
    l_argmap["prototype"] = std::vector<std::string>();
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
        (l_argmap["outfile"].size() != 1)
        #ifndef MACHINELEARNING_MPI
        || 
        (l_argmap["prototype"].size() != 1) || 
        (l_argmap["inputfile"].size() != 1) || 
        (l_argmap["inputpath"].size() != 1)
        #endif
        )
        throw std::runtime_error("number of arguments are incorrect");
    
    p_args["log"]           = l_argmap["log"].size() > 0;
    #ifndef MACHINELEARNING_MPI
    p_args["inputfile"]     = l_argmap["inputfile"][0];
    p_args["inputpath"]     = l_argmap["inputpath"][0];
    #else
    p_args["inputfile"]     = l_argmap["inputfile"];
    p_args["inputpath"]     = l_argmap["inputpath"];
    #endif
    p_args["outfile"]       = l_argmap["outfile"][0];
    
    try {
        p_args["iteration"] = boost::lexical_cast<std::size_t>( l_argmap["iteration"][0] );
        #ifndef MACHINELEARNING_MPI
        p_args["prototype"]  = boost::lexical_cast<std::size_t>( l_argmap["prototype"][0] );
        #else
        std::vector<std::size_t> la;
        for(std::size_t i=0; i < l_argmap["prototype"].size(); ++i)
            la.push_back( boost::lexical_cast<std::size_t>(l_argmap["prototype"][i]) );
        p_args["prototype"] = la;
        #endif
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
    
    #ifdef MACHINELEARNING_MPI
    mpi::environment loMPIenv(argc, argv);
    mpi::communicator loMPICom;
    
    if (!cliArguments(argc, argv, l_args, loMPICom))
    #else
    if (!cliArguments(argc, argv, l_args))
    #endif
        return EXIT_FAILURE;

    
    #ifdef MACHINELEARNING_MPI
    if (!( ((boost::any_cast< std::vector<std::string> >(l_args["inputfile"]).size() == static_cast<std::size_t>(loMPICom.size())) && (boost::any_cast< std::vector<std::string> >(l_args["inputpath"]).size() == 1)) || ((boost::any_cast< std::vector<std::string> >(l_args["inputpath"]).size() == static_cast<std::size_t>(loMPICom.size())) && (boost::any_cast< std::vector<std::string> >(l_args["inputfile"]).size() == 1))
       ))
        throw std::runtime_error("number of files or number of path must be equal to CPU rank");
        
    if (boost::any_cast< std::vector<std::size_t> >(l_args["prototype"]).size() != static_cast<std::size_t>(loMPICom.size()))
        throw std::runtime_error("number of prototypes must be equal to CPU rank");
    #endif
        
    
    // read source hdf file and data
    #ifdef MACHINELEARNING_MPI
    std::size_t filepos = 0;
    if (boost::any_cast< std::vector<std::string> >(l_args["inputfile"]).size() > 1)
        filepos = static_cast<std::size_t>(loMPICom.rank());
    
    tools::files::hdf source( boost::any_cast< std::vector<std::string> >(l_args["inputfile"])[filepos] );
    
    
    std::size_t pathpos = 0;
    if (boost::any_cast< std::vector<std::string> >(l_args["inputpath"]).size() > 1)
        pathpos = static_cast<std::size_t>(loMPICom.rank());
    
    ublas::matrix<double> data = source.readBlasMatrix<double>(boost::any_cast<std::vector<std::string> >(l_args["inputpath"])[pathpos], H5::PredType::NATIVE_DOUBLE);

    #else
    tools::files::hdf source( boost::any_cast<std::string>(l_args["inputfile"]) );
    ublas::matrix<double> data = source.readBlasMatrix<double>(boost::any_cast<std::string>(l_args["inputpath"]), H5::PredType::NATIVE_DOUBLE);
    #endif
    

    
    #ifdef MACHINELEARNING_MPI
    cluster::relational_neuralgas<double> ng(boost::any_cast<std::vector<std::size_t> >(l_args["prototype"])[static_cast<std::size_t>(loMPICom.rank())], data.size2());
    ng.setLogging( boost::any_cast<bool>(l_args["log"]) );
        
    ng.train(loMPICom, data, boost::any_cast<std::size_t>(l_args["iteration"]));
        
    // collect all data (of each process)
    ublas::matrix<double> protos = ng.getPrototypes(loMPICom);
    ublas::vector<double> qerror;
    std::vector< ublas::matrix<double> > logproto;
        
    if (ng.getLogging()) {
        qerror      = tools::vector::copy(ng.getLoggedQuantizationError(loMPICom));
        logproto    = ng.getLoggedPrototypes(loMPICom);
    }
        
        
    // only process 0 writes hdf
    if (loMPICom.rank() == 0) {
        tools::files::hdf target(boost::any_cast<std::string>(l_args["outfile"]), true);

        target.writeBlasMatrix<double>( "/protos",  tools::matrix::setNumericalZero(protos), H5::PredType::NATIVE_DOUBLE );
        target.writeValue<std::size_t>( "/numprotos",  protos.size1(), H5::PredType::NATIVE_ULONG );
        target.writeValue<std::size_t>( "/iteration", boost::any_cast<std::size_t>(l_args["iteration"]), H5::PredType::NATIVE_ULONG );
            
        if (ng.getLogging()) {
            target.writeBlasVector<double>( "/error", qerror, H5::PredType::NATIVE_DOUBLE );
            for(std::size_t i=0; i < logproto.size(); ++i)
                target.writeBlasMatrix<double>("/log" + boost::lexical_cast<std::string>( i )+"/protos", logproto[i], H5::PredType::NATIVE_DOUBLE );
        }
    }
    
    #else    
    
    cluster::relational_neuralgas<double> ng(boost::any_cast<std::size_t>(l_args["prototype"]), data.size2());
    ng.setLogging( boost::any_cast<bool>(l_args["log"]) );
    
    ng.train(data, boost::any_cast<std::size_t>(l_args["iteration"]));
        
    // create target file
    tools::files::hdf target(boost::any_cast<std::string>(l_args["outfile"]), true);
    target.writeValue<std::size_t>( "/numprotos",  boost::any_cast<std::size_t>(l_args["prototype"]), H5::PredType::NATIVE_ULONG );
    target.writeBlasMatrix<double>( "/protos",  tools::matrix::setNumericalZero(ng.getPrototypes()), H5::PredType::NATIVE_DOUBLE );    
    target.writeValue<std::size_t>( "/iteration",  boost::any_cast<std::size_t>(l_args["iteration"]), H5::PredType::NATIVE_ULONG );
        
    if (ng.getLogging()) {
        target.writeBlasVector<double>( "/error",  tools::vector::copy(ng.getLoggedQuantizationError()), H5::PredType::NATIVE_DOUBLE );
        std::vector< ublas::matrix<double> > logproto =  ng.getLoggedPrototypes();
        for(std::size_t i=0; i < logproto.size(); ++i)
            target.writeBlasMatrix<double>("/log" + boost::lexical_cast<std::string>( i )+"/protos", logproto[i], H5::PredType::NATIVE_DOUBLE );
    }
    
    #endif
    
    
    #ifdef MACHINELEARNING_MPI
    if (loMPICom.rank() == 0) {
    #endif

    std::cout << "structure of the output file" << std::endl;
    std::cout << "/numprotos \t\t number of prototypes" << std::endl;
    std::cout << "/protos \t\t prototype matrix (row orientated)" << std::endl;
    std::cout << "/iteration \t\t number of iterations" << std::endl;
    
    if (ng.getLogging()) {
        std::cout << "/error \t\t quantization error on each iteration" << std::endl;
        std::cout << "/log<0 to number of iteration-1>/protosos \t\t prototypes on each iteration" << std::endl;
    }
            
    #ifdef MACHINELEARNING_MPI
    }
    #endif
        
    return EXIT_SUCCESS;
}