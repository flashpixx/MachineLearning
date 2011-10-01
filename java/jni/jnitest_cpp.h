#ifndef JNITEST_CPP_H
#define JNITEST_CPP_H

#include <iostream>

class jnitest
{
    public :
    
		jnitest( const int& p ) : m_number(p) { std::cout << "cpp ctor" << std::endl; }
		~jnitest( void ) { std::cout << "cpp dtor" << std::endl; }
		
		int getNumber( void ) const { std::cout << "cpp getNumber" << std::endl; return m_number; }
		void setNumber( const int& p ) { m_number = p; std::cout << "cpp setNumber" << std::endl;}
        
    private :
    
        int m_number;
};

#endif