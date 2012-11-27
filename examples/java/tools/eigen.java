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


/** java testprogram for using eigenvalue algorithms **/
public class eigen {
    
    
    /** main method
     * @param p_args input arguments
     **/
    public static void main(String[] p_args)
    {
        // generates random datapoints
        java.util.Random l_rand = new java.util.Random();
        
        Double[][] l_data = new Double[6][6];
        for(int i=0; i < l_data.length; i++) {
            for (int j=0; j < l_data[i].length; j++) {
                l_data[i][j] = l_rand.nextDouble() * 500;
                System.out.print(l_data[i][j] + "\t");
            }
            System.out.println("");
        }
        
     
        // create eigenvectors / -values
        ArrayList<Double> l_eigenvals   = new ArrayList<Double>();
        ArrayList<Double[]> l_eigenvecs = new ArrayList<Double[]>();
        
        Lapack.eigen(l_data, l_eigenvals, l_eigenvecs);
            
        
        
        System.out.println("\neigenvalues:");
        for(int i=0; i < l_eigenvals.size(); i++)
            System.out.print(l_eigenvals.get(i) + "\t");
        System.out.println("");
        l_eigenvals = null;
        
        
        
        System.out.println("\neigenvectors:\n");
        for(int j=0; j < l_eigenvecs.size(); j++) {
            System.out.print( (j+1) + " eigenvector:\t");
            for(int i=0; i < l_eigenvecs.get(j).length; i++)
                System.out.print(l_eigenvecs.get(j)[i] + "\t");
            System.out.println("\n");
        }
        l_eigenvecs = null;
        
            
        // get the largest eigenvector with perron-frobenius
        Double[] l_perron = Lapack.perronFrobenius( l_data, 2*l_data.length );
            
        System.out.println("\nlargest eigenvector with perron-frobenius-theorem:");
        for(int i=0; i < l_perron.length; i++)
            System.out.print(l_perron[i] + "\t");
        System.out.println("");
        l_perron = null;
        
        l_data   = null;
        l_rand   = null;
    }
    
}