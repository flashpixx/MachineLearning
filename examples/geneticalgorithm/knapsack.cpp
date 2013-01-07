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
#include <sstream>
#include <machinelearning.h>
#include <boost/lexical_cast.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/options_description.hpp>


namespace po        = boost::program_options;
namespace ublas     = boost::numeric::ublas;
namespace tools     = machinelearning::tools;
namespace ga        = machinelearning::geneticalgorithm;


/** @cond
 fitness function for determine the binary packing **/
template<typename T, typename L> class fitness : public ga::fitness::fitness<T,L>
{
    public :

        fitness( const ublas::vector<T>& p_weight, const T& p_max ) : m_optimum(false), m_weight(p_weight), m_max(p_max) {}

        T getFitness( const ga::individual::individual<L>& p_ind )
        {
            // simple fitness function: sum over the set weights
            T l_sum = 0;
            for(std::size_t i=0; i < m_weight.size(); ++i)
                l_sum += m_weight(i) * p_ind[i];

            m_optimum = tools::function::isNumericalEqual(l_sum, m_max);
            return l_sum > m_max ? 0.0 : l_sum;
        }

        bool isOptimumReached( void ) const
        {
            return m_optimum;
        }

        void clone( boost::shared_ptr< ga::fitness::fitness<T,L> >& p_ptr ) const
        {
            p_ptr = boost::shared_ptr< ga::fitness::fitness<T,L> >( new fitness(m_weight, m_max) );
        }

        void onEachIteration( const std::vector< boost::shared_ptr< ga::individual::individual<L> > >& ) {}


    private :

        bool m_optimum;
        const ublas::vector<T> m_weight;
        const T m_max;
};
/** @endcond **/


/** main program for using the genetic algorithm to solve
 * the binary packing problem (Knapsack problem)
 * @param p_argc number of arguments
 * @param p_argv arguments
 **/
int main(int p_argc, char* p_argv[])
{
    #ifdef MACHINELEARNING_MULTILANGUAGE
    tools::language::bindings::bind();
    #endif
    
    std::size_t l_populationsize;
    std::size_t l_elitesize;
    std::size_t l_iteration;
    std::size_t l_cuts;
    double l_packsize;
    double l_mutation;


    // create CML options with description
    po::options_description l_description("allowed options");
    l_description.add_options()
        ("help", "produce help message")
        ("packs", po::value< std::vector<double> >()->multitoken(), "weights / costs of the different packs")
        ("maxpacksize", po::value<double>(&l_packsize), "maximum pack size")
        ("population", po::value<std::size_t>(&l_populationsize)->default_value(100), "population size / number of individuals")
        ("elite", po::value<std::size_t>(&l_elitesize)->default_value(5), "elite size / number of individuals that are elite")
        ("crossover", po::value<std::size_t>(&l_cuts)->default_value(2), "cut point of the crossover")
        ("selection", po::value< std::vector<std::string> >()->multitoken(), "type of selection (values: bestof <number = 3> [default], roulette)")
        ("iteration", po::value<std::size_t>(&l_iteration)->default_value(25), "number of iterations")
        ("mutation", po::value<double>(&l_mutation)->default_value(0.65), "mutation probability")
    ;

    po::variables_map l_map;
    po::positional_options_description l_input;
    po::store(po::command_line_parser(p_argc, p_argv).options(l_description).positional(l_input).run(), l_map);
    po::notify(l_map);

    if (l_map.count("help")) {
        std::cout << l_description << std::endl;
        return EXIT_SUCCESS;
    }

    if ( (!l_map.count("packs")) || (!l_map.count("maxpacksize")) )  {
        std::cerr << "[--packs] and [--maxpacksize] must be set" << std::endl;
        return EXIT_FAILURE;
    }

    if (l_map["packs"].as< std::vector<double> >().size() < 2) {
        std::cerr << "[--packs] must be greater or equal two" << std::endl;
        return EXIT_FAILURE;
    }


    std::string l_selectionopt("bestof");
    std::size_t l_selectionnumber(3);

    if ( (l_map.count("selection")) && (l_map["selection"].as< std::vector<std::string> >().size() > 0) ) {
        l_selectionopt = l_map["selection"].as< std::vector<std::string> >()[0];

        if ((l_selectionopt == "bestof") && (l_map["selection"].as< std::vector<std::string> >().size() > 1))
            l_selectionnumber = boost::lexical_cast<std::size_t>(l_map["selection"].as< std::vector<std::string> >()[1]);
    }


    const ublas::vector<double> l_packs = tools::vector::copy(l_map["packs"].as< std::vector<double> >());

    if (l_map["maxpacksize"].as<double>() > ublas::sum(l_packs)) {
        std::cerr << "sum of pack values must be equal or smaller than maximum pack size (all packs can be used)" << std::endl;
        return EXIT_FAILURE;
    }


    // genetic algorithm (basic structure eg individual, fitness function, crossover function [see above])
    fitness<double,unsigned char> l_fitness( l_packs, l_map["maxpacksize"].as<double>() );
    ga::individual::binaryindividual<unsigned char> l_individual( l_packs.size() );
    ga::crossover::kcrossover<unsigned char> l_crossover(l_cuts);
    ga::selection::selection<double,unsigned char>* l_selection = NULL;

    if (l_selectionopt == "roulette")
        l_selection = new ga::selection::roulettewheel<double,unsigned char>();

    if ( (l_selectionopt == "bestof") || (!l_selection) )
        l_selection = new ga::selection::bestof<double,unsigned char>(l_selectionnumber);



    // create population, iterate the data and return a copy of elite data
    ga::population<double,unsigned char> l_population(l_individual, l_populationsize, l_elitesize);

    l_population.setMutalProbability( l_map["mutation"].as<double>() );
    l_population.iterate( l_iteration, l_fitness, *l_selection, l_crossover );

    delete l_selection;
    const std::vector< boost::shared_ptr< ga::individual::individual<unsigned char> > > l_elite = l_population.getElite();



    // create output
    std::cout << "best packing options with pack / position values [ value, value, ... ] (position starts with one):" << std::endl;

    for(std::size_t i=0; i < l_elite.size(); ++i) {

        double l_sum = 0;
        std::stringstream  l_pos;
        std::stringstream  l_packvalue;
        bool l_first = true;

        for(std::size_t j=0; j < l_elite[i]->size(); ++j)
            if ( (*(l_elite[i]))[j] ) {

                if (!l_first) {
                    l_pos << ", ";
                    l_packvalue << ", ";
                }

                l_first = false;
                l_pos << (j+1);
                l_packvalue << l_packs[j];
                l_sum += l_packs[j];
            }

        std::cout << (i+1) << ".\tpack value: " << l_sum << " = [" << l_packvalue.str() << "]\t\tposition: [" << l_pos.str() << "]" << std::endl;
    }


    return EXIT_SUCCESS;

}
