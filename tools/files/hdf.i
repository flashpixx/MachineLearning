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

/** interface file for the HDF calls **/

#ifdef MACHINELEARNING_FILES_HDF

#ifdef SWIGJAVA
%module "hdfmodule"
%include "../../swig/java/java.i"
%rename(HDF) hdf;
#endif


%include "hdf.hpp"

%template(readBlasMatrix) machinelearning::tools::files::hdf::readBlasMatrix<double>;
%template(readBlasVector) machinelearning::tools::files::hdf::readBlasVector<double>;
%template(readValue) machinelearning::tools::files::hdf::readValue<double>;

%template(writeBlasMatrix) machinelearning::tools::files::hdf::writeBlasMatrix<double>;
%template(writeBlasVector) machinelearning::tools::files::hdf::writeBlasVector<double>;
%template(writeValue) machinelearning::tools::files::hdf::writeValue<double>;
#endif
