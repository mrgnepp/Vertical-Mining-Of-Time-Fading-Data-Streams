#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <map>

#include "apriori.hpp"

using namespace apriori;

void apriori::do_apriori(std::string filename, double minsup, double fade_factor, int batch_size) {
	std::map< std::set< int >, double, cmp_by_cardinality > total_freq_itemsets;
	std::cout << "Adding new transactions..." << std::endl;

	//Load transactions
	auto transactions = apriori::load_transactions(filename);
	std::vector< std::vector< transaction > > batches = apriori::split_into_batches(transactions, batch_size);

	std::cout << "Begin Apriori..." << std::endl;

	int pass_level = 1;
	auto item_counts = apriori::count_singletons(batches, fade_factor);
	auto freq_singletons = apriori::get_frequent_itemsets(total_freq_itemsets, item_counts, minsup);

	for (auto it : freq_singletons) {
		total_freq_itemsets[it.first] += it.second;
	}

	pass_level++;

	auto candidate_itemsets = apriori::generate_candidate_itemsets(freq_singletons, pass_level);

	while (!candidate_itemsets.empty()) {
		candidate_itemsets = apriori::count_itemsets(batches, candidate_itemsets, fade_factor);
		auto level_freq_itemsets = apriori::get_frequent_itemsets(total_freq_itemsets, candidate_itemsets, minsup);

		for (auto it : level_freq_itemsets) {
			total_freq_itemsets[it.first] += it.second;
		}

		pass_level++;

		candidate_itemsets = apriori::generate_candidate_itemsets(level_freq_itemsets, pass_level);
		candidate_itemsets = apriori::prune_candidates(level_freq_itemsets, candidate_itemsets);
	}

	std::cout << "Frequent itemsets after adding transactions 1 to " << transactions.size() << std::endl;
	apriori::print_frequent_itemsets(total_freq_itemsets);
	std::cout << std::endl;
}

std::map< std::set< int >, double, cmp_by_cardinality > apriori::get_frequent_itemsets( std::map< std::set< int >, double, 
	cmp_by_cardinality > current_frequents, std::map< std::set< int >, double, cmp_by_cardinality > candidates, double minsup ) {
	std::map< std::set< int >, double, cmp_by_cardinality > result;

	for ( auto it : candidates ) {
		if ( current_frequents[it.first] + it.second >= minsup ) {
			result[it.first] = it.second;
		}
	}

	return result;
}

std::map< std::set< int >, double, cmp_by_cardinality > apriori::count_singletons( std::vector< std::vector< transaction > > batches, double fade_factor ) {
	std::map< std::set< int >, double, cmp_by_cardinality > item_counts;

	for (int i = 0; i < batches.size(); i++) {
		auto transactions = batches[i];
		auto applied_fade_factor = std::pow(fade_factor, batches.size() - (i + 1));

		for (auto trans : transactions) {
			for (auto jt : trans.itemset) {
				std::set< int > singleton;
				singleton.insert(jt);
				item_counts[singleton] += applied_fade_factor;
			}
		}
	}
	return item_counts;
}

std::map< std::set< int >, double, cmp_by_cardinality > apriori::prune_candidates( std::map< std::set< int >, double, cmp_by_cardinality > freq_subsets, 
	std::map< std::set< int >, double, cmp_by_cardinality > candidates ) {
	auto result = candidates;
	
	for ( auto it: candidates ) {
		bool all_subsets_frequent = true;
		auto itemset = it.first;

		for ( auto jt = itemset.begin(); jt != itemset.end() && all_subsets_frequent; jt++ ) {
			std::set< int > subset = itemset;
			subset.erase( *jt );

			if ( freq_subsets.find( subset ) == freq_subsets.end() ) {
				result.erase( itemset );
				all_subsets_frequent = false;
			}
		}
	}

	return result;
}

