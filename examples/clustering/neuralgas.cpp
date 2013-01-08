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
namespace cluster   = machinelearning::clustering::nonsupervised;
namespace distance  = machinelearning::distances;
namespace tools     = machinelearning::tools;
#ifdef MACHINELEARNING_MPI
namespace mpi       = boost::mpi;
#endif



/** main program
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
    bool l_log;
    std::size_t l_iteration;

    // create CML options with description
    po::options_description l_description("allowed options");
    l_description.add_options()
        ("help", "produce help message")
        ("outfile", po::value<std::string>(), "output HDF5 file")
        #ifdef MACHINELEARNING_MPI
        ("inputfile", po::value< std::vector<std::string> >()->multitoken(), "input HDF5 file")
        ("inputpath", po::value< std::vector<std::string> >()->multitoken(), "path to dataset")
        ("prototype", po::value< std::vector<std::size_t> >()->multitoken(), "number of prototypes")
        #else
        ("inputfile", po::value<std::string>(), "input HDF5 file")
        ("inputpath", po::value<std::string>(), "path to dataset")
        ("prototype", po::value<std::size_t>(), "number of prototypes")
        #endif
        ("iteration", po::value<std::size_t>(&l_iteration)->default_value(15), "number of iteration [default: 15]")
        ("log", po::value<bool>(&l_log)->default_value(false), "'true' for enable logging [default: false]")
    ;

    po::variables_map l_map;
    po::positional_options_description l_input;
    po::store(po::command_line_parser(p_argc, p_argv).options(l_description).positional(l_input).run(), l_map);
    po::notify(l_map);

    if (l_map.count("help")) {
        std::cout << l_description << std::endl;
        return EXIT_SUCCESS;
    }

    if ( (!l_map.count("outfile")) || (!l_map.count("inputfile")) || (!l_map.count("inputpath")) || (!l_map.count("prototype")) )
    {
        std::cerr << "[--outfile], [--inputfile], [--inputpath] and [--prototype] option must be set" << std::endl;
        return EXIT_FAILURE;
    }



    #ifdef MACHINELEARNING_MPI
    if(!(
         ((l_map["inputfile"].as< std::vector<std::string> >().size() == static_cast<std::size_t>(l_mpicom.size())) && (l_map["inputpath"].as< std::vector<std::string> >().size() == 1)) ||
         ((l_map["inputpath"].as< std::vector<std::string> >().size() == static_cast<std::size_t>(l_mpicom.size())) && (l_map["inputfile"].as< std::vector<std::string> >().size() == 1)) ||
         ((l_map["inputpath"].as< std::vector<std::string> >().size() == static_cast<std::size_t>(l_mpicom.size())) && (l_map["inputfile"].as< std::vector<std::string> >().size() == static_cast<std::size_t>(l_mpicom.size())))
         ))
        throw std::runtime_error("number of files or number of path must be equal to CPU rank");

    if (l_map["prototype"].as< std::vector<std::size_t> >().size() != static_cast<std::size_t>(l_mpicom.size()))
        throw std::runtime_error("number of prototypes must be equal to CPU rank");
    #endif




    // read source hdf file and data
    #ifdef MACHINELEARNING_MPI
    const std::size_t l_filepos = l_map["inputefile"].as< std::vector<std::size_t> >().size() > 1 ? static_cast<std::size_t>(l_mpicom.rank()) : 0;
    const std::size_t l_pathpos = l_map["inputepath"].as< std::vector<std::size_t> >().size() > 1 ? static_cast<std::size_t>(l_mpicom.rank()) : 0;

    tools::files::hdf l_source( l_map["inputefile"].as< std::vector<std::string> >()[l_filepos] );
    ublas::matrix<double> l_data = l_source.readBlasMatrix<double>( l_map["inputpath"].as< std::vector<std::string> >()[l_pathpos], tools::files::hdf::NATIVE_DOUBLE);

    #else
    tools::files::hdf l_source( l_map["inputfile"].as<std::string>() );
    ublas::matrix<double> l_data = l_source.readBlasMatrix<double>( l_map["inputpath"].as<std::string>(), tools::files::hdf::NATIVE_DOUBLE);
    #endif


    #ifdef MACHINELEARNING_MPI
    cluster::neuralgas<double> l_ng(distance::norm::euclid<double>(), l_map["prototype"].as< std::vector<std::size_t> >()[static_cast<std::size_t>(l_mpicom.rank())], l_data.size2());
    l_ng.setLogging( l_log );

    l_ng.train(l_mpicom, l_data, l_iteration);

    // collect all data (of each process)
    ublas::matrix<double> l_protos = l_ng.getPrototypes(l_mpicom);
    ublas::vector<double> l_qerror;
    std::vector< ublas::matrix<double> > l_logproto;

    if (l_ng.getLogging()) {
        l_qerror      = tools::vector::copy(l_ng.getLoggedQuantizationError(l_mpicom));
        l_logproto    = l_ng.getLoggedPrototypes(l_mpicom);
    }


    // only process 0 writes hdf
    if (l_mpicom.rank() == 0) {
        tools::files::hdf l_target( l_map["outfile"].as<std::string>(), true);

        l_target.writeBlasMatrix<double>( "/protos",  l_protos, tools::files::hdf::NATIVE_DOUBLE );
        l_target.writeValue<double>( "/numprotos",  l_protos.size1(), tools::files::hdf::NATIVE_DOUBLE );
        l_target.writeValue<std::size_t>( "/iteration", l_iteration, tools::files::hdf::NATIVE_ULONG );

        if (l_ng.getLogging()) {
            l_target.writeBlasVector<double>( "/error", l_qerror, tools::files::hdf::NATIVE_DOUBLE );
            for(std::size_t i=0; i < l_logproto.size(); ++i)
                l_target.writeBlasMatrix<double>("/log" + boost::lexical_cast<std::string>( i )+"/protos", l_logproto[i], tools::files::hdf::NATIVE_DOUBLE );
        }
    }

    #else

    cluster::neuralgas<double> l_ng(distance::norm::euclid<double>(), l_map["prototype"].as<std::size_t>(), l_data.size2());
    l_ng.setLogging( l_log );

    l_ng.train(l_data, l_iteration);

    // create target file
    tools::files::hdf l_target(l_map["outfile"].as<std::string>(), true);
    l_target.writeValue<std::size_t>( "/numprotos",  l_map["prototype"].as<std::size_t>(), tools::files::hdf::NATIVE_ULONG );
    l_target.writeBlasMatrix<double>( "/protos",  l_ng.getPrototypes(), tools::files::hdf::NATIVE_DOUBLE );
    l_target.writeValue<std::size_t>( "/iteration",  l_iteration, tools::files::hdf::NATIVE_ULONG );

    if (l_ng.getLogging()) {
        l_target.writeBlasVector<double>( "/error",  tools::vector::copy(l_ng.getLoggedQuantizationError()), tools::files::hdf::NATIVE_DOUBLE );
        std::vector< ublas::matrix<double> > l_logproto = l_ng.getLoggedPrototypes();
        for(std::size_t i=0; i < l_logproto.size(); ++i)
            l_target.writeBlasMatrix<double>("/log" + boost::lexical_cast<std::string>( i )+"/protos", l_logproto[i], tools::files::hdf::NATIVE_DOUBLE );
    }

    #endif


    #ifdef MACHINELEARNING_MPI
    if (l_mpicom.rank() == 0) {
    #endif

    std::cout << "structure of the output file" << std::endl;
    std::cout << "/numprotos \t\t number of prototypes" << std::endl;
    std::cout << "/protos \t\t prototype matrix (row orientated)" << std::endl;
    std::cout << "/iteration \t\t number of iterations" << std::endl;

    if (l_ng.getLogging()) {
        std::cout << "/error \t\t quantization error on each iteration" << std::endl;
        std::cout << "/log<0 to number of iteration-1>/protos \t\t prototypes on each iteration" << std::endl;
    }

    #ifdef MACHINELEARNING_MPI
    }
    #endif

    return EXIT_SUCCESS;
}
