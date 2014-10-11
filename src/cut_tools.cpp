#include "cut_tools.hpp"

#include "common.hpp"
#include "file_io.hpp"
#include "laplacian.hpp"
#include "vector.hpp"
#include "tensor_ops.hpp"

#include "Snap.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <vector>


// TODO(arbenson): these are hacks
const double kBadScore = 1000000;
const double kMinCutSize = 2;


std::vector<int> inverse_vector(std::vector<int>& vec) {
    std::vector<int> vec_inv(vec.size(), 0);
    for (int i = 0; i < vec.size(); ++i) {
        vec_inv[vec[i]] = i;
    }
    return vec_inv;
}


std::vector<int> CutVector(PNGraph& graph, std::vector<int>& cut) {
    std::vector<int> keep(graph->GetNodes(), 1);
    for (int node : cut) {
        keep[node] = 0;
    }
    return keep;
}


void CutAndReIndex(PNGraph& graph, std::vector<int>& cut, PNGraph& new_graph,
                   std::vector<int>& new_index,
		   std::vector<int>& new_index_reverse) {
    int num_nodes = graph->GetNodes();

    // Compute the re-index, assumes graph is ordered 0, ..., num_nodes - 1
    std::vector<int> old_index = Range(num_nodes);
    for (int cut_node : cut) {
        old_index[cut_node] = -1;
    }

    // new_index[i] gives the index of node i (from the new graph) in the old graph
    new_index.resize(num_nodes - cut.size(), 0);
    int new_ind = 0;
    for (int val : old_index) {
        if (val != -1) {
            new_index[new_ind] = val;
            ++new_ind;
        }
    }

    if (new_ind != new_index.size()) {
        throw std::logic_error("Cut indexing problem");
    }

    // new_index_reverse[i] gives the index of node i (from the old graph) in
    // the new graph
    new_index_reverse = Range(num_nodes);
    int total_shift = 0;
    for (int i = 0; i < old_index.size(); ++i) {
        int val = old_index[i];
        if (val == -1) {
            total_shift += val;
            new_index_reverse[i] = val;
        } else {
            new_index_reverse[i] = val + total_shift;
        }
    }

    // Now that we have all of the indexing, create the new graph
    // First, add the nodes
    std::vector<int> keep = CutVector(graph, cut);
    new_graph = TNGraph::New();
    for (TNGraph::TNodeI node = graph->BegNI(); node < graph->EndNI(); node++) {
        int node_ID = node.GetId();
        if (keep[node_ID]) {
            new_graph->AddNode(new_index_reverse[node_ID]);
        }
    }

    // Second, add the edges
    for (TNGraph::TNodeI node = graph->BegNI(); node < graph->EndNI(); node++) {
        int node_ID = node.GetId();
        if (keep[node_ID]) {
            for (int e = 0; e < node.GetOutDeg(); ++e) {
                int nbr = node.GetOutNId(e);
                if (keep[nbr]) {
                    new_graph->AddEdge(new_index_reverse[node_ID],
                                       new_index_reverse[nbr]);
                }
            }
        }
    }
}


std::vector<int> GetSortedOrder(Vector<double>& vals) {
    // Get the ratio of the eigenvector and the stationary distribution.
    std::vector< std::pair<double, int> > vals_and_inds(vals.n());
    for (int i = 0; i < vals_and_inds.size(); ++i) {
        vals_and_inds[i] = std::pair<double, int>(vals(i), i);
    }

    // Sort the vals_and_inds by value
    std::sort(vals_and_inds.begin(), vals_and_inds.end());
    std::vector<int> order;
    for (std::pair<double, int>& val_ind : vals_and_inds) {
        order.push_back(val_ind.second);
    }
  
    return order;
}


