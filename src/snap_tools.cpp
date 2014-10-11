#include "snap_tools.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <vector>

double GraphDensity(PNGraph& graph) {
    int N = graph->GetNodes();
    return static_cast<double>(graph->GetEdges()) / N / (N - 1);
}


double AverageDegree(PNGraph& graph) {
    int N = graph->GetNodes();
    return static_cast<double>(graph->GetEdges()) / N;
}


double DirectedModularity(PNGraph& graph, std::vector<int>& communities) {
    if (graph->GetNodes() != communities.size()) {
        throw std::logic_error("Number of nodes does not match community size.");
    }

    int num_edges = graph->GetEdges();
    double score = 0.0;

    int num_unique = 10;
    std::map<int, double> outdeg_sums;
    std::map<int, double> indeg_sums;

    for (TNGraph::TNodeI node = graph->BegNI(); node < graph->EndNI(); node++) {
        int comm = communities[node.GetId()];
        outdeg_sums[comm] += node.GetOutDeg();
        indeg_sums[comm] += node.GetInDeg();
    }

    for (auto& kv : outdeg_sums) {
        score -= (kv.second / num_edges) * indeg_sums[kv.first];
    }

    for (TNGraph::TNodeI node = graph->BegNI(); node < graph->EndNI(); node++) {
        int node_ID = node.GetId();
        for (int e = 0; e < node.GetOutDeg(); ++e) {
            int nbr = node.GetOutNId(e);
            if (communities[node_ID] == communities[nbr]) {
                score += 1.0;
            }
        }
    }  

    return score / num_edges;
}


void UndirCopy(PNGraph& dir_graph, PUNGraph& undir_graph) {
    // Add all of the nodes into the new graph
    for (TNGraph::TNodeI node = dir_graph->BegNI(); node < dir_graph->EndNI();
         node++) {
        int curr_node = node.GetId();
        undir_graph->AddNode(curr_node);
    }
    for (TNGraph::TNodeI node = dir_graph->BegNI(); node < dir_graph->EndNI();
         node++) {
        int curr_node = node.GetId();
        for (int e = 0; e < node.GetOutDeg(); ++e) {
            int nbr_node = node.GetOutNId(e);
            if (!undir_graph->IsEdge(curr_node, nbr_node)) {
                undir_graph->AddEdge(curr_node, nbr_node);
            }
        }
    }
}


void OnlyD3CEdges(PNGraph& dir_graph, PNGraph& d3c_graph, bool recip_edges) {
    // Add all of the nodes into the new graph
    for (TNGraph::TNodeI node = dir_graph->BegNI(); node < dir_graph->EndNI();
         node++) {
        int curr_node = node.GetId();
        d3c_graph->AddNode(curr_node);
    }
    for (TNGraph::TNodeI node = dir_graph->BegNI(); node < dir_graph->EndNI();
         node++) {
        int curr_node = node.GetId();
        auto curr_node_it = dir_graph->GetNI(curr_node);
        for (int out_edge = 0; out_edge < curr_node_it.GetOutDeg(); ++out_edge) {
            int out_node = curr_node_it.GetOutNId(out_edge);
            for (int in_edge = 0; in_edge < curr_node_it.GetInDeg(); ++in_edge) {
                int in_node = curr_node_it.GetInNId(in_edge);
		if (out_node == in_node && !recip_edges) { continue; }
                if (dir_graph->IsEdge(out_node, in_node)) {
                    if (!d3c_graph->IsEdge(out_node, in_node)) { d3c_graph->AddEdge(out_node, in_node); }
                    if (!d3c_graph->IsEdge(in_node, curr_node)) { d3c_graph->AddEdge(in_node, curr_node); }
                    if (!d3c_graph->IsEdge(curr_node, out_node)) { d3c_graph->AddEdge(curr_node, out_node); }
                }
            }
        }
    }
#ifdef _VERBOSE_
    std::cout << "Original graph edge count: " << dir_graph->GetEdges() << std::endl
	      << "D3C graph edge count: " << d3c_graph->GetEdges() << std::endl;
#endif
}

void OnlyD3CEdgesNoBack(PNGraph& dir_graph, PNGraph& d3c_graph) {
    // Add all of the nodes into the new graph
    for (TNGraph::TNodeI node = dir_graph->BegNI(); node < dir_graph->EndNI();
         node++) {
        int curr_node = node.GetId();
        d3c_graph->AddNode(curr_node);
    }
    for (TNGraph::TNodeI node = dir_graph->BegNI(); node < dir_graph->EndNI();
         node++) {
        int curr_node = node.GetId();
        auto curr_node_it = dir_graph->GetNI(curr_node);
        for (int out_edge = 0; out_edge < curr_node_it.GetOutDeg(); ++out_edge) {
            int out_node = curr_node_it.GetOutNId(out_edge);
            for (int in_edge = 0; in_edge < curr_node_it.GetInDeg(); ++in_edge) {
                int in_node = curr_node_it.GetInNId(in_edge);
                if (dir_graph->IsEdge(out_node, in_node) && out_node != in_node) {
		    if (!d3c_graph->IsEdge(in_node, out_node) &&
			!d3c_graph->IsEdge(curr_node, in_node) &&
			!d3c_graph->IsEdge(out_node, curr_node)) {
			if (!d3c_graph->IsEdge(out_node, in_node)) { d3c_graph->AddEdge(out_node, in_node); }
			if (!d3c_graph->IsEdge(in_node, curr_node)) { d3c_graph->AddEdge(in_node, curr_node); }
			if (!d3c_graph->IsEdge(curr_node, out_node)) { d3c_graph->AddEdge(curr_node, out_node); }
		    }
                }
            }
        }
    }
}
