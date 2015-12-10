#include "fade_set.hpp"

#include <sstream>

using namespace vert;

fade_set::fade_set() {}

fade_set::fade_set( std::vector< double > &fades ) : m_fades( fades ) {}

std::size_t fade_set::size() const {
	return m_fades.size();
}

double fade_set::operator[]( std::size_t index ) const {
	return m_fades[index];
}

void fade_set::append( double value, std::size_t times ) {
	for( auto it = m_fades.begin(); it != m_fades.end(); ++it ) {
		*it *= value;
	}

	for( std::size_t i = 0; i < times; ++i ) {
		m_fades.push_back( 1 );
	}
}

std::string fade_set::pretty() const {
	std::stringstream s;
	s << '(';

	for( auto it = m_fades.begin(); it != m_fades.end(); ++it ) {
		s << *it;

		if( it != m_fades.end() - 1 ) {
			s << ", ";
		}
	}

	s << ')';

	return s.str();
}