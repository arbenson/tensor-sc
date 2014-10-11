#ifndef _LAPLACIAN_HPP_
#define _LAPLACIAN_HPP_

#include "vector.hpp"
#include "Snap.h"

// These are all of the tools for computations involving the directed or
// undirected Laplacian.

namespace laplacian {
    // Compute z_{n+1} = Pz_{n}, where P = D^{-1}A
    //
    // graph is the network of interest
    // curr_iter is z_{n} as above
    template <typename Scalar>
    Vector<Scalar> RightApplyDir(PNGraph& graph, Vector<Scalar>& curr_iter);


    // Compute z_{n+1} = P^Tz_{n}, where P = D^{-1}A
    //
    // graph is the network of interest
    // curr_iter is z_{n} as above
    template <typename Scalar>
    Vector<Scalar> LeftApplyDir(PNGraph& graph, Vector<Scalar>& curr_iter);


    // Compute z_{n+1} = Pz_{n}, where P = D^{-1}A is row-stochastic.
    //
    // graph is the network of interest
    // curr_iter is z_{n} as above
    template <typename Scalar>
    Vector<Scalar> RightApplyUndir(PUNGraph& graph, Vector<Scalar>& curr_iter);


    // One iteration of power method to compute next iteration of the 
    // directed combinatorial Laplacian second eigenvector
    //
    // graph is the network of interest
    // curr_iter is the current iteration
    // stationary_distrib is the stationary distribution of the directed
    //     random walk Markov chain
    template <typename Scalar>
    Vector<Scalar> DirEvecIter(PNGraph& graph, Vector<Scalar>& curr_iter,
			       Vector<Scalar>& stationary_distrib);
    

    // One power iteration for computing the second left eigenvector for
    // the undirected Laplacian
    template <typename Scalar>
    Vector<Scalar> UndirEvecIter(PUNGraph& graph, Vector<Scalar>& curr_iter);


    // Compute the second eigenvector of the directed Laplacian
    //
    // graph is the network of interest
    // stationary_distrib is the stationary distribution of the directed
    //     random walk Markov chain
    // max_iter is the maximum number of power iterations to use
    // tol is the stopping tolerance
    template <typename Scalar>
    Vector<Scalar> DirEvec(PNGraph& graph, Vector<Scalar>& stationary_distrib,
			   int max_iter, double tol);


    // Compute the second left eigenvector of P = D^{-1}A
    // This is used for the conductance cut.
    //
    // graph is the network of interest
    // max_iter is the maximum number of power iterations
    // tol is the stopping tolerance
    template <typename Scalar>
    Vector<Scalar> UndirEvec(PUNGraph& graph, int max_iter, double tol);


    // Return stationary distribution for a directed graph
    //
    // graph is the network of interest
    // tol is the stopping tolerance
    template <typename Scalar>
    Vector<Scalar> DirStationaryDistrib(PNGraph& graph, int max_iter, double tol);


    // Return stationary distribution for an undirected graph.
    // This is just z_i = d_i / |E|
    //
    // graph is the network of interest
    template <typename Scalar>
    Vector<Scalar> UndirStationaryDistrib(PUNGraph& graph);


    // Return the second left eigenvector of P = D^{-1}A
    template <typename Scalar>
    Vector<Scalar> AsymmetricEvec(PNGraph& graph, int max_iter, double tol);

};  // namespace laplacian


#endif  // _LAPLACIAN_HPP_
