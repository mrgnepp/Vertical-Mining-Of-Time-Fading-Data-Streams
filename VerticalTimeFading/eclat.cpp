#include "eclat.hpp"

#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <iostream>

using namespace vert::eclat;

item_set::item_set( std::vector< uint32_t > & name, vert::transact_set & tranacts ) : m_name( name ), m_transacts( tranacts ) {}
item_set::item_set() {};

double vert::eclat::item_set::support( const fade_set &fades) const {
	return m_transacts * fades;
}

std::string vert::eclat::item_set::pretty( const fade_set &fades, const std::map< uint32_t, uint32_t > &dataMap ) const {
	std::stringstream s;
	s << '{';

	for( auto it = m_name.begin(); it != m_name.end(); ++it ) {
		s << dataMap.find( *it )->second;
		if( it != m_name.end() - 1 ) {
			s << ", ";
		}
	}
	s << "}:" << support( fades );

	return s.str();
}

bool vert::eclat::item_set::operator<( const item_set &rhs ) const {
	return m_name < rhs.m_name;
}

item_set vert::eclat::operator&( const item_set &lhs, const item_set &rhs ) {
	std::vector< uint32_t > unordered = lhs.m_name;
	unordered.insert( unordered.end(), rhs.m_name.begin(), rhs.m_name.end() );
	std::sort( unordered.begin(), unordered.end() );

	std::vector< uint32_t > newName;
	uint32_t last = std::numeric_limits<uint32_t>::max();

	for( auto it = unordered.begin(); it != unordered.end(); ++it ) {
		if( *it != last ) {
			newName.push_back( *it );
		}

		last = *it;
	}

	return item_set( newName, lhs.m_transacts & rhs.m_transacts );
}

vert::eclat::prefix::prefix( item_set *items ) {
	m_items = items->m_name;
	m_items.pop_back();
	std::stringstream ss;
	bool started = false;
	for( auto it = m_items.begin(); it != m_items.end(); ++it ) {
		if( started ) {
			ss << ' ';
		} else {
			started = true;
		}

		ss << *it;
	}

	extension ext;
	ext.m_from = items;
	ext.m_ext = items->m_name.back();

	m_extensions.push_back( ext );

	m_key = ss.str();
}

std::vector< prefix::result > vert::eclat::prefix::generate_potential_candidates() const {
	std::vector< prefix::result > result;

	for( std::size_t i = 0; i < m_extensions.size(); ++i ) {
		for( std::size_t j = i + 1; j < m_extensions.size(); ++j ) {
			std::vector< uint32_t > temp = m_items;
			temp.push_back( m_extensions[i].m_ext );
			temp.push_back( m_extensions[j].m_ext );
			result.push_back( prefix::result( m_extensions[i].m_from, m_extensions[j].m_from, temp ) );
		}
	}

	return result;
}

bool vert::eclat::prefix::operator<( const prefix &rhs ) const {
	return m_key < rhs.m_key;
}

vert::eclat::prefix::result::result( item_set *first, item_set *second, std::vector< uint32_t > &items ) : m_first( first ), m_second( second ), m_items( items ) {}

std::vector< item_set > vert::eclat::do_eclat( const std::vector< item_set > &items, const vert::fade_set &fades, double minsup ) {
	std::vector< item_set > results;
	std::vector< item_set > currentItems;

	//First Pass
	for( auto it = items.begin(); it != items.end(); ++it ) {
		if( it->m_transacts * fades >= minsup ) {
			results.push_back( *it );
		}
	}

	std::vector< std::vector< double > > counts;
	const std::size_t numItems = items.size();

	//Build up the count array
	for( std::size_t i = 0; i < numItems; ++i ) {
		counts.push_back( std::vector< double >() );
		for( std::size_t j = 0; j < numItems; ++j ) {
			counts[i].push_back( 0.0 );
		}
	}

	//Second Pass
	const std::size_t numTransactions = items[0].m_transacts.size();
	for( std::size_t t = 0; t < numTransactions; ++t ) {
		std::vector< uint32_t > transaction;
		uint32_t dataIndex = 0;
		for( auto it = items.begin(); it != items.end(); ++it ) {
			if( it->m_transacts[t] ) {
				transaction.push_back( dataIndex );
			}
			++dataIndex;
		}

		for( std::size_t i = 0; i < transaction.size(); ++i ) {
			for( std::size_t j = i + 1; j < transaction.size(); ++j ) {
				counts[transaction[i]][transaction[j]] = counts[transaction[i]][transaction[j]] + fades[t];
			}
		}
	}

	//Build up the candidates for going into the 3rd pass
	for( std::size_t i = 0; i < numItems; ++i ) {
		for( std::size_t j = 0; j < numItems - i; ++j ) {
			if( counts[i][j] >= minsup ) {
				item_set temp = items[i] & items[j];
				currentItems.push_back( temp );
				results.push_back( temp );
			}
		}
	}

	//Subsequent passes
	std::size_t currentLevel = 2;
	while( currentItems.size() > 0 ) {
		//FORC (Candidate Generation)
		//This is an STL hash table
		std::unordered_map< std::string, prefix > prefixMap;
		//We also need to keep a list of all of the prefixes, as you can't iterate through a hash table
		std::vector< prefix * > prefixList;

		//Build up the hash table and list of prefixes
		for( auto it = currentItems.begin(); it != currentItems.end(); ++it ) {
			prefix pref( &( *it ) );
			auto res = prefixMap.find( pref.m_key );
			if( res != prefixMap.end() ) {
				res->second.m_extensions.push_back( pref.m_extensions[0] );
			} else {
				prefixMap[pref.m_key] = pref;
				prefixList.push_back( &prefixMap[pref.m_key] );
			}
		}

		std::vector< item_set > candidates;

		for( auto it = prefixList.begin(); it != prefixList.end(); ++it ) {
			std::vector< prefix::result > potentialCandidates = ( *it )->generate_potential_candidates();
			for( auto resIt = potentialCandidates.begin(); resIt != potentialCandidates.end(); ++resIt ) {
				item_set candidate = *resIt->m_first & *resIt->m_second;
				if( candidate.support( fades ) >= minsup ) {
					results.push_back( candidate );
					candidates.push_back( candidate );
				}
			}
		}

		currentItems = candidates;
	}

	return results;
}