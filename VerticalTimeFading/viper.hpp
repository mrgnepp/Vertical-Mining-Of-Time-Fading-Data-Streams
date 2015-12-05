#pragma once

#include <vector>
#include <utility>
#include <map>

#include "bit_vector.hpp"

namespace vert { namespace viper {

struct item_set {
	std::vector< uint32_t > m_name;
	vert::bit_vector m_bits;

	item_set();
	item_set( std::vector< uint32_t > &name, vert::bit_vector &bits );
	double support( const fade_vector &fades ) const;
	std::string pretty( const fade_vector &fades, const std::map< uint32_t, uint32_t > &dataMap ) const;
	bool operator<( const item_set &rhs ) const;
};

struct prefix {
	struct extension {
		uint32_t m_ext;
		item_set *m_from;
	};

	struct result {
		item_set *m_first;
		item_set *m_second;
		std::vector< uint32_t > m_items;

		result( item_set *first, item_set *second, std::vector< uint32_t > &items );
	};

	std::string m_key;
	std::vector< uint32_t > m_items;
	std::vector< extension > m_extensions;

	prefix( item_set *items );
	prefix() {}
	std::vector< result > generate_potential_candidates() const;

	bool operator<( const prefix &rhs ) const;
};

item_set operator&( const item_set &lhs, const item_set &rhs );

std::vector< item_set > do_viper( const std::vector< item_set > &items, const vert::fade_vector &fades, double minsup );

} }