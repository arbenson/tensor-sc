#ifndef _CUT_TOOLS_HPP_
#define _CUT_TOOLS_HPP_

#include "common.hpp"
#include "network.hpp"
#include "vector.hpp"

#include "Snap.h"

#include <string>
#include <vector>

// Compute the "inverse" of the vector vec.  The vector is assumed to be
// a permutation of {0, 1, ..., N - 1}.  The inverse vector provides
// the permutation.
std::vector<int> inverse_vector(std::vector<int>& vec);

// Given a graph and a cut, return a 0/1 vector of which nodes are kept in the
// new graph. Runs in O(N) time.
std::vector<int> CutVector(PNGraph& graph, std::vector<int>& cut);

// Cut the graph and re-index it so that the nodes are 0-indexed.
void CutAndReIndex(PNGraph& graph, std::vector<int>& cut, PNGraph& new_graph,
		   std::vector<int>& new_index, std::vector<int>& new_index_reverse);

// Get ordering of vals.
std::vector<int> GetSortedOrder(Vector<double>& vals);

// Given a vector of scores and the order of the sweep, return the best cut.
std::vector<int> BestCut(std::vector<double>& scores, std::vector<int>& order);


// Class that wraps all of the cutting tools.
class Cutter {
public:
    Cutter(Network& network, int algorithm, int cut_type, std::string name) :
	network_(network), algorithm_(algorithm), cut_type_(cut_type),
	name_(name) {}

    void AddCut(std::vector<int>& cut, Network& net1, Network& net2);
    std::vector<int> RunIter();

    int NumNodes() { return network_.NumNodes(); }
    PNGraph& graph() { return network_.graph(); }
    std::vector<Tuple>& triples() { return network_.triples(); }
    Counts& counts() { return network_.counts(); }

    std::vector<int> D3CCondCut(std::vector<int>& order, PNGraph& graph,
				std::vector<Tuple>& triples, bool recip_edges,
				bool no_back);

    std::vector<int> Density(std::vector<int>& order, PNGraph& graph,
			     std::vector<Tuple>& triples);

    std::vector<int> D3CNormalizedCut(std::vector<int>& order, PNGraph& graph,
				      std::vector<Tuple>& triples, bool recip_edges,
				      bool no_back);

    std::vector<int> D3CExpansionCut(std::vector<int>& order, PNGraph& graph,
				     std::vector<Tuple>& triples, bool recip_edges);

    // Normalized cut for a directed graph
    std::vector<int> NormalizedCut(std::vector<int>& order, PNGraph& graph);

    // Normalized cut for a directed graph
    std::vector<int> ExpansionCut(std::vector<int>& order, PNGraph& graph);

    // Conductance cut for a directed graph
    std::vector<int> ConductanceCut(std::vector<int>& order, PNGraph& graph);

    std::vector<int> BipartiteCut(std::vector<int>& order, PNGraph& graph);

    std::vector<int> GetCut(std::vector<int>& order);

    std::vector<int> GetOrder();

private:
    Network network_;
    int algorithm_;
    int cut_type_;
    std::string name_;
};

#endif  // _CUT_TOOLS_HPP_