void Volumes(std::vector<int>& order, PNGraph& graph,
             std::vector<int>& vol_boundary, std::vector<int>& vol_S,
             std::vector<int>& vol_Sbar) {
    std::vector<int> order_inv = inverse_vector(order);
    vol_boundary.resize(order.size(), 0);
    vol_S.resize(order.size(), 0);
    vol_Sbar.resize(order.size(), graph->GetEdges());

    for (int index = 0; index < order.size(); ++index) {
        int curr_node = order[index];
        auto curr_node_it = graph->GetNI(curr_node);

        if (index > 0) {
            vol_boundary[index] = vol_boundary[index - 1];
            vol_S[index] = vol_S[index - 1];
            vol_Sbar[index] = vol_Sbar[index - 1];
        }

        // Adjust volumes of S and Sbar
        int in_deg = curr_node_it.GetInDeg();
        vol_Sbar[index] -= in_deg;
        if (vol_Sbar[index] < 0) {
	    throw std::logic_error("Sbar volume is negative");
	}

        vol_S[index] += in_deg;
        if (vol_S[index] < 0) {
	    throw std::logic_error("S volume is negative");
	}
        
        // Adjust volume of boundary (outgoing edges)
        for (int out_edge = 0; out_edge < curr_node_it.GetOutDeg(); ++out_edge) {
            int out_node = curr_node_it.GetOutNId(out_edge);
            if (order_inv[out_node] > index) {
                // (curr_node, out_node) now contributes to boundary
                vol_boundary[index] += 1;
            }
        }
        // Adjust volume of boundary (incoming edges)
        for (int in_edge = 0; in_edge < curr_node_it.GetInDeg(); ++in_edge) {
            int in_node = curr_node_it.GetInNId(in_edge);
            if (order_inv[in_node] < index) {
                // (in_node, curr_node) no longer contributes to boundary
                vol_boundary[index] -= 1;
            }
        }

        if (vol_boundary[index] < 0) {
            throw std::logic_error("Boundary volume is negative");
        }
    }
}


void Edges(std::vector<int>& order, PNGraph& graph,
           std::vector<int>& vol_S, std::vector<int>& vol_Sbar) {
    std::vector<int> order_inv = inverse_vector(order);
    vol_S.resize(order.size(), 0);
    vol_Sbar.resize(order.size(), graph->GetEdges());

    for (int index = 0; index < order.size(); ++index) {
        int curr_node = order[index];
        auto curr_node_it = graph->GetNI(curr_node);

        if (index > 0) {
            vol_S[index] = vol_S[index - 1];
            vol_Sbar[index] = vol_Sbar[index - 1];
        }

        for (int out_edge = 0; out_edge < curr_node_it.GetOutDeg(); ++out_edge) {
            int out_node = curr_node_it.GetOutNId(out_edge);
	    if (out_node == curr_node) {
		std::cout << out_node << " " << curr_node << std::endl;
	    }
            if (order_inv[out_node] < index) {
                // Both nodes are now in S
                vol_S[index] += 1;
            } else if (order_inv[out_node] > index) {
                // Both nodes were in Sbar, but now the edge is cut.
                vol_Sbar[index] -=1 ;
            }
        }

        for (int in_edge = 0; in_edge < curr_node_it.GetInDeg(); ++in_edge) {
            int in_node = curr_node_it.GetInNId(in_edge);
            if (order_inv[in_node] < index) {
                // Both nodes are now in S
                vol_S[index] += 1;
            } else if (order_inv[in_node] > index) {
                // Both nodes were in Sbar, but now the edge is cut.
                vol_Sbar[index] -= 1;
            }
        }

        assert(vol_S[index] + vol_Sbar[index] <= graph->GetEdges());
        assert(vol_S[index] >= 0);
        assert(vol_Sbar[index] >= 0);
    }
    
    assert(vol_S.back() == graph->GetEdges());
    assert(vol_Sbar.back() == 0);
}


