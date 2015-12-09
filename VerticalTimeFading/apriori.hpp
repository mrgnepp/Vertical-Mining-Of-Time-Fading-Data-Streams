#pragma once

#include <set>
#include <vector>

namespace apriori {
struct transaction {
	int transaction_id;
	int num_items;
	std::set<int> itemset;
};

struct itemset_count {
	std::set<int> itemset;
	double support;
};

void do_apriori( std::string filename, double minsup, double fade_factor, int chunkSize );
std::map< std::set< int >, double > clean_frequent_itemsets( std::map< std::set< int >, double > candidates, double minsup );
std::map< std::set< int >, double > get_frequent_itemsets( std::map< std::set< int >, double > current_frequents, std::map< std::set< int >, double >, double );
std::map< std::set< int >, double > fade_itemsets( std::map< std::set< int >, double > freq_itemsets, double fade_factor );
std::map< std::set< int >, double > count_singletons( std::vector< transaction > transactions );
std::map< std::set< int >, double > prune_candidates( std::map< std::set< int >, double > freq_subsets, std::map< std::set< int >, double > candidates );
std::map< std::set< int >, double > count_itemsets( std::vector< apriori::transaction > transactions, std::map< std::set< int >, double > candidate_itemsets ) ;
void print_frequent_itemsets( std::map< std::set< int >, double > itemsets );
std::string get_itemset_string( std::set< int > );
std::set< int > get_transaction_itemset( std::string transaction );
std::map< std::set< int >, double > generate_candidate_itemsets( std::map< std::set< int >, double >, int level );
std::vector< apriori::transaction > load_transactions( std::string filename );
apriori::transaction get_transaction( std::string transaction_str );
std::vector< std::vector< transaction > > split_into_batches( std::vector<transaction > transactions, int batch_size );

}
