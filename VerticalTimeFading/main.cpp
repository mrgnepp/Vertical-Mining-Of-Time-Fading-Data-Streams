#include <cstdlib>
#include <cstdint>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>

#include "bit_vector.hpp"
#include "fade_vector.hpp"
#include "viper.hpp"

//Print a bit_vector, a fade_vector, and that bit_vector summed against the fade vector
void print_state( const vert::bit_vector &bits, const vert::fade_vector &fades );

int32_t main( int32_t argc, char *argv[] ) {
	//These should be self explanatory
	const double fadeFactor = 1.0;
	const double minsupPercent = 0.75;
	const std::size_t chunkSize = 5;

	std::ifstream dataFile;
	dataFile.open( "1k5l.txt" );

	std::string dataLengthString;
	std::getline( dataFile, dataLengthString );
	const std::size_t numLines = 5;// std::stoll( dataLengthString );

	std::vector< vert::viper::item_set > itemSets;
	vert::fade_vector fades;

	std::cout << "Time Fading VIPER" << std::endl;
	std::cout << "Time Fade Factor: " << fadeFactor << std::endl;
	std::cout << "Minsup: " << minsupPercent * 100.0 << "%" << std::endl;
	std::cout << "Transaction Block Size: " << chunkSize << std::endl << std::endl;

	for( std::size_t i = 1; i < numLines; i += chunkSize ) {
		std::size_t numTaken = 0;
		std::string dataString;

		std::cout << "Adding new transactions..." << std::endl;

		while( numTaken < chunkSize && std::getline( dataFile, dataString ) && dataString != std::string( "" ) ) {
			std::size_t transaction = i + numTaken;
			std::string buffer;
			std::map< uint32_t, bool > transactionData;
			std::stringstream ss( dataString );

			//Get rid of the transaction number
			ss >> buffer;
			ss >> buffer;

			while( ss >> buffer ) {
				uint32_t item = std::stoi( buffer );
				transactionData[item] = false;
			}

			for( auto it = itemSets.begin(); it != itemSets.end(); ++it ) {
				auto bv = transactionData.find( it->m_name[0] );
				it->m_bits.append( bv == transactionData.end() );
				if( bv != transactionData.end() ) {
					bv->second = true;
				}
			}

			for( auto it = transactionData.begin(); it != transactionData.end(); ++it ) {
				if( ! it->second ) {
					std::vector< bool > v;
					for( std::size_t j = 0; j < transaction; ++j ) {
						v.push_back( false );
					}

					v.push_back( true );
					std::vector< uint32_t > name = { it->first };

					itemSets.push_back( vert::viper::item_set( name, vert::bit_vector( v ) ) );
				}
			}

			++numTaken;
		}

		fades.append( fadeFactor, numTaken );

		
		const double minsup = static_cast< double >( ( i + numTaken ) - 1 ) * minsupPercent;

		std::cout << "Begin VIPER round, minsup = " << minsup << "..." << std::endl;
		std::vector< vert::viper::item_set > result = vert::viper::do_viper( itemSets, fades, minsup );

		std::cout << "Frequent itemsets after addign transactions " << i << " to " << i + chunkSize << ":" << std::endl;

		for( auto it = result.begin(); it != result.end(); ++it ) {
			std::cout << it->pretty( fades ) << std::endl;
		}

		std::cout << std::endl;
		
	}

	dataFile.close();

	/*std::cout << "Time fade factor: " << fadeFactor << std::endl;
	std::cout << "Minimum support threshold: " << minsup << std::endl << std::endl;

	//Data used to initialize the bit_vectors for each 1-itemset
	std::vector< bool > aV = { true, false, true, true, true, false };
	std::vector< bool > bV = { true, true, true, true, true, true };
	std::vector< bool > cV = { false, true, false, true, true, true };
	std::vector< bool > dV = { true, false, true, false, true, true };
	std::vector< bool > eV = { true, true, true, true, true, false };

	//Create the bit_vectors and wrap them in VIPER item_sets
	std::vector< vert::viper::item_set > itemSets = {
		vert::viper::item_set( std::string( "a" ), vert::bit_vector( aV ) ),
		vert::viper::item_set( std::string( "b" ), vert::bit_vector( bV ) ),
		vert::viper::item_set( std::string( "c" ), vert::bit_vector( cV ) ),
		vert::viper::item_set( std::string( "d" ), vert::bit_vector( dV ) ),
		vert::viper::item_set( std::string( "e" ), vert::bit_vector( eV ) ),
	};

	//This vector keeps track of the time fade factors for each transaction
	//Due to the nature of VIPER, we can't simply accumulate them in the bit_vector
	//Note that there is only ONE fade vector for ALL bit_vectors, NOT one for each bit_vector
	vert::fade_vector fades;

	//This call multiplies all existing values in the fade_vector by fadeFactor, and THEN appends 6 1s to the end
	//Because the fade_vector was previously empty, it will just append 6 1s
	fades.append( fadeFactor, 6 );

	//Actually run the VIPER algorithm...
	std::vector< vert::viper::item_set > frequentItemSets = vert::viper::do_viper( itemSets, fades, minsup );

	//...and print out the frequent itemsets it found
	std::cout << "Frequent itemsets with only the initial round of transactions:" << std::endl;
	for( auto it = frequentItemSets.begin(); it != frequentItemSets.end(); ++it ) {
		std::cout << it->pretty( fades ) << std::endl;
	}

	std::cout << std::endl;

	//Now to test the time fade aspect, we'll add a new transaction which contains every item
	//This means a 1 will be appended to each bit_vector
	for( auto it = itemSets.begin(); it != itemSets.end(); ++it ) {
		it->m_bits.append( true );
	}

	//Now we'll update the fade_vector, this will multiply the 6 1s that were previously in the vector by fadeFactor ( 0.5 )
	//And THEN append a single 1 to the end
	fades.append( fadeFactor, 1 );

	//Run VIPER again for the updated bit_vectors and fade_vector
	//Note that we do NOT re-use the frequent itemsets from the initial run in anyway
	frequentItemSets = vert::viper::do_viper( itemSets, fades, minsup );

	//...and print out the resulting frequent itemsets
	std::cout << "Frequent itemsets after time fade and additional transaction:" << std::endl;
	for( auto it = frequentItemSets.begin(); it != frequentItemSets.end(); ++it ) {
		std::cout << it->pretty( fades ) << std::endl;
	}*/

	std::cout << std::endl << "End of Processing" << std::endl << "Press Enter to Exit" << std::endl;
	std::cin.get();
	return EXIT_SUCCESS;
}

void print_state( const vert::bit_vector &bits, const vert::fade_vector &fades ) {
	std::cout << "Bit Vector: " << bits.pretty() << ", Fade Vector: " << fades.pretty() << ", Support: " << bits * fades << std::endl;
}