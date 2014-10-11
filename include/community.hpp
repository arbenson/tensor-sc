#ifndef _COMMUNITY_HPP_
#define _COMMUNITY_HPP_

#include "cut_tools.hpp"
#include "network.hpp"
#include "snap_tools.hpp"

#include <vector>

// Class that wraps the community detection algorithms.
class CommDetection {
public:
  CommDetection(Network& net, int num_comms, int algorithm, int cut_type,
		std::string name);

    // Run the community detection algorithm.
    void Run();

    // Report the number of triples preserved by the communities.
    int NumTriplesPreserved();

    // Print the node and edge counts for each of the communities;
    void PrintCounts();

    // Returns the community of all nodes.  The return value is a vector such
    // that the i-th entry is the community of the i-th node.
    std::vector<int> Communities();

private:
    // Return the current network to work on, i.e. the one with the largest
    // number of triples.
    Network& CurrNetwork() { return networks_[CurrNetworkIndex()]; }

    // Remove the cut from the network on which we are currently operating.
    void RemoveCutFromCurrNetwork(Cutter& cutter, std::vector<int>& cut);

    // Run one step of recursive bisection.
    void Iterate();

    // Split into strongly connected components before running the algorithm.
    void ProcessSCCs();

    // Get the index in our list of networks of the one with the largest
    // number of nodes.
    int CurrNetworkIndex();

    std::vector<Network> networks_;
    std::vector< std::vector<int> > labels_;
    std::string name_;
    int num_comms_;
    int num_nodes_;
    int algorithm_;
    int cut_type_;
};


#endif  // _COMMUNITY_HPP_