std::vector<int> BestCut(std::vector<double>& scores, std::vector<int>& order) {
    int best_sweep_index = -1;
    double best_sweep_score = kBadScore;
    for (int i = kMinCutSize - 1; i < order.size() - kMinCutSize; ++i) {
        if (scores[i] == kBadScore) {
            continue;
        } else {
            if ((best_sweep_score == kBadScore && scores[i] >= 0) ||
                scores[i] < best_sweep_score) {
                best_sweep_score = scores[i];
                best_sweep_index = i;
            } else if  ((best_sweep_score == kBadScore && scores[i] >= 0) ||
                        scores[i] == best_sweep_score) {
                int num_cut = std::min(i + 1, static_cast<int>(order.size()) - i - 1);
                int num_cut_prev =
		    std::min(best_sweep_index + 1,
			     static_cast<int>(order.size()) - best_sweep_index - 1);
                // Always favor a larger cut
                if (num_cut > num_cut_prev) {
                    best_sweep_score = scores[i];
                    best_sweep_index = i;
                }
            }
        }
    }

    if (best_sweep_index == -1) {
#ifdef _VERBOSE_
        std::cout << "Warning: could not find non-trivial cut." << std::endl;
#endif
        best_sweep_index = 0;
    }
#ifdef _VERBOSE_
    std::cout << "Cut value: " << best_sweep_score << std::endl; 
#endif

    // Now pick out the vectors for the cut.  Return the smaller of the cuts.
    std::vector<int> cut;
    if (best_sweep_index > order.size() - best_sweep_index) {
        for (int i = best_sweep_index; i < order.size(); ++i) {
            cut.push_back(order[i]);
        }
        return cut;
    }

    for (int i = 0; i <= best_sweep_index; ++i) {
        cut.push_back(order[i]);
    }

    return cut;
}


std::vector<int> Cutter::NormalizedCut(std::vector<int>& order, PNGraph& graph) {
    std::vector<int> vol_boundary;
    std::vector<int> vol_S;
    std::vector<int> vol_Sbar;
    Volumes(order, graph, vol_boundary, vol_S, vol_Sbar);

    std::vector<double> scores(order.size(), kBadScore);
    for (int i = 0; i < order.size(); ++i) {
        if (vol_S[i] == 0 || vol_Sbar[i] == 0) {
            scores[i] = kBadScore;
        } else {
            scores[i] = vol_boundary[i] * (1.0 / vol_S[i] + 1.0 / vol_Sbar[i]);
        }
    }
    return BestCut(scores, order);
}


std::vector<int> Cutter::ConductanceCut(std::vector<int>& order, PNGraph& graph) {
    std::vector<int> vol_boundary;
    std::vector<int> vol_S;
    std::vector<int> vol_Sbar;
    Volumes(order, graph, vol_boundary, vol_S, vol_Sbar);

    std::vector<double> scores(order.size(), kBadScore);
    for (int i = 0; i < order.size(); ++i) {
        if (vol_S[i] == 0 || vol_Sbar[i] == 0) {
            scores[i] = kBadScore;
        } else {
            if (vol_S[i] < vol_Sbar[i]) {
                scores[i] = static_cast<double>(vol_boundary[i]) / vol_S[i];
            } else {
                scores[i] = static_cast<double>(vol_boundary[i]) / vol_Sbar[i];
            }
        }
    }

#ifdef _VERBOSE_
    if (name_ != "") {
        WriteVector(vol_boundary, name_ + "_num_cut.txt");
        WriteVector(scores, name_ + "_cond.txt");
    }
#endif

    return BestCut(scores, order);
}


std::vector<int> Cutter::ExpansionCut(std::vector<int>& order, PNGraph& graph) {
    std::vector<int> vol_boundary;
    std::vector<int> vol_S;
    std::vector<int> vol_Sbar;
    Volumes(order, graph, vol_boundary, vol_S, vol_Sbar);

    std::vector<double> scores(order.size(), kBadScore);
    for (int i = 0; i < order.size(); ++i) {
        int size_S = i + 1;
        int size_Sbar = graph->GetNodes() - size_S;
        if (size_S == 0 || size_Sbar == 0) {
            scores[i] = kBadScore;
        } else {
            if (size_S < size_Sbar) {
                scores[i] = static_cast<double>(vol_boundary[i]) / size_S;
            } else {
                scores[i] = static_cast<double>(vol_boundary[i]) / size_Sbar;
            }
        }
    }
    return BestCut(scores, order);
}


