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

/** interface file for the supervived clustering class, that should be
 * an java interface, because we can use the multiple inheritance and
 * disable all body parts
 **/


#ifdef SWIGJAVA
%module "supervicedclusteringmodule"
%include "../../swig/java/java.i"

%typemap(javaclassmodifiers) machinelearning::clustering::supervised::clustering<double, std::string>                                                               "public interface"
%typemap(javabody)           machinelearning::clustering::supervised::clustering<double, std::string>                                                               ""
%typemap(javafinalize)       machinelearning::clustering::supervised::clustering<double, std::string>                                                               ""
%typemap(javadestruct)       machinelearning::clustering::supervised::clustering<double, std::string>                                                               ""

%typemap(javaout)            void machinelearning::clustering::supervised::clustering<double, std::string>::train                                                   ";"
%typemap(javaout)            ublas::matrix<double> machinelearning::clustering::supervised::clustering<double, std::string>::getPrototypes                          ";"
%typemap(javaout)            std::vector<std::string> machinelearning::clustering::supervised::clustering<double, std::string>::getPrototypesLabel                  ";"
%typemap(javaout)            void machinelearning::clustering::supervised::clustering<double, std::string>::setLogging                                              ";"
%typemap(javaout)            bool machinelearning::clustering::supervised::clustering<double, std::string>::getLogging                                              ";"
%typemap(javaout)            std::vector< ublas::matrix<double> > machinelearning::clustering::supervised::clustering<double, std::string>::getLoggedPrototypes     ";"
%typemap(javaout)            std::size_t machinelearning::clustering::supervised::clustering<double, std::string>::getPrototypeSize                                 ";"
%typemap(javaout)            std::size_t machinelearning::clustering::supervised::clustering<double, std::string>::getPrototypeCount                                ";"
%typemap(javaout)            std::vector<double> machinelearning::clustering::supervised::clustering<double, std::string>::getLoggedQuantizationError               ";"
%typemap(javaout)            ublas::indirect_array<> machinelearning::clustering::supervised::clustering<double, std::string>::use                                  ";"


%typemap(javaclassmodifiers) machinelearning::clustering::supervised::clustering<double, std::size_t>                                                               "public interface"
%typemap(javabody)           machinelearning::clustering::supervised::clustering<double, std::size_t>                                                               ""
%typemap(javafinalize)       machinelearning::clustering::supervised::clustering<double, std::size_t>                                                               ""
%typemap(javadestruct)       machinelearning::clustering::supervised::clustering<double, std::size_t>                                                               ""

%typemap(javaout)            void machinelearning::clustering::supervised::clustering<double, std::size_t>::train                                                   ";"
%typemap(javaout)            ublas::matrix<double> machinelearning::clustering::supervised::clustering<double, std::size_t>::getPrototypes                          ";"
%typemap(javaout)            std::vector<std::size_t> machinelearning::clustering::supervised::clustering<double, std::size_t>::getPrototypesLabel                  ";"
%typemap(javaout)            void machinelearning::clustering::supervised::clustering<double, std::size_t>::setLogging                                              ";"
%typemap(javaout)            bool machinelearning::clustering::supervised::clustering<double, std::size_t>::getLogging                                              ";"
%typemap(javaout)            std::vector< ublas::matrix<double> > machinelearning::clustering::supervised::clustering<double, std::size_t>::getLoggedPrototypes     ";"
%typemap(javaout)            std::size_t machinelearning::clustering::supervised::clustering<double, std::size_t>::getPrototypeSize                                 ";"
%typemap(javaout)            std::size_t machinelearning::clustering::supervised::clustering<double, std::size_t>::getPrototypeCount                                ";"
%typemap(javaout)            std::vector<double> machinelearning::clustering::supervised::clustering<double, std::size_t>::getLoggedQuantizationError               ";"
%typemap(javaout)            ublas::indirect_array<> machinelearning::clustering::supervised::clustering<double, std::size_t>::use                                  ";"
#endif


%nodefaultctor              machinelearning::clustering::supervised::clustering<double, std::string>;
%nodefaultdtor              machinelearning::clustering::supervised::clustering<double, std::string>;

%nodefaultctor              machinelearning::clustering::supervised::clustering<double, std::size_t>;
%nodefaultdtor              machinelearning::clustering::supervised::clustering<double, std::size_t>;


%include "clustering.hpp"
%template(ClusteringString) machinelearning::clustering::supervised::clustering<double, std::string>;
%template(ClusteringLong) machinelearning::clustering::supervised::clustering<double, std::size_t>;