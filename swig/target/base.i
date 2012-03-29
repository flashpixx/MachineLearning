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


// main typemaps for "return types" with code converting
%typemap(out, optimal=1, noblock=1) ublas::matrix<double>, ublas::matrix<double>& {
    $result = swig::convert::getArrayFromMatrix(jenv, $1);
}

%typemap(out, optimal=1, noblock=1) ublas::symmetric_matrix<double, ublas::upper>, ublas::symmetric_matrix<double, ublas::upper>& {
    $result = swig::convert::getArrayFromMatrix(jenv, static_cast< ublas::matrix<double> >($1));
}


%typemap(out, optimal=1, noblock=1) std::size_t, std::size_t& {
    $result = $1;
}



// main typemaps for "parameter types" with code converting
%typemap(in, optimal=1) ublas::matrix<double>&, ublas::symmetric_matrix<T, ublas::upper>& {
    ublas::matrix<double> l_arg = swig::convert::getDoubleMatrixFrom2DArray(jenv, $input);
    $1                          = &l_arg;
}

// the return parameter must be copy into a local variable (l_param)
// because Swig uses pointers to reference the parameter
%typemap(in, optimal=1, noblock=1) std::string, std::string& (std::string l_param) {
    l_param = swig::convert::getString(jenv, $input);
    $1      = &l_param;
}

%typemap(in, optimal=1, noblock=1) std::vector<std::string>, std::vector<std::string>& (std::vector<std::string> l_param) {
    l_param = swig::convert::getStringVectorFromArray(jenv, $input);
    $1      = &l_param;
}

%typemap(in, optimal=1, noblock=1) std::size_t, std::size_t& {
    $1 = ($1_ltype)& $input;
}
