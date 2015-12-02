#pragma once

#include <cstdint>

#include <vector>

#include "fade_vector.hpp"

namespace vert {
	
class bit_vector {
private:
	std::vector< bool > m_bits;

public:
	bit_vector();
	bit_vector( const std::vector< bool > &bits );
	void append( bool bit );
	void activate_last();
	double sum( const vert::fade_vector &fade ) const;
	double sum() const;
	std::size_t size() const;
	bool operator[]( std::size_t index ) const;
	std::string pretty() const;
	
};

bit_vector operator&( const bit_vector &rhs, const bit_vector &lhs );
double operator*( const bit_vector &rhs, const fade_vector &lhs );
double operator*( const fade_vector &lhs, const bit_vector &rhs );

}