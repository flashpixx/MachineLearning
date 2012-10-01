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


import machinelearning.tools.*;
import java.util.*;


/** java testprogram for using svd algorithms **/
public class svd {
    
    
    /** main method
     * @param p_args input arguments
     **/
    public static void main(String[] p_args)
    {
        // generates random datapoints
        java.util.Random l_rand = new java.util.Random();
        
        Double[][] l_data = new Double[4][8];
        for(int i=0; i < l_data.length; i++) {
            for (int j=0; j < l_data[i].length; j++) {
                l_data[i][j] = l_rand.nextDouble() * 500;
                System.out.print(l_data[i][j] + "\t");
            }
            System.out.println("");
        }
        

        // create SVD
        ArrayList<Double> l_svdvals   = new ArrayList<Double>();
        ArrayList<Double[]> l_svdvecs1 = new ArrayList<Double[]>();
        ArrayList<Double[]> l_svdvecs2 = new ArrayList<Double[]>();
        
        Lapack.svd(l_data, l_svdvals, l_svdvecs1, l_svdvecs2);
        

        
        System.out.println("\nsvd values:");
        for(int i=0; i < l_svdvals.size(); i++)
            System.out.print(l_svdvals.get(i) + "\t");
        System.out.println("");
        l_svdvals = null;
        
        
        
        System.out.println("\nsvd vectors 1:\n");
        for(int j=0; j < l_svdvecs1.size(); j++) {
            System.out.print( (j+1) + " svd vector:\t");
            for(int i=0; i < l_svdvecs1.get(j).length; i++)
                System.out.print(l_svdvecs1.get(j)[i] + "\t");
            System.out.println("\n");
        }
        l_svdvecs1 = null;
        
        
        
        System.out.println("\nsvd vectors 2:\n");
        for(int j=0; j < l_svdvecs2.size(); j++) {
            System.out.print( (j+1) + " svd vector:\t");
            for(int i=0; i < l_svdvecs2.get(j).length; i++)
                System.out.print(l_svdvecs2.get(j)[i] + "\t");
            System.out.println("\n");
        }
        l_svdvecs2 = null;

        
        l_data         = null;
        l_rand         = null;
    }
    
}