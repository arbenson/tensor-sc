#include "community.hpp"
#include "cut_tools.hpp"
#include "file_io.hpp"
#include "snap_tools.hpp"
#include "tensor_ops.hpp"

#include "Snap.h"

#include <iostream>
#include <stdexcept>
#include <vector>

CommDetection::CommDetection(Network& net, int num_comms, int algorithm, int cut_type,
			     std::string name) :
    num_comms_(num_comms), num_nodes_(net.NumNodes()), algorithm_(algorithm),
    cut_type_(cut_type), name_(name) {
    networks_.push_back(net);
    // TODO(arbenson): there is a better way to do this
    labels_.push_back(std::vector<int>());
    labels_[0] = Range(net.NumNodes());
}


// Run the community detection algorithm.
void CommDetection::Run() {
    while (networks_.size() < num_comms_) {
        Iterate();
    }
}

// Report the number of triples preserved by the communities.
int CommDetection::NumTriplesPreserved() {
    int sum = 0;
    for (Network& net : networks_) {
        sum += net.triples().size();
    }
    return sum;
}

// Print the node and edge counts for each of the communities;
void CommDetection::PrintCounts() {
    for (Network& net : networks_) {
        std::cout << net.NumNodes() << " " << net.NumEdges() << std::endl;
    }
}


// Returns the categories of all nodes.  The return value is a vector such
// that the i-th entry is the category of the i-th node.
std::vector<int> CommDetection::Communities() {
    std::vector<int> communities(num_nodes_, -1);
    for (int i = 0; i < labels_.size(); ++i) {
        std::vector<int>& curr_labels = labels_[i];
        for (int node : curr_labels) {
            communities[node] = i;
        }
    }

    for (int comm : communities) {
        if (comm == -1) {
            std::logic_error("Not all nodes assigned communities.");
        }
    }

    return communities;
}


void CommDetection::RemoveCutFromCurrNetwork(Cutter& cutter, std::vector<int>& cut) {
    std::cout << cut.size() << " of " << CurrNetwork().NumNodes()
              << " nodes cut." << std::endl;

    Network net1, net2;
    cutter.AddCut(cut, net1, net2);
    int curr_ind = CurrNetworkIndex();
    std::vector<int>& curr_labels = labels_[curr_ind];

    std::vector<int> labels1, labels2;
    for (int i = 0; i < net1.NumNodes(); ++i) {
        labels1.push_back(curr_labels[net1.NodeIndex(i)]);
    }
    for (int i = 0; i < net2.NumNodes(); ++i) {
        labels2.push_back(curr_labels[net2.NodeIndex(i)]);
    }

    networks_.erase(networks_.begin() + curr_ind);
    labels_.erase(labels_.begin() + curr_ind);
    networks_.push_back(net1);
    networks_.push_back(net2);
    labels_.push_back(labels1);
    labels_.push_back(labels2);
            
    if (networks_.size() != labels_.size()) {
        throw std::logic_error("Networks and labels do not match.");
    }
}


// Process the strongly connected components of the graph.  We only work
// on the largest SCC.
void CommDetection::ProcessSCCs() {
    Network& net = CurrNetwork();
    TCnComV components;
    TSnap::GetSccs(net.graph(), components);
    if (components.Len() == 1) {
        return;
    }
    int num_scc = components.Len();

    std::vector< std::vector<int> > cuts(num_scc);
    for (int j = 0; j < num_scc; ++j) {
        TCnCom comp = components[j];
        std::vector<int>& curr_cut = cuts[j];
        for (int i = 0; i < comp.Len(); ++i) {
            curr_cut.push_back(comp[i].Val);
        }
    }

    // Find the largest remaining component
    int argmax_ind = -1;
    int max_val = -1;
    for (int i = 0; i < cuts.size(); ++i) {
        if (max_val == -1 || cuts[i].size() > max_val) {
            argmax_ind = i;
            max_val = cuts[i].size();
        }
    }

    // Now we cut out each component
    std::cout << "Removing SCC" << std::endl;
    Cutter cutter(net, algorithm_, cut_type_, "");
    RemoveCutFromCurrNetwork(cutter, cuts[argmax_ind]);

    // TODO(arbenson): this is kindof a hack for dealing with SCCs.  We don't
    // want to process all of them because there are many isolated nodes.
    // Instead, we continue processing until the largest community (in terms
    // of the number of nodes) is a SCC.
    if (networks_.size() >= num_comms_) {
	return;
    }
    ProcessSCCs();
}


// Run one step of recursive bisection.
void CommDetection::Iterate() {
    std::cout << "Number of communities: " << networks_.size() << std::endl;
    ProcessSCCs();
    if (networks_.size() >= num_comms_) {
	return;
    }
    Cutter cutter(CurrNetwork(), algorithm_, cut_type_,
		  name_ + "_" + AlgStr(algorithm_));
    std::vector<int> cut = cutter.RunIter();
    RemoveCutFromCurrNetwork(cutter, cut);
    std::cout << "***" << std::endl;
}


// Get the index in our list of networks of the one with the largest
// number of triples.
int CommDetection::CurrNetworkIndex() {
    int curr_ind = -1;
    int curr_max_triples = 0;
    for (int i = 0; i < networks_.size(); ++i) {
        Network& net = networks_[i];
        if (curr_ind == -1 || net.triples().size() > curr_max_triples) {
            curr_ind = i;
            curr_max_triples = net.triples().size();
        }
    }
    return curr_ind;
}
