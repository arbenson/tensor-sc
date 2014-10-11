#ifndef _LINALG_HPP_
#define _LINALG_HPP_

#include "vector.hpp"

#include <ctime>
#include <functional>

// x += alpha * y
template <typename Scalar>
void Update(Vector<Scalar>& x, Scalar alpha, Vector<Scalar>& y) {
  assert(x.n() == y.n());
  for (int i = 0; i < x.n(); ++i) {
    x(i) += alpha * y(i);
  }
}


// Return a vector with all entries the same and with unit 1-norm.
//
// n size of the vector
template <typename Scalar>
Vector<Scalar> UniformVector(int n) {
  return Vector<Scalar>(n, (Scalar) 1.0 / n);
}


// Return a vector with positive uniform random entries.
//
// n is the size of the vector
template <typename Scalar>
Vector<Scalar> RandomVector(int n) {
  Vector<Scalar> vec(n);
  srand (time(NULL));
  for (int i = 0; i < vec.n(); ++i) {
    vec(i) = ((Scalar) rand() / RAND_MAX) * 1024;
  }
  return vec;
}


// z = alpha1 * x + alpha 2 * y
template <typename Scalar>
void Add(Vector<Scalar>& x, Vector<Scalar>& y, Scalar alpha1, Scalar alpha2, Vector<Scalar>& z) {
  assert(x.n() == y.n() && y.n() == z.n());

  Scalar *x_data = x.data();
  Scalar *y_data = y.data();
  Scalar *z_data = z.data();

  for (int i = 0; i < z.n(); ++i) {
    z_data[i] = alpha1 * x_data[i] + alpha2 * y_data[i];
  }
}


// Compute the 1-norm of x
template <typename Scalar>
double Norm1(Vector<Scalar>& x) {
  double sum = 0.0;
  Scalar *data = x.data();
  for (int i = 0; i < x.n(); ++i) {
    if (data[i] > 0) {
      sum += data[i];
    } else {
      sum -= data[i];
    }
  }
  return sum;
}


// Compute the 1-norm difference of x
template <typename Scalar>
double Norm1Diff(Vector<Scalar>& x, Vector<Scalar>& y) {
  double sum = 0.0;
  assert(x.n() == y.n());
  Scalar *x_data = x.data();
  Scalar *y_data = y.data();
  for (int i = 0; i < x.n(); ++i) {
    Scalar curr = x_data[i] - y_data[i];
    if (curr > 0) {
      sum += curr;
    } else {
      sum -= curr;
    }
  }
  return sum;
}


// Compute the redisual for the power iteration.
//
// old_vec is the vector from the last iteration
// new_vec is the vector from the current iteration
template <typename Scalar>
double PowerIterResid(Vector<Scalar>& old_vec, Vector<Scalar>& new_vec) {
    double diff1 = Norm1Diff(old_vec, new_vec);
    Vector<Scalar> old_vec_copy = old_vec;
    old_vec_copy.Scale(-1.0);
    double diff2 = Norm1Diff(old_vec_copy, new_vec);
    return std::min(diff1, diff2);
}


// Orthogonalize a vector against the all ones vector.
template <typename Scalar>
void OrthogAllOnes(Vector<Scalar>& vec) {
  double sub = vec.Sum() / vec.n();
  Scalar *data = vec.data();
  for (int i = 0; i < vec.n(); ++i) {
    data[i] -= sub;
  }
}


// Return entry-wise product of two vector
template<typename Scalar>
Vector<Scalar> EntryWiseProduct(Vector<Scalar>& x, Vector<Scalar>& y) {
    assert(x.n() == y.n());
    Vector<Scalar> res(x.n(), 0.0);
    for (int i = 0; i < res.n(); ++i) {
	res(i) = x(i) * y(i);
    }
    return res;
}


// Return the entry-wise inversion of a vector.
template <typename Scalar>
Vector<Scalar> EntryWiseInvert(Vector<Scalar>& x) {
    Vector<Scalar> y = x;
    for (int i = 0; i < y.n(); ++i) {
	if (y(i) != 0) {
	    y(i) = 1.0 / y(i);
	}
    }
    return y;
}


// Return the dot product of x and y.
template<typename Scalar>
double Dot(Vector<Scalar>& x, Vector<Scalar>& y) {
    assert(x.n() == y.n());
    double sum = 0.0;
    for (int i = 0; i < y.n(); ++i) {
	sum += x(i) * y(i);
    }
    return sum;
}

#endif  // _LINALG_HPP_
