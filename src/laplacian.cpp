#include "laplacian.hpp"
#include "linalg.hpp"
#include "Snap.h"
#include "vector.hpp"

#include <iostream>

namespace laplacian {
    template <typename Scalar>
    Vector<Scalar> RightApplyDir(PNGraph& graph, Vector<Scalar>& curr_iter) {
        Vector<Scalar> next_iter(curr_iter.n(), 0.0);
        for (TNGraph::TNodeI node = graph->BegNI(); node < graph->EndNI(); node++) {
            int curr_node = node.GetId();
            for (int e = 0; e < node.GetOutDeg(); ++e) {
                int nbr_node = node.GetOutNId(e);
                next_iter(curr_node) += curr_iter(nbr_node) / node.GetOutDeg();
            }
        }
        return next_iter;
    }


    template <typename Scalar>
    Vector<Scalar> LeftApplyDir(PNGraph& graph, Vector<Scalar>& curr_iter) {
        Vector<Scalar> next_iter(curr_iter.n(), 0.0);
        for (TNGraph::TNodeI node = graph->BegNI(); node < graph->EndNI(); node++) {
            int curr_node = node.GetId();
            for (int e = 0; e < node.GetOutDeg(); ++e) {
                int nbr_node = node.GetOutNId(e);
                next_iter(nbr_node) += curr_iter(curr_node) / node.GetOutDeg();
            }
        }
        return next_iter;
    }


    template <typename Scalar>
    Vector<Scalar> RightApplyUndir(PUNGraph& graph, Vector<Scalar>& curr_iter) {
        Vector<Scalar> next_iter(curr_iter.n(), 0.0);
        for (TUNGraph::TNodeI node = graph->BegNI(); node < graph->EndNI(); node++) {
            int curr_node = node.GetId();
            for (int e = 0; e < node.GetOutDeg(); ++e) {
                int nbr_node = node.GetOutNId(e);
                next_iter(curr_node) += curr_iter(nbr_node) / node.GetOutDeg();
            }
        }
        return next_iter;
    }


    template <typename Scalar>
    Vector<Scalar> DirEvecIter(PNGraph& graph, Vector<Scalar>& curr_iter,
                               Vector<Scalar>& stationary_distrib) {
        // tmp1 = Pi * x
        Vector<Scalar> tmp1 = EntryWiseProduct(stationary_distrib, curr_iter);

        // tmp1 = P^T * tmp1
        tmp1 = LeftApplyDir(graph, tmp1);

        // Scale by inverse of stationary distribution
        Vector<Scalar> tmp2 = EntryWiseInvert(stationary_distrib);
        tmp1 = EntryWiseProduct(tmp1, tmp2);

        // z = P * x
        Vector<Scalar> z = RightApplyDir(graph, curr_iter);

        // z <-- z / 2 + tmp1 / 2
        Add(z, tmp1, 0.5, 0.5, z);

        // Finally, orthogonalize against the trivial solution and scale
        OrthogAllOnes(z);
        z.Scale(1.0 / Norm1(z));
        
        return z;
    }

    
    template <typename Scalar>
    Vector<Scalar> DirEvec(PNGraph& graph, Vector<Scalar>& stationary_distrib,
                           int max_iter, double tol) {
        Vector<Scalar> curr_vec = RandomVector<Scalar>(graph->GetNodes());
        OrthogAllOnes(curr_vec);
        curr_vec.Scale(1.0 / Norm1(curr_vec));
        double diff_orig = 2.0;
        for (int i = 0; i < max_iter; ++i) {
            Vector<Scalar> save_vec = curr_vec;  // copy
            curr_vec = DirEvecIter(graph, curr_vec, stationary_distrib);
            double diff = Norm1Diff(curr_vec, save_vec);
            if (i == 0) { diff_orig = diff; }
            if (i > 0 && diff / diff_orig < tol) { break; }
        }
        return curr_vec;
    }


    template <typename Scalar>
    Vector<Scalar> UndirEvecIter(PUNGraph& graph, Vector<Scalar>& curr_iter) {
        // y = P * curr_iter
        Vector<Scalar> y = RightApplyUndir(graph, curr_iter);

        // Orthogonalize against the trivial solution and scale
        OrthogAllOnes(y);
        y.Scale(1.0 / Norm1(y));
        
        return y;
    }


