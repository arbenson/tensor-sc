#ifndef _NETWORK_HPP_
#define _NETWORK_HPP_

#include "common.hpp"
#include "Snap.h"
#include "snap_tools.hpp"

// Keep track of the number of triples in which a pair of nodes is involved.
class Counts {
public:
    Counts() {};
    Counts(int n) { counts_.resize(n); }
    ~Counts() {};
    
    // Increment sum of T(:, v, w)
    void Increment(int v, int w);
        
    // 1 / sum(T(:, v, w))
    double GetProb(int v, int w);

    int size() { return counts_.size(); }

private:
    std::vector< std::map<int, int> > counts_;
};


// This class wraps a network with its triple list.
class Network {
public:
    Network() {}

    // Construct a network from file.
    // 
    // data_dir is the directory of where the data is located
    // name is the location of the 
    Network(std::string data_dir, std::string name, int triple_type);

    // graph is the graph object
    // triples is the list of triples (e.g., directed 3-cycles)
    // cut is the partition we are going to use on the network.
    Network(PNGraph& graph, std::vector<Tuple>& triples,
	    std::vector<int>& cut);

    int NumNodes() { return graph_->GetNodes(); }
    int NumEdges() { return graph_->GetEdges(); }
    int Density() { return GraphDensity(graph_); }
    int NodeIndex(int i);
    PNGraph& graph() { return graph_; }
    Counts& counts() { return counts_; }
    std::vector<Tuple>& triples() { return triples_; }

private:
    // Update the triples after the cut.
    void UpdateTriples();

    // Whether or not a triple should be kept after the cut.
    bool KeepTriple(Tuple& tuple) { return keep_vector_[tuple.first] &&
	    keep_vector_[tuple.second] && keep_vector_[tuple.third]; }

    PNGraph graph_;
    std::vector<Tuple> triples_;
    std::vector<int> cut_;
    std::vector<int> keep_vector_;
    std::vector<int> index_;
    std::vector<int> reverse_index_;
    Counts counts_;
};


// Default method to retrieve a network for file.
Network GetNetwork(int network, int triple_type);

#endif  // _NETWORK_HPP_