void D3CVolumes(std::vector<int>& order, PNGraph& graph,
                std::vector<Tuple>& triples,
                std::vector<int>& num_cut, std::vector<int>& vol_S,
                std::vector<int>& vol_Sbar, bool count_recip_edges,
                bool no_back=false) {
    std::vector<int> order_inv = inverse_vector(order);
    num_cut.resize(order.size(), 0);
    vol_S.resize(order.size(), 0);
    vol_Sbar.resize(order.size(), triples.size());

    for (int index = 0; index < order.size(); ++index) {
        int curr_node = order[index];
        if (index > 0) {
            num_cut[index] = num_cut[index - 1];
            vol_S[index] = vol_S[index - 1];
            vol_Sbar[index] = vol_Sbar[index - 1];
        }
        auto curr_node_it = graph->GetNI(curr_node);
        for (int out_edge = 0; out_edge < curr_node_it.GetOutDeg(); ++out_edge) {
            int out_node = curr_node_it.GetOutNId(out_edge);
            for (int in_edge = 0; in_edge < curr_node_it.GetInDeg(); ++in_edge) {
                int in_node = curr_node_it.GetInNId(in_edge);
                if (!count_recip_edges && out_node == in_node) { continue; }
                if (no_back && (graph->IsEdge(in_node, out_node) ||
                                graph->IsEdge(curr_node, in_node) ||
                                graph->IsEdge(out_node, curr_node))) { continue; }
                if (!graph->IsEdge(out_node, in_node)) { continue; }

                if  (std::max(order_inv[out_node], order_inv[in_node]) < index) {
                    // The directed 3 cycle is no longer cut.  out_node and in_node are
                    // currently in the cut and we are adding the current node.
                    num_cut[index] -= 6;
                    vol_S[index] += 6;
                } else if (std::min(order_inv[out_node], order_inv[in_node]) > index) {
                    // The directed 3 cycle is now cut.  All nodes were _not_ in the cut
                    // and now the current node is being added to the cut.
                    num_cut[index] += 6;
                    vol_Sbar[index] -= 6;
                }
            }
        }
    }

    if (num_cut.back() != 0) {
	throw std::logic_error("Bad directed 3 cycle cut counting.");
    }
    if (vol_Sbar.back() != 0) {
	throw std::logic_error("Bad vol_Sbar counting.");
    }
    if (vol_S.back() != triples.size()) {
	throw std::logic_error("Bad vol_S counting.");
    }
}


