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

/** interface file for the HDF calls,
 * we use always double types, so for writing we
 * set the HDF datatype by a fixed value
 **/

#ifdef MACHINELEARNING_FILES_HDF

#ifdef SWIGJAVA
%module "hdfmodule"
%include "../../swig/java/java.i"
%rename(HDF) hdf;

%exception %{
    try {
        $action
    }
    catch (const std::exception& e) { SWIG_JavaThrowException(jenv, SWIG_JavaRuntimeException, e.what() ); }
    catch( const H5::Exception& e) { SWIG_JavaThrowException(jenv, SWIG_JavaRuntimeException, e.getCDetailMsg() ); }
    catch (...) { SWIG_JavaThrowException(jenv, SWIG_JavaRuntimeException,  "exception in machinelearning framework"); }
%}
#endif




%include "hdf.hpp"

%template(readBlasMatrix) machinelearning::tools::files::hdf::readBlasMatrix<double>;
%template(readBlasVector) machinelearning::tools::files::hdf::readBlasVector<double>;
%template(readValue) machinelearning::tools::files::hdf::readValue<double>;

%extend machinelearning::tools::files::hdf {
    void writeValue( const std::string& p_path, const double& p_val ) { $self->writeValue<double>(p_path, p_val, tools::files::hdf::NATIVE_DOUBLE); }
    void writeBlasVector( const std::string& p_path, const ublas::vector<double>& p_vector ) { $self->writeBlasVector<double>(p_path, p_vector, tools::files::hdf::NATIVE_DOUBLE); }
    void writeBlasMatrix( const std::string& p_path, const ublas::matrix<double>& p_matrix ) { $self->writeBlasMatrix<double>(p_path, p_matrix, tools::files::hdf::NATIVE_DOUBLE); }
}

#endif
