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

/** interface file for the distance class, that must create an abstract class
 * without any methods and properties, because the C++ class are pure virtual
 * and the methods are only called in the implementated JNI functions
 **/


#ifdef SWIGJAVA
%module "distancemodule"
%include "../swig/java/java.i"

%typemap(javaclassmodifiers) machinelearning::distances::distance<double>                                         "public interface"
%typemap(javabody)           machinelearning::distances::distance<double>                                         ""
%typemap(javafinalize)       machinelearning::distances::distance<double>                                         ""
%typemap(javadestruct)       machinelearning::distances::distance<double>                                         ""

%typemap(javaout)            ublas::vector<double> machinelearning::distances::distance<double>::getNormalize     ";"
%typemap(javaout)            ublas::matrix<double> machinelearning::distances::distance<double>::getNormalize     ";"
%typemap(javaout)            double machinelearning::distances::distance<double>::getLength                       ";"
%typemap(javaout)            ublas::vector<double> machinelearning::distances::distance<double>::getLength        ";"
%typemap(javaout)            double machinelearning::distances::distance<double>::getInvert                       ";"
%typemap(javaout)            ublas::vector<double> machinelearning::distances::distance<double>::getAbs           ";"
%typemap(javaout)            double machinelearning::distances::distance<double>::getDistance                     ";"
%typemap(javaout)            ublas::vector<double> machinelearning::distances::distance<double>::getDistance      ";"
#endif


%nodefaultctor               machinelearning::distances::distance<double>;
%nodefaultdtor               machinelearning::distances::distance<double>;


%include "distance.hpp"
%template(Distance) machinelearning::distances::distance<double>;