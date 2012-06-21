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


import java.util.Random;


/** java testprogram for using svd algorithms **/
public class svd {
    
    
    /** main method
     * @param p_args input arguments
     **/
    public static void main(String[] p_args)
    {
        // generates random datapoints
        Random l_rand = new Random();
        
        Double[][] l_data = new Double[4][8];
        for(int i=0; i < l_data.length; i++) {
            for (int j=0; j < l_data[i].length; j++) {
                l_data[i][j] = l_rand.nextDouble() * 500;
                System.out.print(l_data[i][j] + "\t");
            }
            System.out.println("");
        }
        

        // create SVD
        machinelearning.util.SVD<Double> l_svd = machinelearning.util.Math.svd(l_data);
        
        if (l_svd != null) {
            
            System.out.println("\nsvd values:");
            
            Double[] l_vals = l_svd.getValues();
            for(int i=0; i < l_vals.length; i++)
                System.out.print(l_vals[i] + "\t");
            System.out.println("");
            l_vals = null;
            
            
            
            System.out.println("\nsvd vectors 1:\n");
            
            Double[][] l_vecs1 = l_svd.getVectors1();
            for(int j=0; j < l_vecs1.length; j++) {
                System.out.print( (j+1) + " svd vector:\t");
                for(int i=0; i < l_vecs1[j].length; i++)
                    System.out.print(l_vecs1[j][i] + "\t");
                System.out.println("\n");
            }
            l_vecs1 = null;
            
            
            
            System.out.println("\nsvd vectors 2:\n");
            
            Double[][] l_vecs2 = l_svd.getVectors2();
            for(int j=0; j < l_vecs2.length; j++) {
                System.out.print( (j+1) + " svd vector:\t");
                for(int i=0; i < l_vecs2[j].length; i++)
                    System.out.print(l_vecs2[j][i] + "\t");
                System.out.println("\n");
            }
            l_vecs2 = null;
        }
        
        l_svd          = null;
        l_data         = null;
        l_rand         = null;
    }
    
}