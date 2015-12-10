#pragma once

#include <vector>

namespace vert {

class fade_set {
private:
	std::vector< double > m_fades;

public:
	fade_set();
	fade_set( std::vector< double > &fades );
	std::size_t size() const;
	double operator[]( std::size_t index ) const;
	void append( double value, std::size_t times );
	std::string pretty() const;

};

}