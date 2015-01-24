#ifndef _COMMON_HPP_
#define _COMMON_HPP_

#include <map>
#include <string>
#include <vector>

// Simple tuple class for reading triples of data
class Tuple {
public:
    Tuple(int x, int y, int z) : first(x), second(y), third(z) {}
    int first;
    int second;
    int third;
};


// Python 2.x range() function.  Returns the vector of elements
// (start, ..., end - 1).
std::vector<int> Range(int start, int end);

// Range starting from 0.
std::vector<int> Range(int end);


// Data sets
enum {
    WIKI_VOTE=0,
    EMAIL_EU,
    WIKI_TALK,
    SOC_EPINIONS,
    SLASHDOT,
    AS_CAIDA,
    AMAZON,
    TWITTER,
    WIKI_RFA,
    CIT_HEP_PH,
    WEB_STANFORD,
    EMAIL_ENRON,
    DBLP,
};

const int kNumNetworks = 13;


// Algorithms
enum {
    TENSOR,                 // Tensor Spectral Clustering (TSC)
    DIRLAP,                 // Chung's directed Laplacian
    LAP,                    // Laplacian with all edges considered undirected
    CO_U,                   // Co-clustering (left singular vector)
    CO_V,                   // Co-clustering (right singular vector)
    D3C_ONLY_DIRLAP,        // Directed laplacian, but only look at D3C subgraph
    D3C_ONLY_RECIP_DIRLAP,  // Directed laplacian, but only look at D3C 
                            // + reciprocated edges subgraph
    D3C_ONLY_NOBACK_DIRLAP, // Directed laplacian, but only look at D3C with
                            // no back edges subgraph
    ASYMMETRIC_LAP,         // Asymmetric laplacian
    RANDOM,                 // Random order sweep cut
};


// Cut types
enum {
    D3C_COND,             // cut3(S) / min(vol3(S), vol3(Sbar))
    D3C_EXPANSION,        // cut3(S) / min(|S|, |Sbar|)
    D3C_NORMALIZED,       // cut3(S) (1 / vol3(S) + 1 / vol3(Sbar))
    D3C_COND_RECIP,       // D3C_COND, but count reciprocated edges
    D3C_EXPANSION_RECIP,  // D3C_EXPANSION, but count reciprocated edges
    D3C_NOBACK_COND,      // D3C_COND, but only look at D3Cs with no back edges
    BIPARTITE,            // Number of edges cut
    NORMALIZED,           // normalized cut (assume undirected)
    COND,                 // conductance (assume undirected)
    EXPANSION,            // cut(S) / min(|S|, |Sbar|)
    DENSITY,              // Maximum density of S, Sbar
};


// Triple types
enum {
    D3CS,         // Directed 3-cycles
    D3CS_RECIP,   // Directed 3-cycles and reciprocated edges
    D3CS_NOBACK,  // Directed 3-cycles with no back edges
    OPEN_TRIS,    // Open triangles
};


// Given a network, return the text file name.
std::string NetworkStr(int network);

// Given an algorithm, return the name of the algorithm.
std::string AlgStr(int algorithm);

// Print information about the networks.
void PrintHelp();

#endif  // _COMMON_HPP_