std::map< std::set< int >, double, cmp_by_cardinality > apriori::count_itemsets( std::vector< std::vector< apriori::transaction > > batches, 
	std::map< std::set< int >, double, cmp_by_cardinality > candidate_itemsets, double fade_factor ) {
	for (int i = 0; i < batches.size(); i++) {
		auto transactions = batches[i];
		auto applied_fade_factor = std::pow(fade_factor, batches.size() - (i + 1));
	
		for (auto it : transactions) {
			auto t_itemset = it.itemset;

			for (auto jt : candidate_itemsets) {
				auto candidate_set = jt.first;
				std::set<int> intersection;
				std::set_intersection(t_itemset.begin(), t_itemset.end(), candidate_set.begin(), candidate_set.end(), std::inserter(intersection, intersection.begin()));

				if (intersection == candidate_set) {
					candidate_itemsets[candidate_set] += applied_fade_factor;
				}
			}
		}
	}
	


	return candidate_itemsets;
}

void apriori::print_frequent_itemsets( std::map< std::set< int >, double, cmp_by_cardinality > itemsets ) {
	std::cout << "Printing all frequent itemsets..." << std::endl;

	for ( auto it : itemsets ) {
		std::set< int > s = it.first;
		std::cout << apriori::get_itemset_string( it.first ) << ":" << it.second << std::endl;
	}
}

std::string apriori::get_itemset_string( std::set< int > itemset ) {
	std::string result = "{";
	for ( auto it: itemset ) {
		result += std::to_string( it ) + ", ";
	}

	return result.substr( 0, result.length() - 2 ) + "}";
}

std::set< int > apriori::get_transaction_itemset( std::string transaction_items ) {
	std::stringstream ss( transaction_items );
	std::string buff;
	std::set< int > t_itemset;
	int num_items = stoi( buff );

	for ( int i = 0; i < num_items; i++ ) {
		ss >> buff;
		t_itemset.insert( std::stoi( buff ) );
	}

	return t_itemset;
}

std::map< std::set< int >, double, cmp_by_cardinality > apriori::generate_candidate_itemsets( std::map< std::set< int >, double, cmp_by_cardinality > freq_items, int level ) {
	std::map< std::set< int >, double, cmp_by_cardinality > candidates;
	
	for ( auto it = freq_items.begin(); it != freq_items.end(); it++ ) {
		auto candidate_itemset = it->first;
		std::vector< int > candidate_list( candidate_itemset.begin(), candidate_itemset.end() );

		for ( auto jt = std::next( it ); jt != freq_items.end(); jt++) {
			auto to_union = jt->first;
			std::vector< int > to_union_list(to_union.begin(), to_union.end() );

			if ( level == 2 ) {
				to_union.insert( candidate_itemset.begin(), candidate_itemset.end() );
				candidates[to_union] = 0;
			}

			else if ( std::equal( candidate_list.begin(), candidate_list.begin() + (level - 2), to_union.begin() ) ) {
				to_union.insert( candidate_itemset.begin(), candidate_itemset.end() );
				candidates[to_union] = 0;
			}
		}
	}

	return candidates;
}

std::vector< apriori::transaction > apriori::load_transactions( std::string filename ) {
	std::vector< apriori::transaction > transactions;
	std::ifstream dataFile;
	dataFile.open( filename );
	std::string dataLengthString;

	std::getline( dataFile, dataLengthString );
	while ( std::getline( dataFile, dataLengthString ) && dataLengthString != std::string( "" ) ) {
		auto trans = apriori::get_transaction( dataLengthString );
		transactions.push_back( trans );
	}

	dataFile.close();
	return transactions;
}
 
apriori::transaction apriori::get_transaction( std::string transaction_str ) {
	std::stringstream ss( transaction_str );
	std::string buff;
	apriori::transaction trans;

	ss >> trans.transaction_id;
	ss >> trans.num_items;

	std::set< int > trans_itemset;

	for ( int i = 0; i < trans.num_items; i++ ) {
		ss >> buff;
		std::cout << buff << std::endl;
		trans_itemset.insert( std::stoi( buff ) );
	}

	trans.itemset = trans_itemset;
	return trans;
}

std::vector< std::vector< transaction > > apriori::split_into_batches( std::vector< transaction > transactions, int batch_size ) {
	std::vector< std::vector< transaction > > batches;
	int num_batches = ( int )std::ceil( double( transactions.size() ) / batch_size );
	
	for ( int i = 0; i < transactions.size(); i += batch_size ) {
		auto start = transactions.begin() + i;
		auto end = transactions.begin() + std::min( ( i + batch_size ), ( int )transactions.size() );
		std::vector< transaction > batch( start, end );
		batches.push_back( batch );
	}

	return batches;
}