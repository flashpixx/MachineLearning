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


import machinelearning.dimensionreduce.nonsupervised.*;
import java.util.Random;


/** java testprogram for using MDS **/
public class mds {
    
    
    /** main method
     * @param p_args input arguments
     **/
    public static void main(String[] p_args)
    {
        // generates random datapoints
        Random l_rand = new Random();
        
        Double[][] l_data = new Double[8][8];
        for(int i=0; i < l_data.length; i++) {
            for (int j=0; j < l_data[i].length; j++) {
                if (i==j)
                    l_data[i][j] = new Double(0);
                else
                    l_data[i][j] = l_rand.nextDouble();
                System.out.print(l_data[i][j] + "\t");
            }
            System.out.println("");
        }
        
        // create MDS object
        MDS l_mds = new MDS(3, MDS.project.metric);
        
        
        // maps the random data points
        Double[][] l_result = l_mds.map(l_data);
        System.out.println("\nproject data:");
        if (l_result == null)
            System.out.println("no data is returned");
        else
            for(int i=0; i < l_result.length; i++) {
                for(int j=0; j < l_result[i].length; ++j)
                    System.out.print(l_result[i][j] + "\t");
                System.out.println("");
            }
        
        
        // delete manually the PCA object (call destructor), should be
        // do the finalizer, but there is no guarantee.
        l_mds.delete();
        
        // set objects to null for gc removing
        l_mds    = null;
        l_result = null;
        l_data   = null;  
        l_rand   = null;
    }
    
}