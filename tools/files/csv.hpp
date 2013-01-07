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

#ifdef MACHINELEARNING_FILES

#ifndef __MACHINELEARNING_TOOLS_FILES_CSV_HPP
#define __MACHINELEARNING_TOOLS_FILES_CSV_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/algorithm/string.hpp> 
#include <boost/lexical_cast.hpp>

#include "../language/language.h"
#include "../../errorhandling/exception.hpp"


namespace machinelearning { namespace tools { namespace files {
    
    #ifndef SWIG
    namespace ublas  = boost::numeric::ublas;
    #endif
    
    
    /** class for reading vector and matrix structur from csv file **/
    class csv
    {
        
        public :
            
            template<typename T> ublas::vector<T> readBlasVector( const std::string&, const bool& = false ) const;
            template<typename T> ublas::matrix<T> readBlasMatrix( const std::string&, const std::string& = ",; \t", const bool& = false ) const;
            std::vector<std::string> readVector( const std::string& ) const;
            template<typename T> void write( const std::string&, const ublas::vector<T>&, const bool& = false ) const;
            template<typename T> void write( const std::string&, const std::vector<T>&, const bool& = false ) const;
            template<typename T> void write( const std::string&, const ublas::matrix<T>&, const char& = ' ', const bool& = false ) const;
        
    };
    
     
        
        
    /** read a vector structure from csv file 
     * @param p_file filename as string
     * @param p_header bool so the first element is set to the length of the vector
     * @return ublas vector with data
     **/
    template<typename T> inline ublas::vector<T> csv::readBlasVector( const std::string& p_file, const bool& p_header ) const
    {
        std::ifstream l_stream( p_file.c_str(), std::ifstream::in ); 
        l_stream.seekg( std::ios_base::beg ); 
        
        std::vector<std::string> l_data;
        
        if (p_header) {
            std::string l_line;
            
            //first line => length of vector
            std::getline(l_stream, l_line);
            std::size_t l_vecsize = boost::lexical_cast<std::size_t>(l_line);
            if (l_vecsize == 0)
                throw exception::runtime(_("vector dimension must be greater than zero"), *this);
        
            for(std::size_t i=0; (i < l_vecsize) && (std::getline(l_stream, l_line)); ++i)
                l_data.push_back(l_line);
        } else {
            std::string l_line;
            while (std::getline(l_stream, l_line))
                l_data.push_back(l_line);
        }
        
        l_stream.close();
        
        
        // convert string data
        ublas::vector<T> l_vec( l_data.size() );        
        for(std::size_t i=0; i < l_data.size(); ++i)
            l_vec(i) = boost::lexical_cast<T>( l_data[i] );
        
        return l_vec;
    }
    
    
    /** read a matrix structure from csv file 
     * @param p_file filename as string
     * @param p_separator characters for sperator (default , ; \\t blank)
     * @param p_header first element in the input file is the size of the input matrix
     * @return ublas vector with data
     **/
    template<typename T> inline ublas::matrix<T> csv::readBlasMatrix( const std::string& p_file, const std::string& p_separator, const bool& p_header ) const
    {
        if (p_separator.empty())
            throw exception::runtime(_("separator can not be empty"), *this);

        std::ifstream l_stream( p_file.c_str(), std::ifstream::in ); 
        l_stream.seekg( std::ios_base::beg );        

        std::vector< std::vector<std::string> > l_data;
        std::size_t l_row = 0;
        std::size_t l_col = 0;
        
        if (p_header) {
            std::string l_line;   
            std::vector<std::string> l_splitline;
            
            std::getline(l_stream, l_line);
        
            // seperate dimensions
            boost::split( l_splitline, l_line, boost::is_any_of(p_separator) );
            if (l_splitline.size() < 2)
                throw exception::runtime(_("can not separate size"), *this);

            l_row = boost::lexical_cast<std::size_t>( l_splitline[0] );
            l_col = boost::lexical_cast<std::size_t>( l_splitline[1] );  
        
            while (std::getline(l_stream, l_line)) {
                l_splitline.clear();
                boost::split( l_splitline, l_line, boost::is_any_of(p_separator) );
                
                l_data.push_back(l_splitline);
            }
        } else {
            std::string l_line;
            std::vector<std::string> l_splitline;
            while (std::getline(l_stream, l_line)) {
                l_splitline.clear();
                boost::split( l_splitline, l_line, boost::is_any_of(p_separator) );
                
                l_data.push_back(l_splitline);
                l_col = std::max(l_col, l_splitline.size());
            }
            l_row = l_data.size();
        }
        l_stream.close();
                    
        
        if (l_col == 0)
            throw exception::runtime(_("column size must be greater than zero"), *this);
        if (l_row == 0)
            throw exception::runtime(_("row size must be greater than zero"), *this);

        
        ublas::matrix<T> l_mat( l_row, l_col ); 
        for(std::size_t i=0; l_mat.size1(); ++i)             
            for(std::size_t j=0; (j < l_mat.size2()) && (j < l_data[i].size()); ++j)
                l_mat(i,j) =  boost::lexical_cast<T>( l_data[i][j] );
        
        return l_mat;
    }
    
    
    /** read all lines from csv file to a std::vector
     * @param p_file filename as string
     * @return std::vector
     **/    
    inline std::vector<std::string> csv::readVector( const std::string& p_file ) const
    {
        std::vector<std::string> l_vec;

        std::ifstream l_stream( p_file.c_str(), std::ifstream::in | std::ifstream::binary ); 
        l_stream.seekg( std::ios_base::beg );
        
        std::string l_line;
        while (std::getline(l_stream, l_line))
            l_vec.push_back( l_line );
        
        return l_vec;
    }
    
    
    /** write a blas vector to file
     * @param p_file filename
     * @param p_header first element in the output file is the size of the input vector
     * @param p_vec blas vector
     **/
    template<typename T> inline void csv::write( const std::string& p_file, const ublas::vector<T>& p_vec, const bool& p_header ) const
    {
        if (p_vec.size() == 0)
            return;
        
        std::fstream l_stream;
        l_stream.open( p_file.c_str(), std::ios::out);
        
        if (p_header)
            l_stream << p_vec.size() << "\n";
        
        for(std::size_t i=0; i < p_vec.size(); ++i) 
            l_stream << p_vec(i) << "\n";
        
        l_stream.close();
        
    }
    
    
    /** write a blas matrix to file
     * @param p_file filename
     * @param p_mat blas matrix
     * @param p_separator separator 
     * @param p_header first element in the output file is the size of the input matrix
     **/    
    template<typename T> inline void csv::write( const std::string& p_file, const ublas::matrix<T>& p_mat, const char& p_separator, const bool& p_header ) const
    {
        if ( (p_mat.size1() == 0) || (p_mat.size2() == 0) )
            return;
        
        std::fstream l_stream;
        l_stream.open( p_file.c_str(), std::ios::out);
        
        if (p_header)
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
     * @param p_header first element in the output file is the size of the input vector
     **/
    template<typename T> inline void csv::write( const std::string& p_file, const std::vector<T>& p_vec, const bool& p_header ) const
    {
        if (p_vec.size() == 0)
            return;
        
        std::fstream l_stream;
        l_stream.open( p_file.c_str(), std::ios::out);
        
        if (p_header)
            l_stream << p_vec.size() << "\n";
        
        for(std::size_t i=0; i < p_vec.size(); ++i) 
            l_stream << p_vec[i] << "\n";
        
        l_stream.close();
    }

    
}}}
#endif
#endif
