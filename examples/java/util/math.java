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


import java.util.Random;


/** java testprogram for using math algorithms **/
public class math {
	
	
    /** main method
     * @param p_args input arguments
     **/
	public static void main(String [] p_args)
	{
        // generates random datapoints
		Random l_rand = new Random();
		
		Double[][] l_data = new Double[10][10];
		for(int i=0; i < l_data.length; i++) {
			for (int j=0; j < l_data[i].length; j++) {
				l_data[i][j] = l_rand.nextDouble() * 500;
				System.out.print(l_data[i][j] + "\t");
			}
			System.out.println("");
		}
        
        Double[] l_eigenvalues    = null;
        Double[][] l_eigenvectors = null;
        machinelearning.util.Math.eigen(l_data, l_eigenvalues, l_eigenvectors);
        
        if ( (l_eigenvalues == null) || (l_eigenvectors == null) )
            System.out.println("no data is returned");
        else {
            
        }
        
        l_data         = null;
        l_eigenvalues  = null;
        l_eigenvectors = null;
        l_rand         = null;
	}
	
}