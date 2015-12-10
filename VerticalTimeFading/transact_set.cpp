#include "transact_set.hpp"

#include <algorithm>
#include <sstream>

using namespace vert;

transact_set::transact_set() {}
transact_set::transact_set( const std::vector< int32_t > &transacts ) : m_transacts( transacts ) {}

void transact_set::append( int32_t transact ) {
	m_transacts.push_back( transact );
}

double transact_set::sum( const vert::fade_set &fade ) const {
	double result = 0.0;
	std::size_t index = 0;

	for( auto it = m_transacts.begin(); it != m_transacts.end(); ++it, ++index ) {
		double transact = 1.0;

		if( index < fade.size() ) {
			transact *= fade[index];
		}

		result += transact;
	}

	return result;
}

double transact_set::sum() const {
	return sum( std::vector< double >() );
}

std::size_t transact_set::size() const {
	return m_transacts.size();
}

bool transact_set::find( int32_t value ) const {
	return std::find( m_transacts.begin(), m_transacts.end(), value ) != m_transacts.end();
}

int32_t transact_set::operator[]( std::size_t index ) const {
	return m_transacts[index];
}

transact_set vert::operator&( const transact_set &rhs, const transact_set &lhs ) {
	transact_set result;

	// If the tID is in both, append to results
	for( std::size_t i = 0; i < rhs.size(); ++i ) {
		for( std::size_t j = 0; j < lhs.size(); ++j ) {
			if( rhs[j] == lhs[i] ) {
				result.append( rhs[i] );
			}
		}
	}

	return result;
}

double vert::operator*( const transact_set &lhs, const fade_set &rhs ) {
	return lhs.sum( rhs );
}

double vert::operator*( const fade_set &lhs, const transact_set &rhs ) {
	return rhs.sum( lhs );
}

std::string transact_set::pretty() const {
	std::stringstream s;
	s << '[';

	for( auto it = m_transacts.begin(); it != m_transacts.end(); ++it ) {
		s << *it;
	}

	s << ']';

	return s.str();
}