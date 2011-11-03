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


/** java testprogram for using eigenvalue algorithms **/
public class eigen {
    
    
    /** main method
     * @param p_args input arguments
     **/
    public static void main(String[] p_args)
    {
        // generates random datapoints
        Random l_rand = new Random();
        
        Double[][] l_data = new Double[6][6];
        for(int i=0; i < l_data.length; i++) {
            for (int j=0; j < l_data[i].length; j++) {
                l_data[i][j] = l_rand.nextDouble() * 500;
                System.out.print(l_data[i][j] + "\t");
            }
            System.out.println("");
        }
        
     
        // create eigeninformation
        machinelearning.util.Eigen<Double> l_eigen = machinelearning.util.Math.eigen(l_data);
        
        if (l_eigen != null) {
            
            System.out.println("\neigenvalues:");
            
            Double[] l_vals = l_eigen.getValues();
            for(int i=0; i < l_vals.length; i++)
                System.out.print(l_vals[i] + "\t");
            System.out.println("");
            l_vals = null;
            
            
            
            System.out.println("\neigenvectors:\n");
            
            Double[][] l_vecs = l_eigen.getVectors();
            for(int j=0; j < l_vecs.length; j++) {
                System.out.print( (j+1) + " eigenvector:\t");
                for(int i=0; i < l_vecs[j].length; i++)
                    System.out.print(l_vecs[j][i] + "\t");
                System.out.println("\n");
            }
            l_vecs = null;
            
            
            // get the largest eigenvector with perron-frobenius
            Double[] l_perron = machinelearning.util.Math.perronfrobenius( l_data, l_data.length );
            
            System.out.println("\nlargest eigenvector with perron-frobenius-theorem:\n");
            for(int i=0; i < l_perron.length; i++)
                System.out.print(l_perron[i] + "\t");
            System.out.println("");
            l_perron = null;
        }
        
        l_eigen        = null;
        l_data         = null;
        l_rand         = null;
    }
    
}