void D3CVolumes2(std::vector<int>& order, PNGraph& graph,
                 std::vector<Tuple>& triples,
                 std::vector<int>& num_cut_S, std::vector<int>& num_cut_Sbar,
                 std::vector<int>& vol_S2, std::vector<int>& vol_Sbar2,
                 bool count_recip_edges, bool no_back=false) {
    std::vector<int> order_inv = inverse_vector(order);
    vol_S2.resize(order.size(), 0);
    vol_Sbar2.resize(order.size(), triples.size());
    num_cut_S.resize(order.size(), 0);
    num_cut_Sbar.resize(order.size(), 0);

    std::vector<int> num_cut(order.size(), 0);

    for (int index = 0; index < order.size(); ++index) {
        int curr_node = order[index];
        if (index > 0) {
            num_cut[index] = num_cut[index - 1];
            num_cut_S[index] = num_cut_S[index - 1];
            num_cut_Sbar[index] = num_cut_Sbar[index - 1];
            vol_S2[index] = vol_S2[index - 1];
            vol_Sbar2[index] = vol_Sbar2[index - 1];
        }
        auto curr_node_it = graph->GetNI(curr_node);
        for (int out_edge = 0; out_edge < curr_node_it.GetOutDeg(); ++out_edge) {
            int out_node = curr_node_it.GetOutNId(out_edge);
            for (int in_edge = 0; in_edge < curr_node_it.GetInDeg(); ++in_edge) {
                int in_node = curr_node_it.GetInNId(in_edge);
                if (!count_recip_edges && out_node == in_node) { continue; }
                if (no_back && (graph->IsEdge(in_node, out_node) ||
                                graph->IsEdge(curr_node, in_node) ||
                                graph->IsEdge(out_node, curr_node))) { continue; }
                if (!graph->IsEdge(out_node, in_node)) { continue; }

                if  (std::max(order_inv[out_node], order_inv[in_node]) < index) {
                    // The directed 3 cycle is no longer cut.  out_node and in_node are
                    // currently in the S and we are adding the current node to S.
                    num_cut[index] -= 6;
                    num_cut_S[index] -= 6;
                } else if (std::min(order_inv[out_node], order_inv[in_node]) > index) {
                    // The directed 3 cycle is now cut.  All nodes were _not_ in S
                    // and now the current node is being added to the S.
                    num_cut[index] += 6;
                    num_cut_Sbar[index] += 6;
                } else if (std::min(order_inv[out_node], order_inv[in_node]) < index &&
                           std::max(order_inv[out_node], order_inv[in_node]) > index) {
                    vol_Sbar2[index] -= 6;
                    vol_S2[index] += 6;
                    num_cut_S[index] += 6;
                    num_cut_Sbar[index] -= 6;
                }
            }
        }
        assert(num_cut_Sbar[index] <= vol_Sbar2[index]);
        assert(num_cut_S[index] <= vol_S2[index]);
        assert(num_cut_Sbar[index] + num_cut_S[index] == num_cut[index]);
    }

    if (num_cut.back() != 0) {
	throw std::logic_error("Bad directed 3 cycle cut counting.");
    }
    if (vol_Sbar2.back() != 0) {
	throw std::logic_error("Bad vol_Sbar counting.");
    }
    if (vol_S2.back() != triples.size()) {
	throw std::logic_error("Bad vol_S counting.");
    }
}


std::vector<int> Cutter::D3CExpansionCut(std::vector<int>& order, PNGraph& graph,
                                         std::vector<Tuple>& triples, bool recip_edges) {
    std::vector<int> order_inv = inverse_vector(order);
    std::vector<int> num_cut;
    std::vector<int> vol_S;
    std::vector<int> vol_Sbar;
    D3CVolumes(order, graph, triples, num_cut, vol_S, vol_Sbar, recip_edges);
    
    std::vector<double> scores(order.size(), kBadScore);
    for (int i = 0; i < order.size(); ++i) {
        int size_S = i + 1;
        int size_Sbar = graph->GetNodes() - size_S;
        if (size_S < size_Sbar) {
            scores[i] = static_cast<double>(num_cut[i]) / size_S;
        } else {
            scores[i] = static_cast<double>(num_cut[i]) / size_Sbar;
        }
    }

    return BestCut(scores, order);
}


std::vector<int> Cutter::D3CCondCut(std::vector<int>& order, PNGraph& graph,
                                    std::vector<Tuple>& triples, bool recip_edges,
                                    bool no_back) {
    std::vector<int> num_cut;
    std::vector<int> vol_S;
    std::vector<int> vol_Sbar;
    D3CVolumes(order, graph, triples, num_cut, vol_S, vol_Sbar,
               recip_edges, no_back);

    std::vector<double> scores(order.size(), kBadScore);
    for (int i = 0; i < order.size(); ++i) {
        if (vol_S[i] == 0 || vol_Sbar[i] == 0) {
            // Nothing is cut.
            scores[i] = kBadScore;
        } else {
            if (vol_S[i] < vol_Sbar[i]) {
                scores[i] = static_cast<double>(num_cut[i]) / vol_S[i];
            } else {
                scores[i] = static_cast<double>(num_cut[i]) / vol_Sbar[i];
            }
        }
    }

#ifdef _VERBOSE_
    if (name_ != "") {
        WriteVector(num_cut, name_ + "_num_cut.txt");
        WriteVector(scores, name_ + "_d3c_cond.txt");
    }
#endif

    return BestCut(scores, order);
}

