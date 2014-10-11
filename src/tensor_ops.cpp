#include "tensor_ops.hpp"
#include "linalg.hpp"
#include "Vector.hpp"

#include <iostream>
#include <vector>

// Apply the row stochastic matrix P
// y <-- P * curr_iter
template <typename Scalar>
Vector<Scalar> RowStochApply(std::vector<Tuple>& triples, Counts& counts,
			     Vector<Scalar>& stationary_distrib,
			     Vector<Scalar>& curr_iter) {
    Vector<Scalar> next_iter(stationary_distrib.n(), 0.0);
    // Note: we need to account for the transpose
    for (Tuple triple : triples) {
	int i = triple.first;
	int j = triple.second;
	int k = triple.third;

	// Apply (i, j, k): (R_k)_{ij}
	next_iter(j) += counts.GetProb(j, k) * curr_iter(i) * stationary_distrib(k);
    }

    return next_iter;
}


template <typename Scalar>
Vector<Scalar> ApplyRowStochProbs(std::vector<Tuple>& triples, Counts& counts,
				  Vector<Scalar>& stationary_distrib,
				  Vector<Scalar>& curr_iter,
				  Vector<Scalar>& leftover,
				  Vector<Scalar>& u) {
    // First apply the probability tensor, weighted by the stationary distrib
    Vector<Scalar> res1 = RowStochApply(triples, counts, stationary_distrib, curr_iter);

    // Handle dangling nodes
    double scale = Dot(curr_iter, u);
    Add(res1, leftover, 1.0, scale, res1);

    return res1;
}


// One application for the power iteration when computing the second left
// eigenvector.  This function does the following to curr_iter
//    1. y := curr_iter^T * P[x]
//    2. Orthogonalize y against e
//    3. Scale y to have unit 1-norm.
template <typename Scalar>
Vector<Scalar> ApplyAndOrthog(std::vector<Tuple>& triples, Counts& counts,
		     Vector<Scalar>& stationary_distrib,
		     Vector<Scalar>& curr_iter,
		     Vector<Scalar>& leftover,
		     Vector<Scalar>& u) {
    Vector<Scalar> res = ApplyRowStochProbs(triples, counts, stationary_distrib,
					    curr_iter, leftover, u);
    OrthogAllOnes(res);
    res.Normalize1();
    return res;
}


// Propogate the vertex-reinforced random walk dynamical system.
// In our notation, this means
//     y <--- R (x \kron x)
//
// triples is the list of third-order moments
// counts contains the counts for the prbabilities
template <typename Scalar>
Vector<Scalar> VRRWPropogate(std::vector<Tuple>& triples, Counts& counts,
			     Vector<Scalar>& curr_iter) {
    Vector<Scalar> next_iter(curr_iter.n(), 0.0);
    for (Tuple triple : triples) {
	int i = triple.first;
	int j = triple.second;
	int k = triple.third;

	// Apply (i, j, k): (R_k)_{ij}
	next_iter(i) += counts.GetProb(j, k) * curr_iter(j) * curr_iter(k);
    }
    return next_iter;
}


// One step of the tensor PageRank iteration. 
//
// triples is the list of third order moments (must be symmetric)
// counts is a counter such that counts[(u, v)] is the number of
//        third-order moments containing u and v
// curr_iter is the vector on which we are iterating
// alpha is the teleportation parameter
// gamma is the shift parameter for SS-HOPM
template <typename Scalar>  
Vector<Scalar> PRApply(std::vector<Tuple>& triples, Counts& counts,
		       Vector<Scalar>& curr_iter, Scalar alpha, Scalar gamma) {
    // Teleportation and dangling vectors
    Vector<Scalar> v = UniformVector<Scalar>(curr_iter.n());
    Vector<Scalar> u = UniformVector<Scalar>(curr_iter.n());

    Vector<Scalar> z = VRRWPropogate(triples, counts, curr_iter);
    // Handle dangling cases
    Update(z, curr_iter.Sum() - z.Sum(), u);

    Vector<Scalar> w(z.n());
    Add(z, v, alpha, 1 - alpha, w);
    Add(w, curr_iter, 1.0 / (1 + gamma), gamma / (1 + gamma), z);
    
    return z;
}


