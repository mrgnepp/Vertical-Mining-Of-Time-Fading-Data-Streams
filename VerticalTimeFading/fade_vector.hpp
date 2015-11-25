#pragma once

#include <vector>

namespace vert {

class fade_vector {
private:
	std::vector< double > m_fades;

public:
	fade_vector();
	fade_vector( std::vector< double > &fades );
	std::size_t size() const;
	double operator[]( std::size_t index ) const;
	void append( double value, std::size_t times );
	std::string pretty() const;

};

}