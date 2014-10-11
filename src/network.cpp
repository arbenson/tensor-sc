#include "common.hpp"
#include "cut_tools.hpp"
#include "file_io.hpp"
#include "network.hpp"
#include "Snap.h"

#include <iostream>
#include <stdexcept>
#include <string>

// Go through the triple list and increment the counts.
// After completion, we have sums T(:, j, k) for all j, k.
void CountPairs(Counts& counts, std::vector<Tuple>& triples) {
    for (Tuple& triple : triples) {
        int i = triple.first;
        int j = triple.second;
        int k = triple.third;
        counts.Increment(j, k);
    }
}


Network GetNetwork(int network, int triple_type) {
    return Network("data", NetworkStr(network), triple_type);
}

Network::Network(PNGraph& graph, std::vector<Tuple>& triples,
                 std::vector<int>& cut) :
    graph_(graph), triples_(triples), cut_(cut) {
    // Remove the cut
    keep_vector_ = CutVector(graph_, cut_);
    cut_ = cut;
    PNGraph new_graph;
    CutAndReIndex(graph_, cut_, new_graph, index_, reverse_index_);

    // Update data
    graph_ = new_graph;
    UpdateTriples();
    counts_ = Counts(new_graph->GetNodes());
    CountPairs(counts_, triples_); 

    std::cout << "New network statistics: "
	      << "nodes: "             << graph_->GetNodes()    << ", "
	      << "edges: "             << graph_->GetEdges()    << ", "
	      << "triples: "           << triples_.size()       << ", "
	      << "density: "           << GraphDensity(graph_)  << ", "
	      << "average degree: "    << AverageDegree(graph_) << std::endl;
}


Network::Network(std::string data_dir, std::string name, int triple_type) {
    std::string base_name = data_dir + "/" + name;
    switch (triple_type) {
    case D3CS:
	triples_ = ReadTriples(base_name + "-d3c.txt");
	break;
    case D3CS_RECIP:
	triples_ = ReadTriples(base_name + "-d3c-recip.txt");
	break;
    case OPEN_TRIS:
	triples_ = ReadTriples(base_name + "-open-tris.txt");
	break;
    case D3CS_NOBACK:
	triples_ = ReadTriples(base_name + "-d3c-noback.txt");
	break;
    default:
	throw std::logic_error("Unknown triple type!");
    }
    std::string file_name = base_name + ".txt";
    graph_ = TSnap::LoadEdgeList<PNGraph>(file_name.c_str(), 0, 1);
    counts_ = Counts(graph_->GetNodes());
    CountPairs(counts_, triples_);
}


int Network::NodeIndex(int i) {
    if (i < 0 || i > index_.size()) {
        throw std::out_of_range("Node index out of range");
    }
    return index_[i];
}


void Network::UpdateTriples() {
    std::vector<Tuple> new_triples;
    // Only keep the triple if all three nodes of the triple are kept.
    for (Tuple& tuple : triples_) {
        if (keep_vector_[tuple.first] && keep_vector_[tuple.second] &&
            keep_vector_[tuple.third]) {
            new_triples.push_back(Tuple(reverse_index_[tuple.first],
                                        reverse_index_[tuple.second],
                                        reverse_index_[tuple.third]));
        }
    }
    triples_ = new_triples;
}


void Counts::Increment(int u, int v) {
    assert(u < counts_.size());
    assert(v < counts_.size());
    assert(u >= 0);
    assert(v >= 0);
    std::map<int, int>& counts = counts_[u];
    counts[v] += 1;
}

// 1 / sum T(:, u, v)        
double Counts::GetProb(int u, int v)  { 
    if (!(0 <= u && u < counts_.size()
          && counts_[u].find(v) != counts_[u].end())) {

        std::cout << u << " " << v << std::endl;

    }
    assert(0 <= u && u < counts_.size()
           && counts_[u].find(v) != counts_[u].end());
    return 1.0 / counts_[u][v];
}
