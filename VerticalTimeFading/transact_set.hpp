#pragma once

#include <cstdint>

#include <vector>

#include "fade_set.hpp"

namespace vert {

class transact_set {
private:
	std::vector< int > m_transacts;

public:
	transact_set();
	transact_set( const std::vector < int > &transacts );
	void append( int set );
	double sum( const vert::fade_set &fade ) const;
	double sum() const;
	std::size_t size() const;
	int operator[]( std::size_t index ) const;
	std::string pretty() const;

};

transact_set operator&( const transact_set &rhs, const transact_set &lhs );
double operator*( const transact_set &rhs, const fade_set &lhs );
double operator*( const fade_set &lhs, const transact_set &rhs );

}