template <typename Scalar>
Vector<Scalar> SecondLeftEvec(std::vector<Tuple>& triples, Counts& counts,
			      Vector<Scalar>& stationary_distrib,
			      Scalar alpha, int max_iter, double tol) {
    Vector<Scalar> v = UniformVector<Scalar>(stationary_distrib.n());
    Vector<Scalar> u = UniformVector<Scalar>(stationary_distrib.n());

    Vector<Scalar> curr_vec = RandomVector<Scalar>(stationary_distrib.n());
    curr_vec.Scale(1.0 / Norm1(curr_vec));
    OrthogAllOnes(curr_vec);
    double diff_orig = 2.0;

    // Compute leftover vector
    Vector<Scalar> allones(curr_vec.n(), 1.0);
    Vector<Scalar> res2 = RowStochApply(triples, counts, stationary_distrib, allones);
    Vector<Scalar> leftover(allones.n());
    Add(allones, res2, 1.0, -1.0, leftover);

    for (int i = 0; i < max_iter; ++i) {
	Vector<Scalar> save_vec = curr_vec;  // copy
	curr_vec = ApplyAndOrthog(triples, counts, stationary_distrib,
				  curr_vec, leftover, u);
	double diff = PowerIterResid(save_vec, curr_vec);

#ifdef _VERBOSE_
	std::cout << "diff: " << diff << std::endl;
#endif
	if (i == 0) { diff_orig = diff; }
	if (i > 0 && diff / diff_orig < tol) { break; }
    }
    
    return curr_vec;
}


template <typename Scalar>
Vector<Scalar> MPRVec(std::vector<Tuple>& triples, Counts& counts,
		      Scalar alpha, Scalar gamma, int num_nodes, int max_iter,
		      double tol) {
    Vector<Scalar> curr_vec = RandomVector<Scalar>(num_nodes);
    curr_vec.Scale(1.0 / Norm1(curr_vec));
    double diff_orig = 2.0;
    for (int i = 0; i < max_iter; ++i) {
	Vector<Scalar> save_vec = curr_vec;  // copy
	curr_vec = PRApply(triples, counts, curr_vec, alpha, gamma);
	double diff = Norm1Diff(curr_vec, save_vec);
#ifdef _VERBOSE_
	std::cout << "diff: " << diff << std::endl;
#endif
	if (i == 0) { diff_orig = diff; }
	if (i > 0 && diff / diff_orig < tol) { break; }
    }

    return curr_vec;
}


// Declarations
template 
Vector<double> RowStochApply(std::vector<Tuple>& triples, Counts& counts,
			     Vector<double>& stationary_distrib,
			     Vector<double>& curr_iter);

template 
Vector<double> ApplyRowStochProbs(std::vector<Tuple>& triples, Counts& counts,
				  Vector<double>& stationary_distrib,
				  Vector<double>& curr_iter,
				  Vector<double>& leftover,
				  Vector<double>& u);
template 
Vector<double> VRRWPropogate(std::vector<Tuple>& triples, Counts& counts,
			     Vector<double>& x);

template 
Vector<double> ApplyAndOrthog(std::vector<Tuple>& triples, Counts& counts,
			      Vector<double>& stationary_distrib,
			      Vector<double>& curr_iter,
			      Vector<double>& leftover,
			      Vector<double>& u);

template 
Vector<double> SecondLeftEvec(std::vector<Tuple>& triples, Counts& counts,
			      Vector<double>& stationary_distrib,
			      double alpha, int max_iter, double tol);


template   
Vector<double> PRApply(std::vector<Tuple>& triples, Counts& counts,
		       Vector<double>& curr_iter, double alpha, double gamma);

template 
Vector<double> MPRVec(std::vector<Tuple>& triples, Counts& counts,
		      double alpha, double gamma, int N, int max_iter,
		      double tol);
