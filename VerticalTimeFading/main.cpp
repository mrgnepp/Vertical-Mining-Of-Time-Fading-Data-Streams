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
	const double fadeFactor = 1;
	const double minsup = 3;
	const std::size_t chunkSize = 6;

	std::ifstream dataFile;
	dataFile.open( "class.txt" );

	std::string dataLengthString;
	std::getline( dataFile, dataLengthString );
	const std::size_t numLines = std::stoll( dataLengthString );

	std::vector< vert::viper::item_set > itemSets;
	std::map< uint32_t, uint32_t > dataMap;
	std::map< uint32_t, uint32_t > reverseDataMap;
	uint32_t nextIndex = 0;
	vert::fade_vector fades;

	std::cout << "Time Fading VIPER" << std::endl;
	std::cout << "Time Fade Factor: " << fadeFactor << std::endl;
	std::cout << "Minsup: " << minsup << std::endl;
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

			//Get rid of the transaction number and count
			ss >> buffer;
			//Uncomment this if the sample data includes the number of items in a transaction before the transaction.
			ss >> buffer;

			while( ss >> buffer ) {
				uint32_t item = std::stoi( buffer );
				auto found = reverseDataMap.find( item );
				if( found == reverseDataMap.end() ) {
					dataMap[nextIndex] = item;
					reverseDataMap[item] = nextIndex;
					transactionData[nextIndex] = false;
					++nextIndex;
				} else {
					transactionData[found->second] = false;
				}

				
			}

			for( auto it = itemSets.begin(); it != itemSets.end(); ++it ) {
				auto bv = transactionData.find( it->m_name[0] );
				it->m_bits.append( bv != transactionData.end() );
				if( bv != transactionData.end() ) {
					bv->second = true;
				}
			}

			for( auto it = transactionData.begin(); it != transactionData.end(); ++it ) {
				if( ! it->second ) {
					std::vector< bool > v;
					for( std::size_t j = 0; j < transaction - 1; ++j ) {
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

		std::cout << "Begin VIPER round..." << std::endl;
		std::vector< vert::viper::item_set > result = vert::viper::do_viper( itemSets, fades, minsup );

		std::cout << "Frequent itemsets after addign transactions " << i << " to " << i + chunkSize - 1 << ":" << std::endl;

		for( auto it = result.begin(); it != result.end(); ++it ) {
			std::cout << it->pretty( fades, dataMap ) << std::endl;
		}

		std::cout << std::endl;
		
	}

	dataFile.close();

	std::cout << std::endl << "End of Processing" << std::endl << "Press Enter to Exit" << std::endl;
	std::cin.get();
	return EXIT_SUCCESS;
}

void print_state( const vert::bit_vector &bits, const vert::fade_vector &fades ) {
	std::cout << "Bit Vector: " << bits.pretty() << ", Fade Vector: " << fades.pretty() << ", Support: " << bits * fades << std::endl;
}