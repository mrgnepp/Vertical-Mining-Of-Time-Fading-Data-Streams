#include "bit_vector.hpp"

#include <algorithm>

using namespace vert;

bit_vector::bit_vector() {}
bit_vector::bit_vector( const std::vector< bool > &bits ) : m_bits( bits ) {}

void bit_vector::append( bool bit ) {
	m_bits.push_back( bit );
}

double bit_vector::sum( const std::vector< double > &fade ) const {
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
		result.append( rhs.size() < i && rhs[i] && lhs.size() < i && lhs[i] );
	}

	return result;
}