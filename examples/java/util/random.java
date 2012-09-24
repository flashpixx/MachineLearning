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



/** java testprogram for random algorithms **/
public class random {
	
	
    /** main method
     * @param p_args input arguments
     **/
	public static void main(String[] p_args)
	{
        if (p_args.length < 2) {
            System.out.println("first parameter type of distribution (uniform, bernoulli, cauchy, gamma, poisson, exponential, normal, student, weibull, rayleigh, chisquared, pareto, triangular, beta)");
            System.out.println("second value number of elements");
            System.exit(0); 
        }
        
        int l_num = Integer.parseInt(p_args[1]);
        if (l_num < 1) {
            System.out.println("number of elements must be greater than zero");
            System.exit(-1);
        }
        
        
        // create random object
        machinelearning.tools.Random l_random = new machinelearning.tools.Random();
        
        for(int i=0; i < l_num; i++) {

            if (p_args[0].toLowerCase().equals("uniform"))
                System.out.println( l_random.get(machinelearning.tools.Random.distribution.uniform) );
            
            if (p_args[0].toLowerCase().equals("bernoulli"))
                System.out.println( l_random.get(machinelearning.tools.Random.distribution.bernoulli) );
        
            if (p_args[0].toLowerCase().equals("cauchy"))
                System.out.println( l_random.get(machinelearning.tools.Random.distribution.cauchy) );
            
            if (p_args[0].toLowerCase().equals("gamma"))
                System.out.println( l_random.get(machinelearning.tools.Random.distribution.gamma) );
            
            if (p_args[0].toLowerCase().equals("poisson"))
                System.out.println( l_random.get(machinelearning.tools.Random.distribution.poisson) );
            
            if (p_args[0].toLowerCase().equals("exponential"))
                System.out.println( l_random.get(machinelearning.tools.Random.distribution.exponential) );
            
            if (p_args[0].toLowerCase().equals("normal"))
                System.out.println( l_random.get(machinelearning.tools.Random.distribution.normal) );
            
            if (p_args[0].toLowerCase().equals("student"))
                System.out.println( l_random.get(machinelearning.tools.Random.distribution.student) );
            
            if (p_args[0].toLowerCase().equals("weibull"))
                System.out.println( l_random.get(machinelearning.tools.Random.distribution.weibull) );
            
            if (p_args[0].toLowerCase().equals("rayleigh"))
                System.out.println( l_random.get(machinelearning.tools.Random.distribution.rayleigh) );
            
            if (p_args[0].toLowerCase().equals("chisquared"))
                System.out.println( l_random.get(machinelearning.tools.Random.distribution.chisquared) );
            
            if (p_args[0].toLowerCase().equals("pareto"))
                System.out.println( l_random.get(machinelearning.tools.Random.distribution.pareto) );
            
            if (p_args[0].toLowerCase().equals("triangular"))
                System.out.println( l_random.get(machinelearning.tools.Random.distribution.triangular) );
            
            if (p_args[0].toLowerCase().equals("beta"))
                System.out.println( l_random.get(machinelearning.tools.Random.distribution.beta) );
        }

        
        l_random.delete();
        l_random = null;
	}
	
}