std::vector<int> Cutter::D3CCond2Cut(std::vector<int>& order, PNGraph& graph,
                                     std::vector<Tuple>& triples, bool recip_edges,
                                     bool no_back) {
    std::vector<int> num_cut_S;
    std::vector<int> num_cut_Sbar;
    std::vector<int> vol_S;
    std::vector<int> vol_Sbar;
    D3CVolumes2(order, graph, triples, num_cut_S, num_cut_Sbar,
                vol_S, vol_Sbar, recip_edges, no_back);
    
    std::vector<double> scores(order.size(), kBadScore);
    for (int i = 0; i < order.size(); ++i) {
        if (vol_S[i] == 0 || vol_Sbar[i] == 0) {
            // Nothing is cut.
            scores[i] = kBadScore;
        } else {
            double score_S = static_cast<double>(num_cut_S[i]) / vol_S[i];
            double score_Sbar = static_cast<double>(num_cut_Sbar[i]) / vol_Sbar[i];
            if (score_S > score_Sbar) {
                scores[i] = score_S;
            } else {
                scores[i] = score_Sbar;
            }
        }
    }
    
#ifdef _VERBOSE_
    if (name_ != "") {
        WriteVector(scores, name_ + "_d3c_cond2.txt");
    }
#endif

    return BestCut(scores, order);
}


std::vector<int> Cutter::Density(std::vector<int>& order, PNGraph& graph,
                                 std::vector<Tuple>& triples) {
    std::vector<int> vol_S;
    std::vector<int> vol_Sbar;
    Edges(order, graph, vol_S, vol_Sbar);
    std::vector<double> scores(order.size(), kBadScore);
    for (int i = 0; i < order.size(); ++i) {
        if (vol_S[i] == 0 || vol_Sbar[i] == 0) {
            // Nothing is cut.
            scores[i] = kBadScore;
        } else {
            int size_S = i + 1;
            int size_Sbar = graph->GetNodes() - size_S;

            double score_S = static_cast<double>(vol_S[i]) / static_cast<double>(size_S) / size_S;
            double score_Sbar = static_cast<double>(vol_Sbar[i]) / static_cast<double>(size_Sbar) / size_Sbar;

            if (score_S > score_Sbar) {
                scores[i] = score_S;
            } else {
                scores[i] = score_Sbar;
            }
        }
    }
    
#ifdef _VERBOSE_
    if (name_ != "") {
        WriteVector(scores, name_ + "_density.txt");
    }
#endif

    return BestCut(scores, order);
}


std::vector<int> Cutter::D3CNormalizedCut(std::vector<int>& order, PNGraph& graph,
                                          std::vector<Tuple>& triples, bool recip_edges,
                                          bool no_back) {
    std::vector<int> num_cut;
    std::vector<int> vol_S;
    std::vector<int> vol_Sbar;
    D3CVolumes(order, graph, triples, num_cut, vol_S, vol_Sbar,
               recip_edges, no_back);

    std::vector<double> scores(order.size(), kBadScore);
    for (int i = 0; i < order.size(); ++i) {
        if (vol_S[i] == 0 || vol_Sbar[i] == 0) {
            // Nothing is cut.
            scores[i] = kBadScore;
        } else {
            scores[i] = static_cast<double>(num_cut[i]) / vol_S[i];
            scores[i] += static_cast<double>(num_cut[i]) / vol_Sbar[i];
        }
    }

#ifdef _VERBOSE_
    if (name_ != "") {
        WriteVector(num_cut, name_ + "_num_cut.txt");
        WriteVector(scores, name_ + "_d3c_ncut.txt");
    }
#endif

    return BestCut(scores, order);
}


