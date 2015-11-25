#include <cstdlib>
#include <cstdint>

#include <iostream>
#include <string>

#include "bit_vector.hpp"
#include "fade_vector.hpp"
#include "viper.hpp"


void print_state( const vert::bit_vector &bits, const vert::fade_vector &fades );

int32_t main( int32_t argc, char *argv[] ) {
	const double fadeFactor = 0.5;
	const double minsup = 3.0;

	std::vector< bool > aV = { true, false, true, true, true, false };
	std::vector< bool > bV = { true, true, true, true, true, true };
	std::vector< bool > cV = { false, true, false, true, true, true };
	std::vector< bool > dV = { true, false, true, false, true, true };
	std::vector< bool > eV = { true, true, true, true, true, false };

	std::vector< vert::item_set > itemSets = {
		vert::item_set( std::string( "a" ), vert::bit_vector( aV ) ),
		vert::item_set( std::string( "b" ), vert::bit_vector( bV ) ),
		vert::item_set( std::string( "c" ), vert::bit_vector( cV ) ),
		vert::item_set( std::string( "d" ), vert::bit_vector( dV ) ),
		vert::item_set( std::string( "e" ), vert::bit_vector( eV ) ),
	};

	vert::fade_vector fades;

	fades.append( fadeFactor, 6 );

	std::vector< vert::item_set > frequentItemSets = vert::viper( itemSets, fades, minsup );

	for( auto it = frequentItemSets.begin(); it != frequentItemSets.end(); ++it ) {
		std::cout << it->pretty( fades ) << std::endl;
	}

	std::cout << std::endl;

	for( auto it = itemSets.begin(); it != itemSets.end(); ++it ) {
		it->m_bits.append( true );
		it->m_bits.append( false );
	}

	fades.append( fadeFactor, 2 );

	frequentItemSets = vert::viper( itemSets, fades, minsup );

	for( auto it = frequentItemSets.begin(); it != frequentItemSets.end(); ++it ) {
		std::cout << it->pretty( fades ) << std::endl;
	}

	std::cout << std::endl << "End of Processing" << std::endl << "Press Enter to Exit" << std::endl;
	std::cin.get();
	return EXIT_SUCCESS;
}

void print_state( const vert::bit_vector &bits, const vert::fade_vector &fades ) {
	std::cout << "Bit Vector: " << bits.pretty() << ", Fade Vector: " << fades.pretty() << ", Support: " << bits * fades << std::endl;
}