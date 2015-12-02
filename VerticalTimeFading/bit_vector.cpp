#include "bit_vector.hpp"

#include <algorithm>
#include <sstream>

using namespace vert;

bit_vector::bit_vector() {}
bit_vector::bit_vector( const std::vector< bool > &bits ) : m_bits( bits ) {}

void bit_vector::append( bool bit ) {
	m_bits.push_back( bit );
}

void bit_vector::activate_last() {
	m_bits[m_bits.size() - 1] = true;
}

double bit_vector::sum( const vert::fade_vector &fade ) const {
	double result = 0.0;
	std::size_t index = 0;

	for( auto it = m_bits.begin(); it != m_bits.end(); ++it, ++index ) {
		double bit = 0.0;
		if( *it ) {
			bit = 1.0;
		}

		if( index < fade.size() ) {
			bit *= fade[index];
		}

		result += bit;
	}
	
	return result;
}

double bit_vector::sum() const {
	return sum( std::vector< double >() );
}

std::size_t bit_vector::size() const {
	return m_bits.size();
}

bool bit_vector::operator[]( std::size_t index ) const {
	return m_bits[index];
}

bit_vector vert::operator&( const bit_vector &rhs, const bit_vector &lhs ) {
	bit_vector result;
	std::size_t maxSize = std::max( rhs.size(), lhs.size() );
	for( std::size_t i = 0; i < maxSize; ++i ) {
		result.append( i < rhs.size() && rhs[i] && i < lhs.size() && lhs[i] );
	}

	return result;
}

double vert::operator*( const bit_vector &lhs, const fade_vector &rhs ) {
	return lhs.sum( rhs );
}

double vert::operator*( const fade_vector &lhs, const bit_vector &rhs ) {
	return rhs.sum( lhs );
}

std::string bit_vector::pretty() const {
	std::stringstream s;
	s << '[';

	for( auto it = m_bits.begin(); it != m_bits.end(); ++it ) {
		if( *it ) {
			s << '1';
		} else {
			s << '0';
		}
	}

	s << ']';

	return s.str();
}