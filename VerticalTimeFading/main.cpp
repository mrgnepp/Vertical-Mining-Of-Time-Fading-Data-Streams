#include <cstdlib>
#include <cstdint>

#include <iostream>
#include <string>

#include "bit_vector.hpp"
#include "fade_vector.hpp"

void print_state( const vert::bit_vector &bits, const vert::fade_vector &fades );

int32_t main( int32_t argc, char *argv[] ) {
	const double fadeFactor = 0.5;

	vert::bit_vector bits;
	vert::fade_vector fades;

	bits.append( true );
	bits.append( false );

	fades.append( fadeFactor, 2 );

	print_state( bits, fades );

	bits.append( false );
	bits.append( true );

	fades.append( fadeFactor, 2 );

	print_state( bits, fades );

	bits.append( true );
	bits.append( true );
	bits.append( false );

	fades.append( fadeFactor, 3 );

	print_state( bits, fades );

	std::cout << std::endl << "End of Processing" << std::endl << "Press Enter to Exit" << std::endl;
	std::cin.get();
	return EXIT_SUCCESS;
}

void print_state( const vert::bit_vector &bits, const vert::fade_vector &fades ) {
	std::cout << "Bit Vector: " << bits.pretty() << ", Fade Vector: " << fades.pretty() << ", Support: " << bits * fades << std::endl;
}