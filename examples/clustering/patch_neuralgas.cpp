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
 * @param argc number of arguments
 * @param argv arguments
 **/
int main(int argc, char* argv[])
{

    #ifdef MACHINELEARNING_MPI
    mpi::environment loMPIenv(argc, argv);
    mpi::communicator loMPICom;
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
        ("inputpath", po::value< std::vector<std::string> >()->multitoken(), "path to dataset")
        #ifdef MACHINELEARNING_MPI
        ("inputfile", po::value< std::vector<std::string> >()->multitoken(), "input HDF5 file")
        ("prototype", po::value< std::vector<std::size_t> >()->multitoken(), "number of prototypes")
        #else
        ("inputfile", po::value<std::string>(), "input HDF5 file")
        ("prototype", po::value<std::size_t>(), "number of prototypes")
        #endif
        ("iteration", po::value<std::size_t>(&l_iteration)->default_value(15), "number of iteration [default: 15]")
        ("log", po::value<bool>(&l_log)->default_value(false), "'true' for enable logging [default: false]")
    ;

    po::variables_map l_map;
    po::positional_options_description l_input;
    po::store(po::command_line_parser(argc, argv).options(l_description).positional(l_input).run(), l_map);
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
    // we check CPU size and number of files
    if (l_map["inputfile"].as< std::vector<std::string> >().size() == static_cast<std::size_t>(loMPICom.size()))
        throw std::runtime_error("number of process and number of source files must be equal");

    if ( static_cast<std::size_t>(loMPICom.size()) != l_map["prototype"].as< std::vector<std::size_t> >().size())
        throw std::runtime_error("number of process and number of input prototypes must be equal");
    #endif



    #ifdef MACHINELEARNING_MPI
    tools::files::hdf source( l_map["inputfile"].as< std::vector<std::string> >()[static_cast<std::size_t>(loMPICom.rank())] );
    #else
    tools::files::hdf source( l_map["inputfile"].as<std::string>() );
    #endif



    // create ng objects
    distance::euclid<double> d;
    #ifdef MACHINELEARNING_MPI
    cluster::neuralgas<double> ng(d,
                l_map["prototype"].as< std::vector<std::size_t> >()[static_cast<std::size_t>(loMPICom.rank())],
                source.readBlasMatrix<double>(l_map["inputpath"].as< std::vector<std::string> >()[0], tools::files::hdf::NATIVE_DOUBLE).size2()
    );
    #else
    cluster::neuralgas<double> ng(d,
                l_map["prototype"].as<std::size_t>(),
                source.readBlasMatrix<double>(l_map["inputpath"].as< std::vector<std::string> >()[0], tools::files::hdf::NATIVE_DOUBLE).size2()
    );
    #endif
    ng.setLogging( l_log );


    #ifdef MACHINELEARNING_MPI

    //create target file (only on the first process)
    tools::files::hdf* target = NULL;
    if (loMPICom.rank() == 0)
        target = new tools::files::hdf( l_map["outfile"].as<std::string>(), true );

    // do each patch
    for (std::size_t i=0; i < l_map["inputpath"].as< std::vector<std::string> >().size(); ++i) {

        ng.trainpatch( loMPICom,
                       source.readBlasMatrix<double>( l_map["inputpath"].as< std::vector<std::string> >()[i], tools::files::hdf::NATIVE_DOUBLE),
                       l_iteration
                     );

        ublas::vector<double> weights                  = ng.getPrototypeWeights(loMPICom);
        ublas::matrix<double> protos                   = ng.getPrototypes(loMPICom);
        ublas::vector<double> qerror                   = tools::vector::copy(ng.getLoggedQuantizationError(loMPICom));
        std::vector< ublas::matrix<double> > logproto  = ng.getLoggedPrototypes(loMPICom);

        if (target) {
            std::string patchpath = "/patch" + boost::lexical_cast<std::string>(i);

            target->writeBlasVector<double>( patchpath+"/weights",  weights, tools::files::hdf::NATIVE_DOUBLE );
            target->writeBlasMatrix<double>( patchpath+"/protos",  protos, tools::files::hdf::NATIVE_DOUBLE );

            if (ng.getLogging()) {
                target->writeBlasVector<double>( patchpath+"/error", qerror, tools::files::hdf::NATIVE_DOUBLE );

                for(std::size_t j=0; j < logproto.size(); ++j)
                    target->writeBlasMatrix<double>( patchpath+"/log" + boost::lexical_cast<std::string>( j )+"/protos", logproto[j], tools::files::hdf::NATIVE_DOUBLE );
            }
        }
    }


    ublas::vector<double> weights    = ng.getPrototypeWeights(loMPICom);
    ublas::matrix<double> protos     = ng.getPrototypes(loMPICom);
    if (target) {
        target->writeValue<std::size_t>( "/numprotos",  protos.size1(), tools::files::hdf::NATIVE_ULONG );
        target->writeBlasMatrix<double>( "/protos",  protos, tools::files::hdf::NATIVE_DOUBLE );
        target->writeBlasVector<double>( "/weights",  weights, tools::files::hdf::NATIVE_DOUBLE );
        target->writeValue<std::size_t>( "/iteration",   l_iteration, tools::files::hdf::NATIVE_ULONG );
    }

    delete(target);

    #else

    //create target file
    tools::files::hdf target( l_map["outfile"].as<std::string>(), true );

    // do each patch
    for (std::size_t i=0; i < l_map["inputpath"].as< std::vector<std::string> >().size(); ++i) {
        ng.trainpatch(
                       source.readBlasMatrix<double>( l_map["inputpath"].as< std::vector<std::string> >()[i], tools::files::hdf::NATIVE_DOUBLE),
                       l_iteration
                     );

        std::string patchpath = "/patch" + boost::lexical_cast<std::string>(i);
        target.writeBlasVector<double>( patchpath+"/weights",  ng.getPrototypeWeights(), tools::files::hdf::NATIVE_DOUBLE );
        target.writeBlasMatrix<double>( patchpath+"/protos",  ng.getPrototypes(), tools::files::hdf::NATIVE_DOUBLE );

        if (ng.getLogging()) {
            target.writeBlasVector<double>( patchpath+"/error",  tools::vector::copy(ng.getLoggedQuantizationError()), tools::files::hdf::NATIVE_DOUBLE );

            std::vector< ublas::matrix<double> > logproto =  ng.getLoggedPrototypes();
            for(std::size_t j=0; j < logproto.size(); ++j)
                target.writeBlasMatrix<double>( patchpath+"/log"+boost::lexical_cast<std::string>(j)+"/protos", logproto[j], tools::files::hdf::NATIVE_DOUBLE );
        }
    }

    target.writeValue<std::size_t>( "/numprotos",   l_map["prototype"].as<std::size_t>(), tools::files::hdf::NATIVE_ULONG );
    target.writeBlasMatrix<double>( "/protos",  ng.getPrototypes(), tools::files::hdf::NATIVE_DOUBLE );
    target.writeBlasVector<double>( "/weights",  ng.getPrototypeWeights(), tools::files::hdf::NATIVE_DOUBLE );
    target.writeValue<std::size_t>( "/iteration",   l_iteration, tools::files::hdf::NATIVE_ULONG );

    #endif




    // show info
    #ifdef MACHINELEARNING_MPI
    if (loMPICom.rank() == 0) {
    #endif

    std::cout << "structure of the output file" << std::endl;
    std::cout << "/numprotos \t\t number of prototypes" << std::endl;
    std::cout << "/protos \t\t prototype matrix (row orientated)" << std::endl;
    std::cout << "/weight \t\t prototype weight" << std::endl;
    std::cout << "/iteration \t\t number of iterations" << std::endl;

    if (ng.getLogging()) {
        std::cout << "/patch<0 to number of patches-1>/weights \t\t prototype weights after calculating patch" << std::endl;
        std::cout << "/patch<0 to number of patches-1>/protos \t\t prototypes after calculating patch" << std::endl;
        std::cout << "/patch<0 to number of patches-1>/error \t\t quantization error on each patch iteration" << std::endl;
        std::cout << "/patch<0 to number of patches-1>/log<0 to number of iteration-1>/protos \t\t prototypes on each patch iteration" << std::endl;
    }

    #ifdef MACHINELEARNING_MPI
    }
    #endif

    return EXIT_SUCCESS;
}
