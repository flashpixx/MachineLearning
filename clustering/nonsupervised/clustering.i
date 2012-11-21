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

/** interface file for the nonsupervived clustering & patchclustering class,
 * that should be an java interface, because we can use the multiple inheritance and
 * disable all body parts
 **/


#ifdef SWIGJAVA
%module "nonsupervicedclusteringmodule"
%include "../../swig/java/java.i"

%typemap(javaclassmodifiers)    machinelearning::clustering::nonsupervised::clustering<double>                                                                          "public interface"
%typemap(javabody)              machinelearning::clustering::nonsupervised::clustering<double>                                                                          ""
%typemap(javafinalize)          machinelearning::clustering::nonsupervised::clustering<double>                                                                          ""
%typemap(javadestruct)          machinelearning::clustering::nonsupervised::clustering<double>                                                                          ""

%typemap(javaout)               void machinelearning::clustering::nonsupervised::clustering<double>::train                                                              ";"
%typemap(javaout)               ublas::matrix<double> machinelearning::clustering::nonsupervised::clustering<double>::getPrototypes                                     ";"
%typemap(javaout)               void machinelearning::clustering::nonsupervised::clustering<double>::setLogging                                                         ";"
%typemap(javaout)               bool machinelearning::clustering::nonsupervised::clustering<double>::getLogging                                                         ";"
%typemap(javaout)               std::vector< ublas::matrix<double> > machinelearning::clustering::nonsupervised::clustering<double>::getLoggedPrototypes                ";"
%typemap(javaout)               std::size_t machinelearning::clustering::nonsupervised::clustering<double>::getPrototypeSize                                            ";"
%typemap(javaout)               std::size_t machinelearning::clustering::nonsupervised::clustering<double>::getPrototypeCount                                           ";"
%typemap(javaout)               std::vector<double> machinelearning::clustering::nonsupervised::clustering<double>::getLoggedQuantizationError                          ";"
%typemap(javaout)               ublas::indirect_array<> machinelearning::clustering::nonsupervised::clustering<double>::use                                             ";"


%typemap(javaclassmodifiers)    machinelearning::clustering::nonsupervised::patchclustering<double>                                                                     "public interface"
%typemap(javabody)              machinelearning::clustering::nonsupervised::patchclustering<double>                                                                     ""
%typemap(javafinalize)          machinelearning::clustering::nonsupervised::patchclustering<double>                                                                     ""
%typemap(javadestruct)          machinelearning::clustering::nonsupervised::patchclustering<double>                                                                     ""

%typemap(javaout)               void machinelearning::clustering::nonsupervised::patchclustering<double>::trainpatch                                                    ";"
%typemap(javaout)               ublas::vector<double> machinelearning::clustering::nonsupervised::patchclustering<double>::getPrototypeWeights                          ";"
%typemap(javaout)               std::vector< ublas::vector<double> > machinelearning::clustering::nonsupervised::patchclustering<double>::getLoggedPrototypeWeights     ";"
#endif


%nodefaultctor                  machinelearning::clustering::nonsupervised::clustering<double>;
%nodefaultdtor                  machinelearning::clustering::nonsupervised::clustering<double>;

%nodefaultctor                  machinelearning::clustering::nonsupervised::patchclustering<double>;
%nodefaultdtor                  machinelearning::clustering::nonsupervised::patchclustering<double>;


%include "clustering.hpp"
%template(Clustering) machinelearning::clustering::nonsupervised::clustering<double>;
%template(PatchClustering) machinelearning::clustering::nonsupervised::patchclustering<double>;