std::vector<int> Cutter::D3CTouchCut(std::vector<int>& order, PNGraph& graph,
                                     std::vector<Tuple>& triples, bool recip_edges,
                                     bool no_back) {
    std::vector<int> num_cut;
    std::vector<int> vol_S;
    std::vector<int> vol_Sbar;
    D3CVolumes(order, graph, triples, num_cut, vol_S, vol_Sbar,
               recip_edges, no_back);

    std::vector<double> scores(order.size(), kBadScore);
    for (int i = 0; i < order.size(); ++i) {
        if (vol_S[i] == 0 || vol_Sbar[i] == 0) {
            // Nothing is cut.
            scores[i] = kBadScore;
        } else {
            int touch_S = triples.size() - vol_Sbar[i];
            int touch_Sbar = triples.size() - vol_S[i];
            if (touch_S < touch_Sbar) {
                scores[i] = static_cast<double>(num_cut[i]) / touch_S;
            } else {
                scores[i] = static_cast<double>(num_cut[i]) / touch_Sbar;
            }
        }
    }

#ifdef _VERBOSE_
    if (name_ != "") {
        WriteVector(num_cut, name_ + "_num_cut.txt");
        WriteVector(scores, name_ + "_touch.txt");
    }
#endif

    return BestCut(scores, order);
}


std::vector<int> Cutter::BipartiteCut(std::vector<int>& order, PNGraph& graph) {
    std::vector<int> vol_boundary;
    std::vector<int> vol_S;
    std::vector<int> vol_Sbar;
    Volumes(order, graph, vol_boundary, vol_S, vol_Sbar);

    std::vector<double> scores(order.size(), kBadScore);
    for (int i = 0; i < order.size(); ++i) {
        int size_S = i + 1;
        int size_Sbar = graph->GetNodes() - size_S;
        if (size_S == 0 || size_Sbar == 0) {
            scores[i] = kBadScore;
        } else {
            scores[i] = 1.0 / static_cast<double>(vol_boundary[i]);
        }
    }

    return BestCut(scores, order);
}


void Cutter::AddCut(std::vector<int>& cut, Network& net1, Network& net2) {
    std::vector<int> all_nodes(NumNodes());
    for (int i = 0; i < all_nodes.size(); ++i) {
        all_nodes[i] = i;
    }
    for (int node : cut) {
        all_nodes[node] = -1;
    }

    std::vector<int> anti_cut;
    for (int node : all_nodes) {
        if (node != -1) {
            anti_cut.push_back(node);
        }
    }
  
    net1 =  Network(graph(), triples(), cut);
    net2 =  Network(graph(), triples(), anti_cut);
}