    template <typename Scalar>
    Vector<Scalar> UndirEvec(PUNGraph& graph, int max_iter, double tol) {
        Vector<Scalar> curr_vec = RandomVector<Scalar>(graph->GetNodes());
        OrthogAllOnes(curr_vec);
        curr_vec.Scale(1.0 / Norm1(curr_vec));
        double diff_orig = 2.0;
        for (int i = 0; i < max_iter; ++i) {
            Vector<Scalar> save_vec = curr_vec;
            curr_vec = UndirEvecIter(graph, curr_vec);
            double diff = Norm1Diff(curr_vec, save_vec);
            if (i == 0) { diff_orig = diff; }
            if (i > 0 && diff / diff_orig < tol) {
                std::cout << "Breaking on iteration: " << i << std::endl;
                break;
            }
        }
        return curr_vec;
    }


    template <typename Scalar>
    Vector<Scalar> UndirStationaryDistrib(PUNGraph& graph) {
        Vector<Scalar> z(graph->GetNodes(), 0);
        int ind = 0;
        for (TUNGraph::TNodeI node = graph->BegNI(); node < graph->EndNI(); node++) {
            z(ind) = static_cast<double>(node.GetDeg()) / graph->GetEdges();
            ++ind;
        }
        return z;
    }


    template <typename Scalar>
    Vector<Scalar> DirStationaryDistrib(PNGraph& graph, int max_iter, double tol) {
        Vector<Scalar> curr_vec = RandomVector<Scalar>(graph->GetNodes());
        double diff_orig = 2.0;
        for (int i = 0; i < max_iter; ++i) {
            Vector<Scalar> save_vec = curr_vec;
            curr_vec = LeftApplyDir(graph, curr_vec);
            double diff = Norm1Diff(curr_vec, save_vec);
            if (i == 0) { diff_orig = diff; }
            if (i > 0 && diff / diff_orig < tol) { break; }
        }
        return curr_vec;
    }

    template <typename Scalar>
    Vector<Scalar> AsymmetricEvec(PNGraph& graph, int max_iter, double tol) {
        Vector<Scalar> curr_vec = RandomVector<Scalar>(graph->GetNodes());
        OrthogAllOnes(curr_vec);
        curr_vec.Scale(1.0 / Norm1(curr_vec));
        double diff_orig = 2.0;
        for (int i = 0; i < max_iter; ++i) {
            Vector<Scalar> save_vec = curr_vec;
            curr_vec = RightApplyDir(graph, curr_vec);
            OrthogAllOnes(curr_vec);
            curr_vec.Scale(1.0 / Norm1(curr_vec));
            double diff = Norm1Diff(curr_vec, save_vec);
            if (i == 0) { diff_orig = diff; }
#ifdef _VERBOSE_
            std::cout << "diff: " << diff << std::endl;
#endif
            if (i > 0 && diff / diff_orig < tol) { break; }
        }
        return curr_vec;
    }


    // Declarations
    template
    Vector<double> RightApplyDir(PNGraph& graph, Vector<double>& curr_iter);

    template
    Vector<double> LeftApplyDir(PNGraph& graph, Vector<double>& curr_iter);

    template
    Vector<double> RightApplyUndir(PUNGraph& graph, Vector<double>& curr_iter);

    template
    Vector<double> DirEvecIter(PNGraph& graph, Vector<double>& curr_iter,
                               Vector<double>& stationary_distrib);
    
    template
    Vector<double> UndirEvecIter(PUNGraph& graph, Vector<double>& curr_iter);

    template
    Vector<double> DirEvec(PNGraph& graph, Vector<double>& stationary_distrib,
                           int max_iter, double tol);

    template
    Vector<double> UndirEvec(PUNGraph& graph, int max_iter, double tol);

    template
    Vector<double> AsymmetricEvec(PNGraph& graph, int max_iter, double tol);

    template
    Vector<double> DirStationaryDistrib(PNGraph& graph, int max_iter, double tol);

    template
    Vector<double> UndirStationaryDistrib(PUNGraph& graph);
};  // namespace laplacian
