#include "viper.hpp"

#include <sstream>
#include <algorithm>
#include <set>

using namespace vert::viper;

item_set::item_set( std::string & name, vert::bit_vector & bits ) : m_name( name ), m_bits( bits ) {}

double vert::viper::item_set::support( const fade_vector &fades ) const {
	return m_bits * fades;
}

std::string vert::viper::item_set::pretty( const fade_vector &fades ) const {
	std::stringstream s;
	s << '{';

	for( auto it = m_name.begin(); it != m_name.end(); ++it ) {
		s << *it;
		if( it != m_name.end() - 1 ) {
			s << ", ";
		}
	}
	s << "}:" << support( fades );

	return s.str();
}

bool vert::viper::item_set::operator<( const item_set &rhs ) const {
	return m_name < rhs.m_name;
}

item_set vert::viper::operator&( const item_set &lhs, const item_set &rhs ) {
	std::string unordered = lhs.m_name + rhs.m_name;
	std::sort( unordered.begin(), unordered.end() );

	std::stringstream newName;
	char last = '\0';

	for( auto it = unordered.begin(); it != unordered.end(); ++it ) {
		if( *it != last ) {
			newName << *it;
		}

		last = *it;
	}

	return item_set( newName.str(), lhs.m_bits & rhs.m_bits );
}

std::vector< item_set > vert::viper::do_viper( const std::vector< item_set > &items, const vert::fade_vector &fades, double minsup ) {
	std::vector< item_set > results;
	std::vector< item_set > currentItems;

	for( auto it = items.begin(); it != items.end(); ++it ) {
		if( it->m_bits * fades >= minsup ) {
			currentItems.push_back( *it );
			results.push_back( *it );
		}
	}

	std::size_t currentSetSize = 1;

	while( currentItems.size() > 0 ) {
		std::set< item_set > candidates;

		for( std::size_t i = 0; i < currentItems.size(); ++i ) {
			for( std::size_t j = i + 1; j < currentItems.size(); ++j ) {
				item_set candidate = currentItems[i] & currentItems[j];
				if( candidate.m_name.size() == currentSetSize + 1 && candidate.support( fades ) >= minsup ) {
					candidates.insert( candidate );
				}
			}
		}

		++currentSetSize;
		currentItems.clear();
		for( auto it = candidates.begin(); it != candidates.end(); ++it ) {
			results.push_back( *it );
			currentItems.push_back( *it );
		}
	}

	return results;
}

