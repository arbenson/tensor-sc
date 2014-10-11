#ifndef _SNAP_TOOLS_HPP_
#define _SNAP_TOOLS_HPP_

#include "common.hpp"

#include "Snap.h"

#include <vector>

// These are just some helper tools that snap doesn't have.


// Compute the density of the graph.
double GraphDensity(PNGraph& graph);

// Compute the average degree of the graph.
double AverageDegree(PNGraph& graph);

// Copy a directed graph into an undirected one.
void UndirCopy(PNGraph& dir_graph, PUNGraph& undir_graph);

// Given a directed graph, create a new subgraph that only contains
// edges that participate in at least one directed 3-cycle.
void OnlyD3CEdges(PNGraph& dir_graph, PNGraph& dir3cycle_graph, bool recip_edges);

// Given a directed graph, create a new subgraph that only contains
// edges that participate in at least one directed 3-cycle with no back edges.
void OnlyD3CEdgesNoBack(PNGraph& dir_graph, PNGraph& d3c_graph);

// Compute the directed modularity of the graph given the communities.
// Nodes i and j are in the same community if and only if communities[i]
// equals communities[j].
//
// Graph is the network on which to compute the directed modularity.
// communities is an array of length |V| of communities
double DirectedModularity(PNGraph& graph, std::vector<int>& communities);

#endif  // _SNAP_TOOLS_HPP_