std::vector<int> Cutter::GetOrder() {
    double tol_sd = 1e-5;
    double tol_evec = 1e-5;
    int max_iter = 1000;

    switch (algorithm_) {
    case MULTILINEAR:
        {
            // TODO(arbenson): make these parameters arguments
            double alpha = 0.99;
            double gamma = 0.01;
            
            Vector<double> sd =
		MPRVec<double>(triples(), counts(), alpha, gamma, NumNodes(),
			       max_iter, tol_sd);
            Vector<double> fiedler =
		SecondLeftEvec<double>(triples(), counts(), sd, alpha,
				       max_iter, tol_evec);
            return GetSortedOrder(fiedler);
        }
        break;
    case LAP:
        {
            // First, get an undirected version of the graph.
            PUNGraph undir_graph = TUNGraph::New();
            UndirCopy(graph(), undir_graph);

	    // Now get the eigenvector
            Vector<double> fiedler = laplacian::UndirEvec<double>(undir_graph,
                                                                  max_iter, tol_evec);
            return GetSortedOrder(fiedler);
        }
        break;
    case DIRLAP:
        {
            Vector<double> sd =
		laplacian::DirStationaryDistrib<double>(graph(), max_iter, tol_sd);
            Vector<double> evec =
		laplacian::DirEvec(graph(), sd, max_iter, tol_evec);
            return GetSortedOrder(evec);
        }
        break;
    case D3C_ONLY_DIRLAP:
        {
            // First, get an undirected version of the graph.
            PNGraph d3c_graph = TNGraph::New();
            OnlyD3CEdges(graph(), d3c_graph, false);

            Vector<double> sd =
		laplacian::DirStationaryDistrib<double>(d3c_graph, max_iter, tol_sd);
            Vector<double> evec =
		laplacian::DirEvec<double>(d3c_graph, sd, max_iter, tol_evec);
            return GetSortedOrder(evec);
        }
        break;
    case D3C_ONLY_RECIP_DIRLAP:
        {
            // First, get an undirected version of the graph.
            PNGraph d3c_graph = TNGraph::New();
            OnlyD3CEdges(graph(), d3c_graph, true);

            Vector<double> sd =
		laplacian::DirStationaryDistrib<double>(d3c_graph, max_iter,
							tol_sd);
            Vector<double> evec =
		laplacian::DirEvec<double>(d3c_graph, sd, max_iter, tol_evec);
            return GetSortedOrder(evec);
        }
        break;
    case D3C_ONLY_NOBACK_DIRLAP:
        {
            // First, get an undirected version of the graph.
            PNGraph d3c_graph = TNGraph::New();
            OnlyD3CEdgesNoBack(graph(), d3c_graph);

            Vector<double> sd =
		laplacian::DirStationaryDistrib<double>(d3c_graph, max_iter,
							tol_sd);
            Vector<double> evec =
		laplacian::DirEvec<double>(d3c_graph, sd, max_iter, tol_evec);
            return GetSortedOrder(evec);
        }
        break;
    case ASYMMETRIC_LAP:
        {
            Vector<double> evec =
		laplacian::AsymmetricEvec<double>(graph(), max_iter, tol_evec);
            return GetSortedOrder(evec);
        }
        break;
    case RANDOM:
        {
            std::vector<int> rand_order(NumNodes());
            for (int i = 0; i < rand_order.size(); ++i) {
                rand_order[i] = i;
            }
            std::random_shuffle(rand_order.begin(), rand_order.end());
            return rand_order;
        }
    default:
        throw std::logic_error("Invalid algorithm");
    }
}


std::vector<int> Cutter::GetCut(std::vector<int>& order) {
    switch (cut_type_) {
    case D3C_COND:
        return D3CCondCut(order, graph(), triples(), false, false);
    case DENSITY:
        return Density(order, graph(), triples());
    case D3C_TOUCH:
        return D3CTouchCut(order, graph(), triples(), false, false);
    case D3C_COND_2NODE:
        return D3CCond2Cut(order, graph(), triples(), false, false);
    case D3C_EXPANSION:
        return D3CExpansionCut(order, graph(), triples(), false);
    case D3C_NORMALIZED:
        return D3CNormalizedCut(order, graph(), triples(), false, false);
    case D3C_COND_RECIP:
        return D3CCondCut(order, graph(), triples(), true, false);
    case D3C_NOBACK_COND:
        return D3CCondCut(order, graph(), triples(), false, true);
    case NORMALIZED:
        return NormalizedCut(order, graph());
    case COND:
        return ConductanceCut(order, graph());
    case EXPANSION:
        return ExpansionCut(order, graph());
    case BIPARTITE:
        return BipartiteCut(order, graph());
    default:
        throw std::logic_error("Invalid cut type");
    }
}


std::vector<int> Cutter::RunIter() {
    std::vector<int> order = GetOrder();
#ifdef _VERBOSE_
    if (name_ != "") {
	WriteVector(order, name_ + "_order.txt");
    }
#endif

    return GetCut(order);
}
