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
	std::string pretty( const fade_vector &fades ) const;
	bool operator<( const item_set &rhs ) const;
};

item_set operator&( const item_set &lhs, const item_set &rhs );

std::vector< item_set > do_viper( const std::map< uint32_t, item_set > &items, const vert::fade_vector &fades, double minsup );

} }