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
        machinelearning.util.Random l_random = new machinelearning.util.Random();
        
        for(int i=0; i < l_num; i++) {

            if (p_args[0].toLowerCase().equals("uniform"))
                System.out.println( l_random.getDouble(machinelearning.util.Random.Distribution.UNIFORM) );
            
            if (p_args[0].toLowerCase().equals("bernoulli"))
                System.out.println( l_random.getDouble(machinelearning.util.Random.Distribution.BERNOULLI) );
        
            if (p_args[0].toLowerCase().equals("cauchy"))
                System.out.println( l_random.getDouble(machinelearning.util.Random.Distribution.CAUCHY) );
            
            if (p_args[0].toLowerCase().equals("gamma"))
                System.out.println( l_random.getDouble(machinelearning.util.Random.Distribution.GAMMA) );
            
            if (p_args[0].toLowerCase().equals("poisson"))
                System.out.println( l_random.getDouble(machinelearning.util.Random.Distribution.POISSON) );
            
            if (p_args[0].toLowerCase().equals("exponential"))
                System.out.println( l_random.getDouble(machinelearning.util.Random.Distribution.EXPONENTIAL) );
            
            if (p_args[0].toLowerCase().equals("normal"))
                System.out.println( l_random.getDouble(machinelearning.util.Random.Distribution.NORMAL) );
            
            if (p_args[0].toLowerCase().equals("student"))
                System.out.println( l_random.getDouble(machinelearning.util.Random.Distribution.STUDENT) );
            
            if (p_args[0].toLowerCase().equals("weibull"))
                System.out.println( l_random.getDouble(machinelearning.util.Random.Distribution.WEIBULL) );
            
            if (p_args[0].toLowerCase().equals("rayleigh"))
                System.out.println( l_random.getDouble(machinelearning.util.Random.Distribution.RAYLEIGH) );
            
            if (p_args[0].toLowerCase().equals("chisquared"))
                System.out.println( l_random.getDouble(machinelearning.util.Random.Distribution.CHISQUARED) );
            
            if (p_args[0].toLowerCase().equals("pareto"))
                System.out.println( l_random.getDouble(machinelearning.util.Random.Distribution.PARETO) );
            
            if (p_args[0].toLowerCase().equals("triangular"))
                System.out.println( l_random.getDouble(machinelearning.util.Random.Distribution.TRIANGULAR) );
            
            if (p_args[0].toLowerCase().equals("beta"))
                System.out.println( l_random.getDouble(machinelearning.util.Random.Distribution.BETA) );
        }

        
        l_random.dispose();
        l_random = null;
	}
	
}