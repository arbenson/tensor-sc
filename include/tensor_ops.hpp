#ifndef _TENSOR_OPS_HPP_
#define _TENSOR_OPS_HPP_

#include "common.hpp"
#include "network.hpp"
#include "vector.hpp"

#include <vector>

// Routine to compute the second left eigenvector of the Markov Chain
// induced by the stationary distribution.
//
// triples is the list of third order moments (must be symmetric)
// counts is a counter such that counts[(u, v)] is the number of
//        third-order moments containing u and v
// stationary_distrib is the multilinear PageRank vector
// alpha is the teleportation parameter
// max_iter is the maximum number of power iterations to run
// tol is the relative error tolerance for stopping
template <typename Scalar>
Vector<Scalar> SecondLeftEvec(std::vector<Tuple>& triples, Counts& counts,
			      Vector<Scalar>& stationary_distrib,
			      Scalar alpha, int max_iter,
			      double tol);

// Find the multilinear PageRank vector.
// We use the SS-HOPM by Kolda and Mayo to compute the vector.
//
// triples is the list of third order moments (must be symmetric)
// counts is a counter such that counts[(u, v)] is the number of
//        third-order moments containing u and v
// alpha is the teleportation parameter
// gamma is the shift parameter for SS-HOPM
// num_nodes is the number of nodes
// max_iter is the maximum number of SS-HOPM iterations
// tol is the relative error tolerance for stopping
template <typename Scalar>
Vector<Scalar> MPRVec(std::vector<Tuple>& triples, Counts& counts,
		      Scalar alpha, Scalar gamma, int num_nodes, int max_iter,
		      double tol);


#endif  // _TENSOR_OPS_HPP_
