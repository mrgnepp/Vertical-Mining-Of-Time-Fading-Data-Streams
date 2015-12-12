#pragma once

#include <cstdint>

#include <vector>

#include "fade_set.hpp"

namespace vert {

class transact_set {
private:
	std::vector< int32_t > m_transacts;

public:
	transact_set();
	transact_set( const std::vector < int > &transacts );
	void append( int32_t set );
	double sum( const vert::fade_set &fade ) const;
	double sum() const;
	std::size_t size() const;
	bool find( size_t value ) const;
	int operator[]( std::size_t index ) const;
	std::string pretty() const;

};

transact_set operator&( const transact_set &rhs, const transact_set &lhs );
double operator*( const transact_set &rhs, const fade_set &lhs );
double operator*( const fade_set &lhs, const transact_set &rhs );

}