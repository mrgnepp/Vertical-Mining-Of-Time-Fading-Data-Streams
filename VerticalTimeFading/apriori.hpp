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

struct cmp_by_cardinality : std::binary_function< std::set<int>, std::set<int>, bool> {
	bool operator() (const std::set<int> a, const std::set<int> b) const {
		if (a.size() == b.size()) {
			return a < b;
		}

		else {
			return a.size() < b.size();
		}
	}
};

void do_apriori( std::string filename, double minsup, double fade_factor, int chunkSize );
std::map< std::set< int >, double, cmp_by_cardinality > get_frequent_itemsets( std::map< std::set< int >, double, cmp_by_cardinality > current_frequents, std::map< std::set< int >, double, cmp_by_cardinality >, double minsup);
std::map< std::set< int >, double, cmp_by_cardinality > count_singletons( std::vector< std::vector< transaction > > transactions, double fade_factor);
std::map< std::set< int >, double, cmp_by_cardinality > prune_candidates( std::map< std::set< int >, double, cmp_by_cardinality > freq_subsets, std::map< std::set< int >, double, cmp_by_cardinality > candidates );
std::map< std::set< int >, double, cmp_by_cardinality > count_itemsets( std::vector< std::vector< apriori::transaction > > transactions, std::map< std::set< int >, double, cmp_by_cardinality > candidate_itemsets, double fade_factor ) ;
void print_frequent_itemsets( std::map< std::set< int >, double, cmp_by_cardinality > itemsets );
std::string get_itemset_string( std::set< int > );
std::set< int > get_transaction_itemset( std::string transaction );
std::map< std::set< int >, double, cmp_by_cardinality > generate_candidate_itemsets( std::map< std::set< int >, double, cmp_by_cardinality >, int level );
std::vector< apriori::transaction > load_transactions( std::string filename );
apriori::transaction get_transaction( std::string transaction_str );
std::vector< std::vector< transaction > > split_into_batches( std::vector<transaction > transactions, int batch_size );

}
