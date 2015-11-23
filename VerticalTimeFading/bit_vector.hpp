#pragma once

#include <cstdint>

#include <vector>

namespace vert {
	
class bit_vector {
private:
	std::vector< bool > m_bits;

public:
	bit_vector();
	bit_vector( const std::vector< bool > &bits );
	void append( bool bit );
	double sum( const std::vector< double > &fade ) const;
	double sum() const;
	std::size_t size() const;
	bool operator[]( std::size_t index ) const;
	
};

bit_vector operator&( const bit_vector &rhs, const bit_vector &lhs );

}