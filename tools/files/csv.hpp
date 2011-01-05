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

#ifdef MACHINELEARNING_FILES

#ifndef MACHINELEARNING_TOOLS_FILES_CSV_HPP
#define MACHINELEARNING_TOOLS_FILES_CSV_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/algorithm/string.hpp> 
#include <boost/lexical_cast.hpp>

#include "../../exception/exception.h"


namespace machinelearning { namespace tools { namespace files {
    
    namespace ublas  = boost::numeric::ublas;
    
    
    
    /** class for reading vector and matrix structur from csv file
     * $LastChangedDate$
     **/
    class csv {
        
        public :
            
            template<typename T> ublas::vector<T> readBlasVector( const std::string& ) const;
            template<typename T> ublas::matrix<T> readBlasMatrix( const std::string&, const std::string& = ",; " ) const;
            template<typename T> std::vector<T> readVector( const std::string&, const std::size_t& = 100 ) const;
            template<typename T> void write( const std::string&, const ublas::vector<T>& ) const;
            template<typename T> void write( const std::string&, const ublas::matrix<T>&, const char& = ' ' ) const;
            template<typename T> void write( const std::string&, const std::vector<T>& ) const;
        
    };
    
     
        
        
    /** read a vector structure from csv file 
     * @param p_file filename as string
     * @return ublas vector with data
     **/
    template<typename T> inline ublas::vector<T> csv::readBlasVector( const std::string& p_file ) const
    {
        
        std::string l_line;
        std::ifstream l_stream( p_file.c_str(), std::ifstream::in ); 
        
        l_stream.seekg( std::ios_base::beg ); 
        
        //first line => length of vector
        std::getline(l_stream, l_line);
        std::size_t l_vecsize = boost::lexical_cast<std::size_t>(l_line);
        if (l_vecsize == 0)
            throw exception::runtime(_("vector dimension must be greater than zero"));
        ublas::vector<T> l_vec( l_vecsize );        
        
        // read other lines
        for(std::size_t i=0; (i < l_vec.size()) && (std::getline(l_stream, l_line)); ++i)
            l_vec(i) = boost::lexical_cast<T>(l_line);
        
        l_stream.close();
        
        return l_vec;
    }
    
    
    /** read a matrix structure from csv file 
     * @param p_file filename as string
     * @param p_separator characters for sperator (default , ; blank)
     * @return ublas vector with data
     **/
    template<typename T> inline ublas::matrix<T> csv::readBlasMatrix( const std::string& p_file, const std::string& p_separator ) const
    {
        // open stream and read first line with matrix dimensions (rows cols)
        std::string l_line;
        std::ifstream l_stream( p_file.c_str(), std::ifstream::in ); 
        l_stream.seekg( std::ios_base::beg );        
        std::getline(l_stream, l_line);
        
        
        // seperate dimensions and creates matrix
        std::vector<std::string> l_data;
        boost::split( l_data, l_line, boost::is_any_of(p_separator) );
        
        if (l_data.size() == 0)
            throw exception::runtime(_("can not separate size"));

        std::size_t l_row = 0;
        if (l_data.size() > 0)
            l_row = boost::lexical_cast<std::size_t>( l_data[0] );
        
        std::size_t l_col = l_row;
        if (l_data.size() > 1)
            l_col = boost::lexical_cast<std::size_t>( l_data[1] );  
        
        if (l_col > 0)
            throw exception::runtime(_("column size must be greater than zero"));
        if (l_row > 0)
            throw exception::runtime(_("row size must be greater than zero"));
        
        ublas::matrix<T> l_mat( l_row, l_col ); 
        
        
        // read lines and seperate elements
        l_data.reserve(l_col);
        for(std::size_t i=0; (i < l_mat.size1()) && (std::getline(l_stream, l_line)); ++i) {
            l_data.erase(l_data.begin(), l_data.end());
            boost::split( l_data, l_line, boost::is_any_of(p_separator) );
            
            for(std::size_t j=0; (j < l_mat.size2()) && (j < l_data.size()); ++j)
                l_mat(i,j) =  boost::lexical_cast<T>( l_data[j] );
        }
        l_stream.close();
        
        return l_mat;
    }
    
    
    /** read all lines from csv file to a std::vector
     * @param p_file filename as string
     * @param p_vecreserve reserve capacity for std::vector (default 100)
     * @return std::vector
     **/    
    template<typename T> inline std::vector<T> csv::readVector( const std::string& p_file, const std::size_t& p_vecreserve ) const
    {
        if (p_vecreserve == 0)
            throw exception::runtime(_("vector reserve must be greater than zero"));
        
        std::vector<std::string> l_vec();
        l_vec.reserve(p_vecreserve);
        
        std::string l_line;
        std::ifstream l_stream( p_file.c_str(), std::ifstream::in | std::ifstream::binary ); 
        
        l_stream.seekg( std::ios_base::beg );
        while (std::getline(l_stream, l_line))
            l_vec.push_back( boost::lexical_cast<T>(l_line) );
        
        return l_vec;
    }
    
    
    /** write a blas vector to file
     * @param p_file filename
     * @param p_vec blas vector
     **/
    template<typename T> inline void csv::write( const std::string& p_file, const ublas::vector<T>& p_vec ) const
    {
        if (p_vec.size() == 0)
            return;
        
        std::fstream l_stream;
        l_stream.open( p_file.c_str(), std::ios::out);
        
        l_stream << p_vec.size() << "\n";
        for(std::size_t i=0; i < p_vec.size(); ++i) 
            l_stream << p_vec(i) << "\n";
        
        l_stream.close();
        
    }
    
    
    /** write a blas matrix to file
     * @param p_file filename
     * @param p_mat blas matrix
     * @param p_separator separator 
     **/    
    template<typename T> inline void csv::write( const std::string& p_file, const ublas::matrix<T>& p_mat, const char& p_separator ) const
    {
        if ( (p_mat.size1() == 0) || (p_mat.size2() == 0) )
            return;
        
        std::fstream l_stream;
        l_stream.open( p_file.c_str(), std::ios::out);
        
        l_stream << p_mat.size1() << p_separator << p_mat.size2() << "\n";
        for(std::size_t i=0; i < p_mat.size1(); ++i) {
            for(std::size_t j=0; j < p_mat.size2(); ++j) 
                l_stream << p_mat(i, j) << p_separator;
            l_stream << "\n";
        }
        
        l_stream.close();
        
    }    
    
    
    /** write a std vector to file
     * @param p_file filename
     * @param p_vec std vector
     **/
    template<typename T> inline void csv::write( const std::string& p_file, const std::vector<T>& p_vec ) const
    {
        if (p_vec.size() == 0)
            return;
        
        std::fstream l_stream;
        l_stream.open( p_file.c_str(), std::ios::out);
        
        l_stream << p_vec.size() << "\n";
        for(std::size_t i=0; i < p_vec.size(); ++i) 
            l_stream << p_vec[i] << "\n";
        
        l_stream.close();
    }

    
};};};


#endif
#endif